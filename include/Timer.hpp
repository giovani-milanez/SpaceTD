/*
 * Timer.hpp
 *
 *  Created on: 16/10/2014
 *      Author: giovani
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_


struct Timer
{
	Timer(double lifeTimeSeconds) : lifeTime_(lifeTimeSeconds), runTime_(0) {}

	void update(const float dt)
	{
		runTime_ += dt;
		if(runTime_ > lifeTime_)
			runTime_ = lifeTime_;
	}
	bool done() const
	{
		return runTime_ == lifeTime_;
	}
	double progress() const
	{
		return runTime_ / lifeTime_;
	}
	void reset(double lifeTimeSeconds = 0)
	{
		if(lifeTimeSeconds != 0)
			lifeTime_ = lifeTimeSeconds;

		runTime_ = 0;
	}
private:
	double lifeTime_;
	double runTime_;
};


#endif /* TIMER_HPP_ */
