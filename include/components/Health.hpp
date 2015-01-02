/*
 * Health.hpp
 *
 *  Created on: 07/10/2014
 *      Author: giovani
 */

#ifndef HEALTH_HPP_
#define HEALTH_HPP_

#include "entityx/entityx.h"

struct Health : entityx::Component<Health>
{
	Health(double maxHealth) : maxHealth_(maxHealth), currentHealth_(maxHealth) {}

	void damage(double damage)
	{
		currentHealth_ -= damage;
		if(currentHealth_ < 0)
			currentHealth_ = 0;
	}
	void heal(double heal)
	{
		currentHealth_ += heal;
		if(currentHealth_ > maxHealth_)
			currentHealth_ = maxHealth_;
	}
	double isDead() const
	{
		return currentHealth_ == 0;
	}
	double currentHealth() const
	{
		return currentHealth_;
	}
	double maxHealth() const
	{
		return maxHealth_;
	}
private:
	double maxHealth_;
	double currentHealth_;
};


#endif /* HEALTH_HPP_ */
