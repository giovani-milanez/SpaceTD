/*
 * AnimationSystem.cpp
 *
 *  Created on: 01/10/2014
 *      Author: giovani
 */

#include "systems/AnimationSystem.hpp"

#include "components/Animate.hpp"
#include "components/Modifier.hpp"


void AnimationSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
	for (auto entity : es.entities_with_components<Animate>())
	{
		auto animation = entity.component<Animate>();

		auto delta = dt;
		if(entity.has_component<Slow>())
		{
			// applies slow percentage on time elapsed
			delta *= 1.0 - entity.component<Slow>()->pecentage_;
		}

		if(animation->enabled_ && !entity.has_component<Stun>())
			animation->sprite_.update(delta);
	}
}
