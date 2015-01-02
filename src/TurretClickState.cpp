/*
 * TurretClickState.cpp
 *
 *  Created on: 13/10/2014
 *      Author: giovani
 */

#include "TurretClickState.hpp"

#include "Game.hpp"
#include "GamePlay.hpp"
#include "ResourceManager.hpp"
#include "Operations.hpp"

#include "systems/InputSystem.hpp"
#include "systems/RenderSystem.hpp"

#include "components/Turret.hpp"
#include "components/Draw.hpp"
#include "components/Position.hpp"
#include "components/Box.hpp"
#include "components/Range.hpp"

#include "messagebox.h"

#include <thread>
#include <sstream>

bool receiveClick = false;

struct TurretClick : public Box
{
	TurretClick(GamePlay* gp, entityx::Entity turret, const SDL_Rect& box) : Box(box), gp_(gp), turret_(turret)// args_(std::make_shared<ThreadArgs>())
	{

	};

	void onClick()
	{
		if(receiveClick)
		{
			gp_->turretToUpgrade_ = turret_.id();
			turret_.component<Range>()->draw_ = false;
			Game::getInstance().toPop_ = "clickturret";
		}
	}
	void onOver()
	{
#ifndef ANDROID_BUILD
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		SDL_SetCursor(cursor);
#endif
	}
	void onOut()
	{
#ifndef ANDROID_BUILD
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(cursor);
#endif
	}
	GamePlay* gp_;
	entityx::Entity turret_;
//	std::shared_ptr<ThreadArgs> args_;
};

TurretClickState::TurretClickState(entityx::Entity turret, GamePlay* gamePlay) :
	turret_(turret),
	gamePlay_(gamePlay),
	game_(Game::getInstance())
{
	systems.add<InputSystem>();
	systems.add<RenderSystem>();
	systems.configure();
	configure(this->events);

	turret.component<Range>()->draw_ = true;
	auto turretC = turret_.component<Turret>();
	auto posX = turret_.component<Position>()->x_;
	auto posY = turret_.component<Position>()->y_;
//	posY += 20;
	posX += 40;

	auto& rm = ResourceManager::getInstance();
	TTF_Font* fontBold = rm.getFont("resource/arial.ttf", 10, true);
	TTF_Font* fontNormal = rm.getFont("resource/arial.ttf", 10);

//	SDL_Color white = { 255, 255, 255, 255 };
//	SDL_Color blue = { 0, 0, 255, 255 };
//	SDL_Color red = { 255, 0, 0, 255 };
	SDL_Color yellow = { 255, 255, 0, 255 };
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color lightGreen = { 176, 255, 195, 255 };
	SDL_Color gray = { 184, 184, 184, 255 };

	int offset = 12;
	int sumOffset = offset;

	clickE_ = entities.create();
	clickE_.assign<TurretClick>(gamePlay_, turret_, getRect(turret_));
	menu_ = entities.create();
	auto texture = Texture("resource/tower-defense/stats-background.png");
	auto menuW = texture.width_;

	menu_.assign<Position>(posX, posY);

	auto nameE = entities.create();
	auto nameT = Texture(turretC->info_.name_, green, fontBold);
	nameE.assign<Position>(posX + (texture.width_ - nameT.width_) / 2, posY);
	nameE.assign<Draw>(std::move(nameT));
	menu_.assign<Draw>(std::move(texture));

	std::stringstream ss;
	ss << ((int) turretC->info_.level_);

	auto levelE = entities.create();
	auto levelT = Texture("Level " + ss.str(), gray, fontNormal);
	int levelTw = levelT.width_;
	levelE.assign<Draw>(std::move(levelT));
	levelE.assign<Position>(posX + 7, posY + sumOffset);

	if(turretC->info_.level_ != TurretLevel::L3)
	{
		ss.clear();
		ss.str("");
		ss << ((int) turretC->upgradeCost());

		auto upgradeE = entities.create();
		auto upgradeT = Texture(ss.str(), yellow, fontBold);

		upgradeE.assign<Position>(posX + menuW - upgradeT.width_ - 4, posY + sumOffset);
		upgradeE.assign<Draw>(std::move(upgradeT));
	}
	sumOffset += offset + 10;

	auto drawIcon = [&, this](int x, int y, std::string iconName)
		{
			entityx::Entity icon = entities.create();
			icon.assign<Position>(x, y);
			icon.assign<Draw>(Texture("resource/"+iconName+".gif"));
		};

	int iconX = levelE.component<Position>()->x_ + levelTw;
	int iconY = levelE.component<Position>()->y_;

	if(turretC->info_.splash_range_ != 0)
	{
		drawIcon(iconX, iconY, "area");
		iconX += 11;
	}
	if(turretC->info_.stun_duration_ != 0)
	{
		drawIcon(iconX, iconY, "stun");
		iconX += 11;
	}
	if(turretC->info_.slow_duration_ != 0)
	{
		drawIcon(iconX, iconY, "slow");
		iconX += 11;
	}
	if(turretC->info_.bleed_duration_ != 0)
	{
		drawIcon(iconX, iconY, "bleed");
		iconX += 11;
	}

	auto drawStat = [&, this](int startX, int y, int quantity, bool current)
			{
				for(int i = 1; i <= quantity; i++)
				{
					auto statE = entities.create();

					statE.assign<Draw>(Texture(current ? "resource/tower-defense/stat-current.png" : "resource/tower-defense/stat-upgrade.png"));
					statE.assign<Position>(startX + i * 8, y);
				}
			};


	int dmg = turretC->damageLevel();
	auto dmgE = entities.create();
	auto dmgT = Texture("DMG", lightGreen, fontNormal);
	int textureWidth = dmgT.width_;
	dmgE.assign<Draw>(std::move(dmgT));
	dmgE.assign<Position>(posX + 7, posY + sumOffset);
	drawStat(posX + 5 + textureWidth, posY + sumOffset + 2, dmg, true);

	int nextDmg = turretC->nextDamageLevel();
	drawStat(posX + 5 + textureWidth + dmg * 8, posY + sumOffset + 2, nextDmg - dmg, false);
	sumOffset += offset;

	int range = turretC->rangeLevel();
	auto rangeE = entities.create();
	rangeE.assign<Draw>(Texture("RNG", lightGreen, fontNormal));
	rangeE.assign<Position>(posX + 7, posY + sumOffset);
	drawStat(posX + 5 + textureWidth, posY + sumOffset + 2, range, true);

	int nextRange = turretC->nextRangeLevel();
	drawStat(posX + 5 + textureWidth + range * 8, posY + sumOffset + 2, nextRange - range, false);
	sumOffset += offset;

	int cd = turretC->cooldownLevel();
	auto cdE = entities.create();
	cdE.assign<Draw>(Texture("CD", lightGreen, fontNormal));
	cdE.assign<Position>(posX + 7, posY + sumOffset);
	drawStat(posX + 5 + textureWidth, posY + sumOffset + 2, cd, true);

	int nextCd = turretC->nextCooldownLevel();
	drawStat(posX + 5 + textureWidth + cd * 8, posY + sumOffset + 2, nextCd - cd, false);
	sumOffset += offset;

}

