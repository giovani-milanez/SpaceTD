/*
 * CombatSystem.cpp
 *
 *  Created on: 02/10/2014
 *      Author: giovani
 */

#include "systems/CombatSystem.hpp"
#include "PathFinding.hpp"
#include "Operations.hpp"

#include "components/Turret.hpp"
#include "components/Draw.hpp"
#include "components/Position.hpp"
#include "components/AutoMove.hpp"
#include "components/Velocity.hpp"
#include "components/Creep.hpp"
#include "components/Target.hpp"
#include "components/Animate.hpp"
#include "components/Cooldown.hpp"
#include "components/Range.hpp"
#include "components/Bullet.hpp"

#include "events/DestroyEvent.hpp"

#include <iostream>
#include <math.h>

void CombatSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
	for (auto turret : es.entities_with_components<Turret, Draw, Position>())
	{
		if(turret.has_component<Target>())
		{
			auto targetEntity = turret.component<Target>()->target_;
			if(inRange(turret, targetEntity))
			{
				rotateToTarget(turret, targetEntity, dt);
				attack(turret, targetEntity, es);
			}
			else
			{
				turret.remove<Target>();
				rotateToAngle(turret, 0, dt);
			}
		}
		else
		{
			auto closestCreep = getClosestCreep(turret, es);
			if(closestCreep.id() != entityx::Entity::INVALID)
			{
				if(inRange(turret, closestCreep))
				{
					turret.assign<Target>(closestCreep);
					rotateToTarget(turret, closestCreep, dt);
					attack(turret, closestCreep, es);
				}
				else
				{
					rotateToAngle(turret, 0, dt);
				}
			}
			else
			{
				rotateToAngle(turret, 0, dt);
			}
		}
		turret.component<Cooldown>()->update(dt);
	}

}

void CombatSystem::attack(entityx::Entity& turret, entityx::Entity& target, entityx::EntityManager &es)
{
	auto turretComponent = turret.component<Turret>();
	auto cd = turret.component<Cooldown>();
	if(turretComponent->ready_ && cd->good())
	{
		turretComponent->fire(target, es);
		cd->restart();
	}
}

entityx::Entity CombatSystem::getClosestCreep(entityx::Entity& turret, entityx::EntityManager &es)
{
	auto posTurret = turret.component<Position>();
	Position& posTurretRef = *posTurret.get();

	auto creeps = es.entities_with_components<Creep, Velocity, Position>();
	auto closestCreep = std::min_element(creeps.begin(), creeps.end(), [&] (const entityx::Entity & p1, const entityx::Entity & p2)
			  -> bool
			 {
				auto distance = [](const Position& p1, const Position& p2)
				{
					float x = p1.x_ - p2.x_;
					float y = p1.y_ - p2.y_;
					double dist = pow(x,2)+pow(y,2);

					return sqrt(dist);
				};

				auto pos1 = p1.component<Position>();
				auto pos2 = p2.component<Position>();

				return distance(*pos1.get(), posTurretRef) < distance(*pos2.get(), posTurretRef);
			 });
	if(closestCreep == creeps.end())
		return entityx::Entity();

	return *closestCreep;
}


bool CombatSystem::inRange(entityx::Entity& turret, entityx::Entity& target)
{
	auto turretPos = turret.component<Position>();
	auto range = turret.component<Range>();
	Circle rangeCircle { (int)turretPos->x_ + 20, (int)turretPos->y_ + 20, range->range_ + range->bonus_ };
	SDL_Rect targetRect = getRect(target);

	return collidesWith(rangeCircle, targetRect);
}


void CombatSystem::receive(const DestroyEvent& evt)
{
	for (auto turret : evt.es_.entities_with_components<Turret>())
		if(turret.has_component<Target>() && turret.component<Target>()->target_ == evt.entity_)
			turret.remove<Target>();

	for (auto bullet : evt.es_.entities_with_components<Bullet>())
		if(bullet.has_component<Target>() && bullet.component<Target>()->target_ == evt.entity_)
			bullet.remove<Target>();
}
