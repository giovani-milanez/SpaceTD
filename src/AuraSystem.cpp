/*
 * AuraSystem.cpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#include "systems/AuraSystem.hpp"

#include "Operations.hpp"

#include "components/Creep.hpp"
#include "components/Position.hpp"
#include "components/Health.hpp"
#include "components/Velocity.hpp"


void AuraSystem::update(entityx::EntityManager &entityManager, entityx::EventManager &eventManager, double deltaTimeSeconds)
{
	for (auto creepEntity : entityManager.entities_with_components<Creep, Position>())
	{
		auto creepComponent = creepEntity.component<Creep>();

		if(creepComponent->info_.hasHealAura())
		{
			healCreepsInAuraRange(entityManager, creepEntity, deltaTimeSeconds);
		}

		if(creepComponent->info_.hasSpeedAura())
		{
			speedCreepsInAuraRange(entityManager, creepEntity);
		}

		auto velocity = creepEntity.component<Velocity>();
		if(velocity->speedBonus_ > 0 && !inSpeedAura(entityManager, creepEntity))
		{
			velocity->speedBonus_ = 0;
		}
	}
}

void AuraSystem::healCreepsInAuraRange(entityx::EntityManager &entityManager, entityx::Entity creepWithHealAura, double deltaTimeSeconds)
{
	auto creepComponent = creepWithHealAura.component<Creep>();
	auto rangeCircle = creepComponent->getHealAuraRange();

	for(auto& creep : creepsInRange(entityManager, rangeCircle))
	{
		auto health = creep.component<Health>();
		health->heal(creepComponent->info_.healAura_.healPerSecond_ * deltaTimeSeconds);
	}
}

void AuraSystem::speedCreepsInAuraRange(entityx::EntityManager &entityManager, entityx::Entity creepWithSpeedAura)
{
	auto creepComponent = creepWithSpeedAura.component<Creep>();
	auto rangeCircle = creepComponent->getSpeedAuraRange();

	for(auto& creep : creepsInRange(entityManager, rangeCircle))
	{
		auto vel = creep.component<Velocity>();
		vel->speedBonus_ = vel->speedBase() * creepComponent->info_.speedAura_.percentage_;
	}
}


bool AuraSystem::inSpeedAura(entityx::EntityManager &entityManager, entityx::Entity creepToCheck)
{
	bool inAura = false;
	for (auto neighbor : entityManager.entities_with_components<Creep>())
	{
		auto creepNeighbor = neighbor.component<Creep>();
		if(creepNeighbor->info_.hasSpeedAura())
		{
			auto rangeCircle = creepNeighbor->getSpeedAuraRange();
			SDL_Rect creepRect = getRect(creepToCheck);
			if(collidesWith(rangeCircle, creepRect))
			{
				inAura = true;
				break;
			}
		}
	}

	return inAura;
}
