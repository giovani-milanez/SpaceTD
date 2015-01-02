/*
 * BuyState.cpp
 *
 *  Created on: 16/10/2014
 *      Author: giovani
 */

#include "BuyState.hpp"

#include "GamePlay.hpp"
#include "Game.hpp"
#include "Operations.hpp"
#include "ResourceManager.hpp"
#include "TurretTrackState.hpp"

#include "systems/RenderSystem.hpp"
#include "systems/InputSystem.hpp"

#include "components/Turret.hpp"
#include "components/Draw.hpp"
#include "components/Position.hpp"
#include "components/Box.hpp"
#include "components/ViewPort.hpp"

#include <sstream>


BuyState::BuyState(entityx::Entity turret, const TurretInfo& ti, GamePlay* gamePlay) :
	gamePlay_(gamePlay),
	game_(Game::getInstance()),
	ti_(ti),
	turret_(turret)
{
	systems.add<RenderSystem>();
	systems.add<InputSystem>();
	systems.configure();
	configure(this->events);

	ResourceManager& rm = ResourceManager::getInstance();

	TTF_Font* font = rm.getFont("resource/space-age.ttf", 14);
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color blue = { 0, 191, 255, 255 };
	SDL_Color red = { 255, 0, 0, 255 };
	SDL_Color yellow = { 255, 255, 0, 255 };
	SDL_Color orange = { 255, 165, 0, 255 };
	SDL_Color gray = {208,208,208};

	auto text = [&, this](int x, int y, std::stringstream& str, SDL_Color color)
			{
				entityx::Entity turretText = entities.create();
				turretText.assign<Position>(x, y);
				turretText.assign<Draw>(Texture(str.str(), color, font));
				str.str("");
				str.clear();
			};
	auto drawIcon = [&, this](int x, int y, std::string iconName)
		{
			entityx::Entity icon = entities.create();
			icon.assign<Position>(x, y);
			icon.assign<Draw>(Texture("resource/"+iconName+".gif"));
		};

	std::stringstream ss;

	int posX = turret_.component<Position>()->x_ - 4 * 46;
	posX += 45;
	int posY = turret_.component<Position>()->y_;
	int lineOffset = 10;

	ss << ti.name_ << std::endl;
	text(posX , posY, ss, green);
	ss << ti.cost_ << " Gold" << std::endl;
	text(posX  + 20, posY+15, ss, yellow);

	ss << "Attack: " << ti.damge_from_ << "-" << ti.damge_to_ << std::endl;
	text(posX , posY+30, ss, white);
	ss << "Cooldown: " << ti.cooldown_ << std::endl;
	text(posX , posY+40, ss, white);
	ss << "Range: " << ti.range_ << std::endl;
	text(posX , posY+50, ss, white);
	int offset = 10;
	if(ti.splash_range_ != 0)
	{
		drawIcon(posX , posY+50 + offset, "area");
		ss << "Area damage" << std::endl;
		text(posX +11, posY+50 + offset, ss, orange);
		offset += 10;
	}
	if(ti.stun_duration_ != 0)
	{
		drawIcon(posX , posY+50 + offset, "stun");
		ss << "Stun: " << ti.stun_duration_ << "s " << ti.stun_chance_ << "%" << std::endl;
		text(posX +11, posY+50 + offset, ss, yellow);
		offset += 10;
	}
	if(ti.slow_duration_ != 0)
	{
		drawIcon(posX , posY+50 + offset, "slow");
		ss << "Slow: " << ti.slow_duration_ << "s " << ti.slow_percentage_ << "%" << std::endl;
		text(posX +11, posY+50 + offset, ss, blue);
		offset += 10;
	}
	if(ti.bleed_duration_ != 0)
	{
		drawIcon(posX , posY+50 + offset, "bleed");
		ss << "Bleed: " << ti.bleed_duration_ << "s " << ti.bleed_dps_ << " DPS" << std::endl;
		text(posX +11, posY+50 + offset, ss, red);
		offset += 10;
	}

	if(gamePlay_->playerGold_ >= ti.cost_)
	{
		ss << "Click to buy" << std::endl;
		text(posX , posY+60 + offset, ss, green);
	}
	else
	{
		ss << "Cannot afford" << std::endl;
		text(posX , posY+60 + offset, ss, gray);
	}
}

BuyState::~BuyState()
{
}

void BuyState::handleEvents(const float deltaTime)
{
	systems.update<InputSystem>(deltaTime);
}

void BuyState::update(const float deltaTime)
{
#ifndef ANDROID_BUILD
	SDL_Point mousePosRunning;

	SDL_GetMouseState(&mousePosRunning.x, &mousePosRunning.y);
	SDL_Rect turretRect = getRect(turret_);

	if (!SDL_EnclosePoints(&mousePosRunning, 1, &turretRect, NULL))
	{
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(cursor);
		Game::getInstance().toPop_ = name();
	}
#else

#endif
}

void BuyState::render(const float deltaTime)
{
	systems.update<RenderSystem>(deltaTime);
}

void BuyState::restart()
{
}

void BuyState::receive(const InputEvent &input)
{
#ifdef ANDROID_BUILD
	int x, y;
	switch (input.evt_.type)
	{
	case SDL_KEYUP:
		if (input.evt_.key.keysym.sym == SDLK_AC_BACK)
		{
			game_.toPop_ = name();
		}
		break;
	case SDL_FINGERUP:
		game_.toPop_ = name();
		break;
	case SDL_FINGERDOWN:
	case SDL_FINGERMOTION:
		x = input.evt_.tfinger.x * game_.screen_width_;
		y = input.evt_.tfinger.y * game_.screen_height_;

		SDL_Rect mouseRect{ x, y, 10, 10 };
		SDL_Rect turretRect = getRect(turret_);

		if (!collidesWith(mouseRect, turretRect))
		{
			Game::getInstance().toPop_ = name();
		}
		else
		{
			if(!gamePlay_->paused_ && gamePlay_->playerGold_ >= ti_.cost_)
			{
				gamePlay_->paused_ = true;
				Game::getInstance().toPop_ = name();
				Game::getInstance().pushState(std::make_shared<TurretTrackState>(ti_, gamePlay_, x ,y));
			}
		}
	}
#else
	int x, y;
	switch (input.evt_.type)
	{
	case SDL_KEYUP:
		if (input.evt_.key.keysym.sym == SDLK_AC_BACK)
		{
			game_.toPop_ = name();
		}
		break;
	case SDL_MOUSEBUTTONUP:
		SDL_GetMouseState(&x, &y);
		switch (input.evt_.button.button)
		{
		case SDL_BUTTON_LEFT:
			if(!gamePlay_->paused_ && gamePlay_->playerGold_ >= ti_.cost_)
			{
				gamePlay_->paused_ = true;
				Game::getInstance().toPop_ = name();
				Game::getInstance().pushState(std::make_shared<TurretTrackState>(ti_, gamePlay_, x ,y));
			}
			break;
		}
	}
#endif
}

std::string BuyState::name() const
{
	return "buystate";
}
