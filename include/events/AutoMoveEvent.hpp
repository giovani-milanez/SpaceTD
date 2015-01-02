/*
 * AutoMoveEvent.hpp
 *
 *  Created on: 17/10/2014
 *      Author: giovani
 */

#ifndef AUTOMOVEEVENT_HPP_
#define AUTOMOVEEVENT_HPP_


#include "entityx/entityx.h"

struct AutoMoveEndEvent : entityx::Event<AutoMoveEndEvent>
{
	AutoMoveEndEvent(entityx::Entity entity){}

	entityx::Entity entity_;
};


#endif /* AUTOMOVEEVENT_HPP_ */
