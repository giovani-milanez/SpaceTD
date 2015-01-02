/*
 * LifeTime.hpp
 *
 *  Created on: 02/10/2014
 *      Author: giovani
 */

#ifndef LIFETIME_HPP_
#define LIFETIME_HPP_

#include "Timer.hpp"

#include "entityx/entityx.h"

struct LifeTime : public Timer, public entityx::Component<LifeTime>
{
	LifeTime(double lifeTimeSeconds, bool destroyWhenDone = false) : Timer(lifeTimeSeconds), destroyWhenDone_(destroyWhenDone) {}
	bool destroyWhenDone_;
};


#endif /* LIFETIME_HPP_ */
