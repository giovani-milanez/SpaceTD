/*
 * MovementSystem.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef MOVEMENTSYSTEM_HPP_
#define MOVEMENTSYSTEM_HPP_

#include <entityx/entityx.h>

struct MovementSystem : public entityx::System<MovementSystem>
{
	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
	void follow(entityx::Entity entity, int targetX, int targetY);
};

#endif /* MOVEMENTSYSTEM_HPP_ */
