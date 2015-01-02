/*
 * AuraSystem.hpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#ifndef AURASYSTEM_HPP_
#define AURASYSTEM_HPP_

#include <entityx/entityx.h>

struct AuraSystem : public entityx::System<AuraSystem>
{
	void update(entityx::EntityManager &entityManager, entityx::EventManager &eventManager, double deltaTimeSeconds) override;
private:
	void healCreepsInAuraRange(entityx::EntityManager &entityManager, entityx::Entity creepWithHealAura, double deltaTimeSeconds);
	void speedCreepsInAuraRange(entityx::EntityManager &entityManager, entityx::Entity creepWithSpeedAura);
	bool inSpeedAura(entityx::EntityManager &entityManager, entityx::Entity creepToCheck);
};

#endif /* AURASYSTEM_HPP_ */
