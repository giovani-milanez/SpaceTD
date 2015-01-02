/*
 * Velocity.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef POSITION_HPP_
#define POSITION_HPP_

#include "entityx/entityx.h"

struct Position : entityx::Component<Position> 
{
	float x_, y_;
	Position(float x = 0, float y = 0) : x_(x), y_(y) {}
};

#endif
