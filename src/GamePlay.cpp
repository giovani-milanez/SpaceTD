/*
 * GamePlay.cpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#include "GamePlay.hpp"
#include "Game.hpp"
#include "MapLoader.hpp"
#include "PathFinding.hpp"
#include "ResourceManager.hpp"
#include "ChooseCreep.hpp"
#include "BuyState.hpp"
#include "TurretClickState.hpp"
#include "TurretTrackState.hpp"
#include "TurretInfo.hpp"

#include "systems/InputSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/AnimationSystem.hpp"
#include "systems/CombatSystem.hpp"
#include "systems/BulletSystem.hpp"
#include "systems/ModifierSystem.hpp"
#include "systems/AuraSystem.hpp"
#include "systems/WaveSystem.hpp"

#include "components/Controllable.hpp"
#include "components/Draw.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Animate.hpp"
#include "components/AutoMove.hpp"
#include "components/Turret.hpp"
#include "components/Creep.hpp"
#include "components/Health.hpp"
#include "components/MouseTrack.hpp"
#include "components/Box.hpp"
#include "components/Slot.hpp"
#include "components/LifeTime.hpp"

#include <SDL2/SDL_mixer.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstdlib>

struct TurretFrame : public Box
{
	TurretFrame(GamePlay* gp, entityx::Entity turret, const SDL_Rect& box, const SDL_Color& color) : Box(box, color), gp_(gp), turret_(turret) {};

	void onClick()
	{
		if(gp_->paused_)
			return;
#ifdef ANDROID_BUILD
		Game::getInstance().pushState(std::make_shared<TurretClickState>(turret_, gp_));
#endif
	}
	void onOver()
	{
		if(gp_->paused_)
			return;
#ifndef ANDROID_BUILD
		Game::getInstance().pushState(std::make_shared<TurretClickState>(turret_, gp_));
#endif
	}
	void onOut()
	{
	}

	GamePlay* gp_;
	entityx::Entity turret_;
};

struct TurretBuyBox : public Box
{
	TurretBuyBox(GamePlay* gp, entityx::Entity turret, const TurretInfo& ti, const SDL_Rect& box, const SDL_Color& color) : Box(box, color), gp_(gp), turret_(turret), ti_(ti) {};

#ifndef ANDROID_BUILD
	void onClick()
	{
		//Game::getInstance().pushState(std::make_shared<TurretTrackState>(ti_, gp_));
	}
	void onOver()
	{
		if(gp_->paused_)
			return;
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		SDL_SetCursor(cursor);

		Game::getInstance().pushState(std::make_shared<BuyState>(turret_, ti_, gp_));
	}
	void onOut()
	{
		if(gp_->paused_)
			return;
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(cursor);
	}
#else
	void onClick()
	{
		if(gp_->paused_)
			return;
		Game::getInstance().pushState(std::make_shared<BuyState>(turret_, ti_, gp_));
	}
	void onOver()
	{
//		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
//		SDL_SetCursor(cursor);
//
//		Game::getInstance().pushState(std::make_shared<BuyState>(turret_, ti_, gp_));
	}
	void onOut()
	{
//		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
//		SDL_SetCursor(cursor);
	}
#endif

	GamePlay* gp_;
	entityx::Entity turret_;
	TurretInfo ti_;
};


GamePlay::GamePlay() :
	game_(Game::getInstance()),
	rm_(ResourceManager::getInstance()),
	textFont_(rm_.getFont("resource/arial.ttf", 16, true)),
	map_(JSONMapLoader().loadMap("resource/mapa.json")),
	playerGold_(90),
	playerLife_(5),
	turrets_({ TurretInfo(TurretType::T1), TurretInfo(TurretType::T2), TurretInfo(TurretType::T3),
				TurretInfo(TurretType::T4), TurretInfo(TurretType::T5), TurretInfo(TurretType::T6),
				TurretInfo(TurretType::T7)}),
	creepStart_({atoi(map_.properties["start_x"].c_str()) * map_.tileW, atoi(map_.properties["start_y"].c_str()) * map_.tileH, map_.tileW, map_.tileH}),
	creepEnd_({atoi(map_.properties["end_x"].c_str()) * map_.tileW, atoi(map_.properties["end_y"].c_str()) * map_.tileH, map_.tileW, map_.tileH})
{

	music_ = Mix_LoadMUS("resource/Undaunted.ogg");
	if(!music_)
	{
	    printf("Mix_LoadMUS: %s\n", Mix_GetError());
	}
	else
	{
		Mix_PlayMusic( music_, -1 );
	}

	turretToUpgrade_ = entityx::Entity::INVALID;
	systems.add<InputSystem>();
	systems.add<MovementSystem>();
	systems.add<RenderSystem>();
	systems.add<AnimationSystem>();
	systems.add<CombatSystem>();
	systems.add<BulletSystem>();
	systems.add<ModifierSystem>();
	systems.add<AuraSystem>();
	systems.add<WaveSystem>(events);

	systems.configure();
	configure(this->events);

	auto background = Texture("resource/tower-defense/tower-defense-background-stars.png");
	background.height_ = game_.screen_height_;
	background.width_ = game_.screen_width_;
	entityx::Entity bg = entities.create();
	bg.assign<Position>();
	bg.assign<Draw>(std::move(background));

	for(auto& layer : map_.layers)
	{
		std::cout << layer.name << std::endl;
		if(layer.name == "slots")
			colliders_.reserve(layer.tiles.size());

		for(auto& tile : layer.tiles)
		{
			entityx::Entity tileEntity = entities.create();
			tileEntity.assign<Position>(tile.rect.x, tile.rect.y);
			tileEntity.assign<Draw>(Texture(tile.imagePath));
			tileEntity.component<Draw>()->crop_ = tile.crop;
			
			if(layer.name == "slots")
			{
				colliders_.push_back(tile.rect);
				tileEntity.assign<Slot>(tile.rect);
			}
		}
	}

	creepPath_ = getPath(creepStart_, creepEnd_, map_, colliders_);

	//auto base = entities.create();	
	//base.assign<Draw>(std::make_shared<TextureDraw>(Texture("resource/tower-defense-turrets/turret-base.gif")));
	//base.assign<Position>(6 * 46 + 3, 7 * 46);

	int beginX = 12 * 46;
	int beginY = 2 * 46;
	int i = 0;
	for(auto& turret : turrets_)
	{
		auto turretE = entities.create();
		turretE.assign<Position>(beginX, beginY + (i * 46));
		turretE.assign<Draw>(Texture(turret.image_), -90);
		turretE.assign<TurretBuyBox>(this, turretE, turret, SDL_Rect{ beginX, beginY + (i * 46), 40, 40 }, SDL_Color{ 0, 255, 0, 0 });
		turretsE_.push_back(turretE);
		i++;
	}

	fps_ = entities.create();
	gold_ = entities.create();
	life_ = entities.create();
	fps_.assign<Position>(0, 0);

	std::stringstream ss;
	ss << game_.fps_;

	std::string fpsText = "FPS: " + ss.str();
	fps_.assign<Draw>(Texture(fpsText, SDL_Color{ 0, 255, 255, 255 }, textFont_));

	ss.str("");
	ss.clear();
	ss << playerGold_;
	std::string goldText = "GOLD: " + ss.str();
	Texture goldTexture(goldText, SDL_Color{ 255, 255, 0, 255 }, textFont_);
	gold_.assign<Position>(game_.screen_width_ - goldTexture.width_, 0);

	ss.str("");
	ss.clear();
	ss << playerLife_;
	std::string lifeText = "LIFE: " + ss.str();
	life_.assign<Draw>(Texture(lifeText, SDL_Color{ 0, 255, 0, 255 }, textFont_));
	life_.assign<Position>(game_.screen_width_ - goldTexture.width_, goldTexture.height_ + 5);
	gold_.assign<Draw>(std::move(goldTexture));

	loose_ = false;
	paused_ = false;
}

GamePlay::~GamePlay()
{
	if(music_)
	{
		Mix_HaltMusic();
		Mix_FreeMusic( music_ );
	}
}

void GamePlay::handleEvents(const float deltaTime)
{
	systems.update<InputSystem>(deltaTime);
}

void GamePlay::receive(const InputEvent &input)
{
	switch (input.evt_.type)
	{
	case SDL_QUIT:
		game_.toPop_ = name();
		break;
	case SDL_KEYUP:
		if (input.evt_.key.keysym.sym == SDLK_p)
		{
			paused_ = !paused_;
		}
		else
		{
			if(paused_)
				return;
		}

		if (input.evt_.key.keysym.sym == SDLK_AC_BACK)
		{
			game_.toPop_ = name();
		}
		else if (input.evt_.key.keysym.sym == SDLK_v)
		{
			game_.pushState(std::make_shared<ChooseCreep>(*this));
		}
		else if (input.evt_.key.keysym.sym == SDLK_k)
		{
			for (auto creep : entities.entities_with_components<Creep>())
			{
				creep.component<Creep>()->onDead(entities, events);
//				events.emit<DestroyEvent>(creep, entities);
//				creep.destroy();
			}

			for (auto turret : entities.entities_with_components<Turret>())
			{
				turret.destroy();
			}
		}
		else if(input.evt_.key.keysym.sym == SDLK_l)
		{
			for (auto turret : entities.entities_with_components<Turret>())
			{
				turret.component<Turret>()->levelUp();
			}
		}
		break;
#ifndef ANDROID_BUILD
	case SDL_MOUSEBUTTONUP:
		switch (input.evt_.button.button)
		{
		case SDL_BUTTON_RIGHT:
			if(lastSelectedCreep_.image_ != "")
			{
				createCreep(lastSelectedCreep_);
			}
			break;
		}
#endif
	}
}

void GamePlay::receive(const DestroyEvent &input)
{
	if(input.entity_.has_component<Creep>())
	{
		auto creep = input.entity_.component<Creep>();
		playerGold_ += creep->info_.gold_;
	}
}

void GamePlay::receive(const AutoMoveEndEvent &input)
{
	playerLife_--;
	if(playerLife_ == 0)
		loose_ = true;
}

void GamePlay::receive(const WaveSpawn &input)
{
	std::cout << "GP SPAWN!" << std::endl;
	createCreep(input.ci_, input.quantity_);
}
void GamePlay::receive(const WaveDone &input)
{
	std::cout << "DONE!" << std::endl;
	auto text = entities.create();
	auto texture = Texture("WELL DONE!", SDL_Color { 255, 255, 255, 255 }, rm_.getFont("resource/space-age.ttf", 30));
	text.assign<Position>(game_.screen_width_ / 2 - texture.width_ / 2, game_.screen_height_ / 2 - texture.height_ / 2);
	text.assign<LifeTime>(5, true);
	text.assign<Draw>(std::move(texture));
	text.component<Draw>()->fadeOnLifeTime_ = true;

}
void GamePlay::receive(const WaveBegin &input)
{
	std::cout << "START WAVE!" << std::endl;
	auto text = entities.create();
	auto texture = Texture("PREPARE YOUR TURRETS!", SDL_Color { 255, 255, 255, 255 }, rm_.getFont("resource/space-age.ttf", 30));
	text.assign<Position>(game_.screen_width_ / 2 - texture.width_ / 2, game_.screen_height_ / 2 - texture.height_ / 2);
	text.assign<LifeTime>(5, true);
	text.assign<Draw>(std::move(texture));
	text.component<Draw>()->fadeOnLifeTime_ = true;
}
void GamePlay::receive(const WaveEnd &input)
{
	std::cout << "END WAVE!" << std::endl;
	auto text = entities.create();
	auto texture = Texture("NEXT WAVE IS COMMING!", SDL_Color { 255, 255, 255, 255 }, rm_.getFont("resource/space-age.ttf", 30));
	text.assign<Position>(game_.screen_width_ / 2 - texture.width_ / 2, game_.screen_height_ / 2 - texture.height_ / 2);
	text.assign<LifeTime>(5, true);
	text.assign<Draw>(std::move(texture));
	text.component<Draw>()->fadeOnLifeTime_ = true;
}

void GamePlay::createCreep(const CreepInfo& creepInfo, int quantity)
{
	for(int i = 0; i < quantity; i++)
	{
		auto creep = entities.create();
		creep.assign<Creep>(creep, creepInfo);
		creep.assign<AutoMove>(creepPath_);
		creep.component<AutoMove>()->destroyOnFinish_ = true;
		creep.assign<Position>(creepStart_.x - (i * 50), creepStart_.y);
	}

	lastSelectedCreep_ = creepInfo;
}
void GamePlay::createTurret(const TurretInfo& ti, int posX, int posY)
{
	if(playerGold_ >= ti.cost_)
	{
		auto turret = entities.create();
		turret.assign<Position>(posX, posY);
		turret.assign<TurretFrame>(this, turret, SDL_Rect { posX, posY, 40, 40 }, SDL_Color { 0, 255, 0, 0 });
		turret.assign<Turret>(turret, ti);
		playerGold_ -= ti.cost_;
	}
}

void GamePlay::update(const float deltaTime)
{
	if(paused_)
		return;

	if(loose_)
	{
		for (auto turret : entities.entities_with_components<Turret>())
		{
			turret.destroy();
		}

		for (auto creep : entities.entities_with_components<Creep>())
		{
			creep.destroy();
		}

		auto text = entities.create();
		auto texture = Texture("LOOSER", SDL_Color { 255, 0, 0, 255 }, rm_.getFont("resource/space-age.ttf", 30));
		text.assign<Position>(game_.screen_width_ / 2 - texture.width_ / 2, game_.screen_height_ / 2 - texture.height_ / 2);
		text.assign<LifeTime>(5, true);
		text.assign<Draw>(std::move(texture));
		text.component<Draw>()->fadeOnLifeTime_ = true;
	}
	systems.update<WaveSystem>(deltaTime);
	systems.update<AuraSystem>(deltaTime);
	systems.update<MovementSystem>(deltaTime);
	systems.update<AnimationSystem>(deltaTime);
	systems.update<CombatSystem>(deltaTime);
	systems.update<BulletSystem>(deltaTime);
	systems.update<ModifierSystem>(deltaTime);

	if(	turretToUpgrade_ != entityx::Entity::INVALID)
	{
		auto turretE = entities.get(turretToUpgrade_);
		auto turretC = turretE.component<Turret>();
		if(turretC->info_.level_ != TurretLevel::L3 && playerGold_ >= turretC->upgradeCost())
		{
			playerGold_ -= turretC->upgradeCost();
			turretC->levelUp();
			game_.pushState(std::make_shared<TurretClickState>(turretE, this));

		}
		turretToUpgrade_ = entityx::Entity::INVALID;
	}

	for(auto& turret : turretsE_)
	{
		if(playerGold_ >= turret.component<TurretBuyBox>()->ti_.cost_)
		{
			turret.component<Draw>()->sprite_.setAlpha(255);
		}
		else
		{
			turret.component<Draw>()->sprite_.setAlpha(128);
		}
	}

	std::stringstream ss;
	ss << game_.fps_;

	std::string fpsText = "FPS: " + ss.str();
	fps_.remove<Draw>();
	fps_.assign<Draw>(Texture(fpsText, SDL_Color{ 0, 255, 255, 255 }, textFont_));

	ss.str("");
	ss.clear();
	ss << playerGold_;
	std::string goldText = "GOLD: " + ss.str();
	Texture goldTexture(goldText, SDL_Color{ 255, 255, 0, 255 }, textFont_);
	gold_.remove<Position>();
	gold_.assign<Position>(game_.screen_width_ - goldTexture.width_, 0);

	ss.str("");
	ss.clear();
	ss << playerLife_;
	std::string lifeText = "LIFE: " + ss.str();
	life_.remove<Draw>();
	life_.assign<Draw>(Texture(lifeText, SDL_Color{ 0, 255, 0, 255 }, textFont_));
	life_.remove<Position>();
	life_.assign<Position>(game_.screen_width_ - goldTexture.width_, goldTexture.height_ + 5);

	gold_.remove<Draw>();
	gold_.assign<Draw>(std::move(goldTexture));
}

void GamePlay::render(const float deltaTime)
{
	systems.update<RenderSystem>(deltaTime);
}

void GamePlay::restart()
{
}

std::string GamePlay::name() const
{
	return "GamePlay";
}
