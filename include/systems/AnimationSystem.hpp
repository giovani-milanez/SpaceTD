/*
 * AnimationSystem.hpp
 *
 *  Created on: 01/10/2014
 *      Author: giovani
 */

#ifndef ANIMATIONSYSTEM_HPP_
#define ANIMATIONSYSTEM_HPP_

#include <entityx/entityx.h>

class AnimationSystem : public entityx::System<AnimationSystem>
{
public:
	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};

#endif /* ANIMATIONSYSTEM_HPP_ */
