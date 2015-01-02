/*
 * InputSystem.cpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#include "systems/InputSystem.hpp"
#include "components/Controllable.hpp"
#include "components/Box.hpp"
#include "components/Position.hpp"
#include "components/MouseTrack.hpp"
#include "components/ViewPort.hpp"
#include "events/InputEvent.hpp"
#include "Game.hpp"
#include "Operations.hpp"

#include <SDL2/SDL.h>

#include <iostream>

void InputSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		events.emit<InputEvent>(e);
		for (auto entity : es.entities_with_components<Controllable>())
		{
			auto cont = entity.component<Controllable>();
			if (cont->enabled_)
				cont->control(entity, e);
		}

		switch (e.type)
		{
	#ifdef ANDROID_BUILD
		case SDL_FINGERMOTION:
	#else
		case SDL_MOUSEMOTION:
	#endif
			int x, y;
#ifdef ANDROID_BUILD
			x = e.tfinger.x * Game::getInstance().screen_width_;
			y = e.tfinger.y * Game::getInstance().screen_height_;
#else
			x = e.motion.x;
			y = e.motion.y;
#endif
			for (auto mt : es.entities_with_components<MouseTrack>())
			{
				if(mt.has_component<Position>())
				{
					auto pos = mt.component<Position>();
					pos->x_ = x - 20;
					pos->y_ = y - 20;
				}

				//mt.component<MouseTrack>()->onMotion();
			}
			for (auto box : es.entities_with_components<Box>())
			{
				auto b = box.component<Box>();
				SDL_Point mousePosRunning;
				mousePosRunning.x = e.button.x;
				mousePosRunning.y = e.button.y;
				SDL_Rect mouseRect { x, y, 20, 20 };
				SDL_Rect boxRect = b->box_;

				if(box.has_component<ViewPort>() && box.has_component<Position>())
				{
					auto pos = box.component<Position>();
					boxRect.x += (int)pos->x_;
					boxRect.y += (int)pos->y_;
				}

				//if(collidesWith(mouseRect, boxRect))
				if (SDL_EnclosePoints(&mousePosRunning, 1, &boxRect, NULL))
				{
					b->onOver();
					b->in_ = true;
				}
				else
				{
					if (b->in_)
					{
						b->onOut();
					}

					b->in_ = false;
				}
				//std::cout << mousePosRunning.x << "," << mousePosRunning.y << std::endl;
				//std::cout << boxRect.x << "," << boxRect.y << " - " << boxRect.w << "x" << boxRect.h << std::endl;
			}
			break;
	#ifdef ANDROID_BUILD
		case SDL_FINGERDOWN:
	#else
		case SDL_MOUSEBUTTONDOWN:
	#endif

#ifndef ANDROID_BUILD

			switch (e.button.button)
			{
			case SDL_BUTTON_LEFT:
#endif
				for (auto mt : es.entities_with_components<MouseTrack>())
				{
					mt.component<MouseTrack>()->onClick();
				}
				for (auto box : es.entities_with_components<Box>())
				{
					auto b = box.component<Box>();
					#ifndef ANDROID_BUILD
					SDL_Point mousePosRunning;
					mousePosRunning.x = e.button.x;
					mousePosRunning.y = e.button.y;
					#else
					SDL_Point mousePosRunning;
					mousePosRunning.x = e.tfinger.x * Game::getInstance().screen_width_;
					mousePosRunning.y = e.tfinger.y * Game::getInstance().screen_height_;
					#endif
					SDL_Rect boxRect = b->box_;

					if(box.has_component<ViewPort>() && box.has_component<Position>())
					{
						auto pos = box.component<Position>();
						boxRect.x += (int)pos->x_;
						boxRect.y += (int)pos->y_;
					}
					if (SDL_EnclosePoints(&mousePosRunning, 1, &boxRect, NULL))
					{
						b->onClick();
					}
				}
#ifndef ANDROID_BUILD
				break;
			}
#endif
		}
	}
}