TurretClickState::~TurretClickState()
{
}

void TurretClickState::handleEvents(const float deltaTime)
{
	systems.update<InputSystem>(deltaTime);
}

void TurretClickState::update(const float deltaTime)
{
	receiveClick = true;
#ifndef ANDROID_BUILD
	int x, y;
	SDL_GetMouseState(&x, &y);

	SDL_Rect mouseRect{ x, y, 2, 2 };
	SDL_Rect turretRect = getRect(turret_);

	if (!collidesWith(mouseRect, turretRect))
	{	
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(cursor);
		turret_.component<Range>()->draw_ = false;
		Game::getInstance().toPop_ = name();
	}
#else

#endif
}

void TurretClickState::render(const float deltaTime)
{
	systems.update<RenderSystem>(deltaTime);
}

void TurretClickState::restart()
{
}

std::string TurretClickState::name() const
{
	return "clickturret";
}

void TurretClickState::receive(const InputEvent &input)
{
	if(input.evt_.type == SDL_KEYUP)
	{
		if (input.evt_.key.keysym.sym == SDLK_AC_BACK)
		{
			turret_.component<Range>()->draw_ = false;
			SDL_Log("back android");
			game_.toPop_ = name();
		}
	}

#ifdef ANDROID_BUILD
	if(input.evt_.type == SDL_FINGERDOWN)
	{
		int x = input.evt_.tfinger.x * game_.screen_width_;
		int y = input.evt_.tfinger.y * game_.screen_height_;

		SDL_Rect mouseRect{ x, y, 10, 10 };
		SDL_Rect turretRect = getRect(turret_);

		if (!collidesWith(mouseRect, turretRect))
		{
			turret_.component<Range>()->draw_ = false;
			Game::getInstance().toPop_ = name();
		}
		else
		{
			clickE_.component<TurretClick>()->onClick();
		}
	}
#endif
}
