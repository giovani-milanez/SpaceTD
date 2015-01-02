/*
 * Damage.hpp
 *
 *  Created on: 07/10/2014
 *      Author: giovani
 */

#ifndef DAMAGE_HPP_
#define DAMAGE_HPP_


#include "entityx/entityx.h"

struct Damage : entityx::Component<Damage>
{
	Damage(int from, int to) : from_(from), to_(to), bonus_(0) {}

	int from_, to_;
	int bonus_;
	
};


#endif /* DAMAGE_HPP_ */
