/*
 * BulletSystem.hpp
 *
 *  Created on: 07/10/2014
 *      Author: giovani
 */

#ifndef BULLETSYSTEM_HPP_
#define BULLETSYSTEM_HPP_

#include <entityx/entityx.h>

class Game;

class BulletSystem : public entityx::System<BulletSystem>
{
public:
	BulletSystem();
	virtual ~BulletSystem();

	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
	void hit(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet);
	void applyBulletEffects(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet);
	void applyStun(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet);
	void applySlow(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet);
	void applyBleed(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet);
	void applySplash(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet);
	void processBleedEffect(entityx::EntityManager& es, entityx::EventManager& events, double dt);
	Game& game_;
};

#endif /* BULLETSYSTEM_HPP_ */
