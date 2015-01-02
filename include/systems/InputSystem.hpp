/*
 * InputSystem.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef INPUTSYSTEM_HPP_
#define INPUTSYSTEM_HPP_

#include "entityx/entityx.h"

struct InputSystem : public entityx::System<InputSystem>
{
	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};

#endif /* INPUTSYSTEM_HPP_ */
