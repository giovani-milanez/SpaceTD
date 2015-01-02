/*
 * Animation.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include "Texture.hpp"

class Animation
{
public:
	Animation(Texture&& texture, int frameWidth, int frameHigh, std::vector<float> frameDuration);
	virtual ~Animation();

	Animation(Animation&& src);
	Animation& operator=(Animation&& src);

	void update(const float dt);
	void restart();
	bool isFinished() const;

	SDL_Rect getRect() const;
	const Texture& getSpriteSheet() const
	{
		return spriteSheet_;
	}

	Texture& getSpriteSheet()
	{
		return spriteSheet_;
	}

	int frameWidth() const
	{
		return frameWidth_;
	}

	int frameHigh() const
	{
		return frameHigh_;
	}

private:
	Texture spriteSheet_;
	std::vector<float> frameDuration_;
	int frameWidth_, frameHigh_;

	float totalDuration_, runTime_;
	unsigned int currentFrame_;
};

#endif /* ANIMATION_HPP_ */
