/*
 * Controllable.cpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */


#include "components/Controllable.hpp"
#include "components/Velocity.hpp"

Controllable::Controllable(bool enabled) :
	enabled_(enabled)
{
}

void Controllable::control(entityx::Entity &entity, const SDL_Event &event)
{
	auto vel = entity.component<Velocity>();
	if (event.type == SDL_KEYDOWN){
		switch (event.key.keysym.sym){
			case SDLK_w:
				vel->y_ = -0.5f * vel->speed();
				break;
			case SDLK_s:
				vel->y_ = 0.5f * vel->speed();
				break;
			case SDLK_d:
				vel->x_ = 0.5f * vel->speed();
				break;
			case SDLK_a:
				vel->x_ = -0.5f * vel->speed();
				break;
		}
	}
	if (event.type == SDL_KEYUP){
		switch (event.key.keysym.sym){
			case SDLK_w:
			case SDLK_s:
				vel->y_ = 0;
				break;
			case SDLK_d:
			case SDLK_a:
				vel->x_ = 0;
				break;
		}
	}
}
