/*
 * ChooseCreep.cpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#include "ChooseCreep.hpp"

#include "Game.hpp"
#include "GamePlay.hpp"
#include "ResourceManager.hpp"

#include "components/Box.hpp"
#include "components/Position.hpp"
#include "components/Draw.hpp"
#include "components/Animate.hpp"
#include "components/ViewPort.hpp"

#include "systems/InputSystem.hpp"
#include "systems/AnimationSystem.hpp"
#include "systems/RenderSystem.hpp"

#include <sstream>

struct CreepBox : public Box
{
	CreepBox(GamePlay& gp, const CreepInfo& ti, const SDL_Rect& box, const SDL_Color& color) : Box(box, color), gp_(gp), ti_(ti) {};

	void onClick()
	{
		gp_.createCreep(ti_);

		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(cursor);
		Game::getInstance().toPop_ = "choosecreep";
	}
	void onOver()
	{
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		SDL_SetCursor(cursor);

		this->color_.a = 128;
	}
	void onOut()
	{
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(cursor);

		this->color_.a = 0;
	}
	GamePlay& gp_;
	CreepInfo ti_;
};

ChooseCreep::ChooseCreep(GamePlay& gamePlay) :
	gamePlay_(gamePlay),
	game_(Game::getInstance()),
	creeps_({
	CreepInfo(CreepColor::BLUE, CreepLevel::ONE), CreepInfo(CreepColor::GREEN, CreepLevel::ONE), CreepInfo(CreepColor::RED, CreepLevel::ONE), CreepInfo(CreepColor::YELLOW, CreepLevel::ONE),
	CreepInfo(CreepColor::BLUE, CreepLevel::TWO), CreepInfo(CreepColor::GREEN, CreepLevel::TWO), CreepInfo(CreepColor::RED, CreepLevel::TWO), CreepInfo(CreepColor::YELLOW, CreepLevel::TWO),
	CreepInfo(CreepColor::BLUE, CreepLevel::THREE), CreepInfo(CreepColor::GREEN, CreepLevel::THREE), CreepInfo(CreepColor::RED, CreepLevel::THREE), CreepInfo(CreepColor::YELLOW, CreepLevel::THREE)}),
	viewPorts_()
{
	systems.add<InputSystem>();
	systems.add<AnimationSystem>();
	systems.add<RenderSystem>();

	systems.configure();
	configure(this->events);
	ResourceManager& rm = ResourceManager::getInstance();

	if(getType() == WindowType::FULL)
	{
		Texture background("resource/tower-defense/tower-defense-background-stars.png");
		background.height_ = game_.screen_height_;
		background.width_ = game_.screen_width_;
		entityx::Entity bg = entities.create();
		bg.assign<Position>();
		bg.assign<Draw>(std::move(background));
	}

	TTF_Font* font = rm.getFont("resource/space-age.ttf", 14);
	SDL_Color white = { 255, 255, 255, 255 };


	auto text = [&, this](const SDL_Rect& vp, int x, int y, std::stringstream& str, SDL_Color color)
			{
				entityx::Entity turretText = entities.create();
				turretText.assign<ViewPort>(vp);
				turretText.assign<Position>(x, y);
				turretText.assign<Draw>(Texture(str.str(), color, font));
				str.str("");
				str.clear();
			};

	int x = 0;
	int y = 0;
	int vpSize = game_.screen_width_ / 4;
	for(auto& creep : creeps_)
	{
		std::stringstream ss;
		SDL_Rect viewport { x * vpSize, y * vpSize + 20, vpSize, vpSize };
		entityx::Entity box = entities.create();
		box.assign<CreepBox>(gamePlay_, creep, viewport, SDL_Color{ 0, 255, 0, 0 });

		entityx::Entity creepE = entities.create();
		creepE.assign<ViewPort>(viewport);
		creepE.assign<Position>(0, 15);
		creepE.assign<Draw>(Texture(creep.image_));
		creepE.assign<Animate>(Animation( Texture(creep.image_), 46, 46 , creep.animationFrames_ ));


		ss << "Health: " << creep.health_ << std::endl;
		text(viewport, 45, 15, ss, white);
		ss << "Speed: " << creep.speed_ << std::endl;
		text(viewport, 45, 25, ss, white);
		x++;
		if(x % 4 == 0)
		{
			y++;
			x = 0;
		}
	}
}

ChooseCreep::~ChooseCreep()
{
}

void ChooseCreep::handleEvents(const float deltaTime)
{
	systems.update<InputSystem>(deltaTime);
}
void ChooseCreep::update(const float deltaTime)
{
	systems.update<AnimationSystem>(deltaTime);
}
void ChooseCreep::render(const float deltaTime)
{
	systems.update<RenderSystem>(deltaTime);
}
void ChooseCreep::restart()
{

}
std::string ChooseCreep::name() const
{
	return "choosecreep";
}

void ChooseCreep::receive(const InputEvent &input)
{
	switch (input.evt_.type)
	{
		case SDL_QUIT:
			game_.toPop_ = name();
			break;
		case SDL_KEYUP:
		if (input.evt_.key.keysym.sym == SDLK_ESCAPE)
		{
			game_.toPop_ = name();
			break;
		}
	}
}
