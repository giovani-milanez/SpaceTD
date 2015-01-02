/*
 * TurretTrackState.cpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#include "TurretTrackState.hpp"

#include "Game.hpp"
#include "GamePlay.hpp"
#include "ResourceManager.hpp"
#include "Operations.hpp"

#include "systems/InputSystem.hpp"
#include "systems/RenderSystem.hpp"

#include "components/Turret.hpp"
#include "components/Draw.hpp"
#include "components/Position.hpp"
#include "components/MouseTrack.hpp"
#include "components/Slot.hpp"
#include "components/Range.hpp"

#include <iostream>

struct TurretTrack : public MouseTrack
{
	TurretTrack(TurretTrackState* gp, entityx::Entity trackEntity, const TurretInfo& ti) :
		gp_(gp),
		trackEntity_(trackEntity),
		ti_(ti) {};
	void onClick()
	{
		bool positionOk = false;

		auto posX = (int) trackEntity_.component<Position>()->x_;
		auto posY = (int) trackEntity_.component<Position>()->y_;

		int tileX = posX / 46;
		int modX = posX % 46;
		if(modX > 23)
			tileX++;
		tileX = tileX * 46;

		int tileY = posY / 46;
		int modY = posY % 46;
		if(modY > 23)
			tileY++;

		tileY = tileY * 46;

		for(auto tile : gp_->gamePlay_->entities.entities_with_components<Slot>())
		{
			auto slot = tile.component<Slot>();
//			auto draw = tile.component<Draw>();
			if(!slot->occupied_ && slot->tileRect_.x == tileX && slot->tileRect_.y == tileY)
			{
				posX = slot->tileRect_.x + 3;
				posY = slot->tileRect_.y;
				slot->occupied_ = true;

//				draw->fillColor_.a = 0;
				positionOk = true;
				break;
			}
		}
		if(positionOk)
		{
			for(auto tile : gp_->gamePlay_->entities.entities_with_components<Slot>())
			{
				auto draw = tile.component<Draw>();
				draw->fillColor_.a = 0;
			}
			gp_->gamePlay_->createTurret(ti_, posX, posY);
			trackEntity_.destroy();
#ifndef ANDROID_BUILD
			SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
			SDL_SetCursor(cursor);
#endif
			gp_->gamePlay_->paused_ = false;
			Game::getInstance().removeState("turrettrack");
		}
		else
		{
#ifdef ANDROID_BUILD
			gp_->quitState();
#endif
		}
	}

	TurretTrackState* gp_;
	entityx::Entity trackEntity_;
	TurretInfo ti_;
};

TurretTrackState::TurretTrackState(const TurretInfo& ti, GamePlay* gamePlay, int x, int y) :
	gamePlay_(gamePlay),
	game_(Game::getInstance()),
	ti_(ti),
	quit_(false)
{
	systems.add<InputSystem>();
	systems.add<RenderSystem>();
	systems.configure();
	configure(this->events);

	trackE_ = entities.create();
	auto texture = Texture(ti.image_);
	texture.setAlpha(128);
	trackE_.assign<Draw>(std::move(texture));
	trackE_.assign<Position>(x - 20, y - 20);
	trackE_.assign<TurretTrack>(this, trackE_, ti_);
	trackE_.assign<Range>(ti_.range_);
	trackE_.component<Range>()->draw_ = true;
}

TurretTrackState::~TurretTrackState()
{
}

void TurretTrackState::handleEvents(const float deltaTime)
{
	systems.update<InputSystem>(deltaTime);
}

void TurretTrackState::update(const float deltaTime)
{
	if(!quit_)
	{
		auto posX = (int) trackE_.component<Position>()->x_;
		auto posY = (int) trackE_.component<Position>()->y_;

		int tileX = posX / 46;
		int modX = posX % 46;
		if(modX > 23)
			tileX++;
		tileX = tileX * 46;

		int tileY = posY / 46;
		int modY = posY % 46;
		if(modY > 23)
			tileY++;

		tileY = tileY * 46;

		for(auto tile : this->gamePlay_->entities.entities_with_components<Slot>())
		{
			auto slot = tile.component<Slot>();
			auto draw = tile.component<Draw>();
			if(slot->tileRect_.x == tileX && slot->tileRect_.y == tileY)
			{

				if(slot->occupied_)
					draw->fillColor_ = SDL_Color { 255, 0, 0, 128 };
				else
					draw->fillColor_ = SDL_Color { 0, 255, 0, 128 };
			}
			else
			{
				draw->fillColor_.a = 0;
			}
		}
	}
}

void TurretTrackState::render(const float deltaTime)
{
	systems.update<RenderSystem>(deltaTime);
}

void TurretTrackState::restart()
{
}

std::string TurretTrackState::name() const
{
	return "turrettrack";
}

void TurretTrackState::receive(const InputEvent& input)
{
	switch (input.evt_.type)
		{
		case SDL_QUIT:
			quitState();
			break;
		case SDL_KEYUP:
			if (input.evt_.key.keysym.sym == SDLK_ESCAPE)
			{
				quitState();
			}
			else if (input.evt_.key.keysym.sym == SDLK_AC_BACK)
			{
				SDL_Log("back android");
				quitState();
			}
			break;
#ifndef ANDROID_BUILD
		case SDL_MOUSEBUTTONUP:
			switch (input.evt_.button.button)
			{
			case SDL_BUTTON_RIGHT:
			{
				quitState();
				break;
			}
			}
			break;
#else
		case SDL_FINGERUP:
		{
			trackE_.component<TurretTrack>()->onClick();
			break;
		}
#endif
		}
}

void TurretTrackState::quitState()
{
	quit_ = true;
	for(auto tile : this->gamePlay_->entities.entities_with_components<Slot>())
	{
		auto draw = tile.component<Draw>();
		draw->fillColor_.a = 0;
	}

	#ifndef ANDROID_BUILD
	SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	SDL_SetCursor(cursor);
	#endif

	gamePlay_->paused_ = false;
	game_.toPop_ = name();
}
