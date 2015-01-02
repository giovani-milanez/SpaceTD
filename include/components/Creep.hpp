/*
 * Creep.hpp
 *
 *  Created on: 02/10/2014
 *      Author: giovani
 */

#ifndef CREEP_HPP_
#define CREEP_HPP_

#include "CreepInfo.hpp"
#include "Circle.hpp"

#include <entityx/entityx.h>

struct Creep : entityx::Component<Creep>
{
	Creep(entityx::Entity entity, const CreepInfo& info);

	entityx::Entity entity_;
	CreepInfo info_;
	void onDead(entityx::EntityManager& es, entityx::EventManager& events);
	Circle getSpeedAuraRange() const;
	Circle getHealAuraRange() const;
private:
	void reloadComponents();
};


#endif /* CREEP_HPP_ */
