/*
 * CombatSystem.hpp
 *
 *  Created on: 02/10/2014
 *      Author: giovani
 */

#ifndef COMBATSYSTEM_HPP_
#define COMBATSYSTEM_HPP_

#include "events/DestroyEvent.hpp"

#include <entityx/entityx.h>

struct CombatSystem : public entityx::System<CombatSystem>, public entityx::Receiver<DestroyEvent>
{
	void configure(entityx::EventManager &event_manager)
	{
		event_manager.subscribe<DestroyEvent>(*this);
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;

	void attack(entityx::Entity& turret, entityx::Entity& target, entityx::EntityManager &es);

	entityx::Entity getClosestCreep(entityx::Entity& turret, entityx::EntityManager &es);

	bool inRange(entityx::Entity& turret, entityx::Entity& target);

	void receive(const DestroyEvent& evt);
};

#endif /* COMBATSYSTEM_HPP_ */
