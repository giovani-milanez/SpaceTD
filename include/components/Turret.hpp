/*
 * Turret.hpp
 *
 *  Created on: 02/10/2014
 *      Author: giovani
 */

#ifndef TURRET_HPP_
#define TURRET_HPP_

#include <entityx/entityx.h>

#include "TurretInfo.hpp"


struct Turret : entityx::Component<Turret>
{
	Turret(entityx::Entity entity, const TurretInfo& info);
	~Turret();

	entityx::Entity entity_;

	bool ready_;

	void fire(entityx::Entity target, entityx::EntityManager& es);
	void levelUp();

	int damageLevel() const;
	int cooldownLevel() const;
	int rangeLevel() const;

	int nextDamageLevel() const;
	int nextCooldownLevel() const;
	int nextRangeLevel() const;

	int upgradeCost() const;

	TurretInfo info_;
private:
	void reloadComponents();
	int damageLevel(int from, int to) const;
	int cooldownLevel(double cd) const;
	int rangeLevel(int range) const;
};

#endif /* TURRET_HPP_ */
