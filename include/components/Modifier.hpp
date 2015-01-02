/*
 * Modifier.hpp
 *
 *  Created on: 07/10/2014
 *      Author: giovani
 */

#ifndef MODIFIER_HPP_
#define MODIFIER_HPP_

#include "components/LifeTime.hpp"

#include <entityx/entityx.h>


struct Stun : entityx::Component<Stun>
{
	Stun(LifeTime lifeTime, double chance) : lifeTime_(lifeTime), chance_(chance) {}

	LifeTime lifeTime_;
	double chance_;
};

struct Slow : entityx::Component<Slow>
{
	Slow(LifeTime lifeTime, double pecentage) : lifeTime_(lifeTime), pecentage_(pecentage) {}

	LifeTime lifeTime_;
	double pecentage_;
};

struct Splash : entityx::Component<Splash>
{
	Splash(int range) : range_(range) {}

	int range_;
};

struct Bleed : entityx::Component<Bleed>
{
	Bleed(LifeTime lifeTime, double dps) : lifeTime_(lifeTime), dps_(dps) {}

	LifeTime lifeTime_;
	double dps_;
};

#endif /* MODIFIER_HPP_ */
