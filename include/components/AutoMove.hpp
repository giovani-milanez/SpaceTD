/*
 * AutoMove.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef AUTOMOVE_HPP_
#define AUTOMOVE_HPP_

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

struct AutoMove : entityx::Component<AutoMove> 
{	

	AutoMove(const std::vector<SDL_Rect>& paths) : paths_(paths), destroyOnFinish_(false) {}

	std::vector<SDL_Rect> paths_;
	bool destroyOnFinish_;
};



#endif /* VELOCITY_HPP_ */
