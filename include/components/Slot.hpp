/*
 * Tile.hpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#ifndef SLOT_HPP_
#define SLOT_HPP_

#include <entityx/entityx.h>

#include <SDL2/SDL.h>

struct Slot : entityx::Component<Slot>
{
	Slot(const SDL_Rect& tileRect) : tileRect_(tileRect), occupied_(false) {}
	SDL_Rect tileRect_;
	bool occupied_;
};


#endif /* TILE_HPP_ */
