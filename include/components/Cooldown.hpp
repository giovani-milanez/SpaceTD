/*
 * Cooldown.hpp
 *
 *  Created on: 08/10/2014
 *      Author: giovani
 */

#ifndef COOLDOWN_HPP_
#define COOLDOWN_HPP_

#include <entityx/entityx.h>

struct Cooldown : entityx::Component<Cooldown>
{
	Cooldown(float duration) : cd_(0), duration_(duration), runTime_(0) {}

	void update(const float dt)
	{
		runTime_ += dt;
		if(runTime_ >= duration_)
		{
			cd_ = 0.f;
			runTime_ = duration_;
		}
		else
		{
			cd_ += dt;
		}
	}

	void restart()
	{
		runTime_ = 0.f;
		cd_ = duration_;
	}

	bool good() const
	{
		return cd_ == 0.f;
	}

	void setDuration(float duration)
	{
		duration_ = duration;
		restart();
	}

	float cd_;
private:
	float duration_;
	float runTime_;
};


#endif /* COOLDOWN_HPP_ */
