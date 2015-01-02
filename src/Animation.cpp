/*
 * Animation.cpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#include "Animation.hpp"

#include <algorithm>
#include <numeric>

Animation::Animation(Texture&& texture, int frameWidth, int frameHigh, std::vector<float> frameDuration) :
	spriteSheet_(std::move(texture)),
	frameDuration_(frameDuration),
	frameWidth_(frameWidth),
	frameHigh_(frameHigh),
	totalDuration_(std::accumulate(frameDuration_.begin(), frameDuration_.end(), 0)),
	runTime_(0),
	currentFrame_(0)
{

}

Animation::~Animation()
{
}

Animation::Animation(Animation&& src) :
	spriteSheet_(std::move(src.spriteSheet_)),
	frameDuration_(src.frameDuration_),
	frameWidth_(src.frameWidth_),
	frameHigh_(src.frameHigh_),
	totalDuration_(src.totalDuration_),
	runTime_(src.runTime_),
	currentFrame_(src.currentFrame_)
{
}

Animation& Animation::operator=(Animation&& src)
{
	spriteSheet_ = std::move(src.spriteSheet_);
	frameDuration_ = src.frameDuration_;
	frameWidth_ = src.frameWidth_;
	frameHigh_ = src.frameHigh_;
	totalDuration_ = src.totalDuration_;
	runTime_ = src.runTime_;
	currentFrame_ = src.currentFrame_;

	return *this;
}

void Animation::update(const float dt)
{
	runTime_ += dt;
	if(runTime_ > frameDuration_[currentFrame_])
	{
		runTime_ = 0;
		currentFrame_++;
		if(currentFrame_ >= frameDuration_.size())
			currentFrame_ = 0;

	}
}

void Animation::restart()
{
	runTime_ = 0;
}

bool Animation::isFinished() const
{
	return runTime_ > totalDuration_;
}

SDL_Rect Animation::getRect() const
{
	return SDL_Rect{(int)currentFrame_ * frameWidth_, 0, frameWidth_, frameHigh_};
}
