/*
 * InputEvent.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef INPUTEVENT_HPP_
#define INPUTEVENT_HPP_

#include "entityx/entityx.h"

#include "SDL2/SDL.h"

struct InputEvent : entityx::Event<InputEvent> 
{
	SDL_Event evt_;
	InputEvent(SDL_Event evt) : evt_(evt){}
};

#endif
