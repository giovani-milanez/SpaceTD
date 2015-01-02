/*
 * Box.hpp
 *
 *  Created on: 10/10/2014
 *      Author: giovani
 */

#ifndef Box_HPP_
#define Box_HPP_

#include "entityx/entityx.h"

#include "SDL2/SDL.h"

struct Box : entityx::Component<Box>
{
	virtual ~Box() {};
	Box(const SDL_Rect& box) : box_(box), color_({ 0, 0, 0, 0 }) {};
	Box(const SDL_Rect& box, const SDL_Color& color) : box_(box), color_(color) {};
	SDL_Rect box_;
	
	SDL_Color color_;

	virtual void onClick() = 0;
	virtual void onOver() = 0;
	virtual void onOut() = 0;

	bool in_ = false;
};


#endif /* Box_HPP_ */
