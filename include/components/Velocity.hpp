/*
 * Velocity.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef VELOCITY_HPP_
#define VELOCITY_HPP_

#include "entityx/entityx.h"

struct Velocity : entityx::Component<Velocity> 
{
	float x_, y_;
	int speedBonus_;
	Velocity(float x = 0, float y = 0, int speed = 64) : x_(x), y_(y), speedBonus_(0), speed_(speed) {}
	int speedBase() const
	{
		return speed_;
	}
	int speed() const
	{
		return speed_ + speedBonus_;
	}
private:
	int speed_;
};



#endif /* VELOCITY_HPP_ */
