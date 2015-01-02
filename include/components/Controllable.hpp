/*
 * Controllable.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef CONTROLLABLE_HPP_
#define CONTROLLABLE_HPP_


#include <entityx/entityx.h>

#include <SDL2/SDL.h>

struct Controllable : entityx::Component<Controllable> {
	bool enabled_;
	Controllable(bool enabled = true);
	void control(entityx::Entity &entity, const SDL_Event &event);
};


#endif /* CONTROLLABLE_HPP_ */
