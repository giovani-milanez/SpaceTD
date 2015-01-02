/*
 * DestroyEvent.hpp
 *
 *  Created on: 07/10/2014
 *      Author: giovani
 */

#ifndef DESTROYEVENT_HPP_
#define DESTROYEVENT_HPP_


#include "entityx/entityx.h"

struct DestroyEvent : entityx::Event<DestroyEvent>
{
	DestroyEvent(entityx::Entity entity, entityx::EntityManager &es) : entity_(entity), es_(es){}

	entityx::Entity entity_;
	entityx::EntityManager &es_;
};


#endif /* DESTROYEVENT_HPP_ */
