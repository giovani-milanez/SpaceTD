/*
 * Animate.hpp
 *
 *  Created on: 01/10/2014
 *      Author: giovani
 */

#ifndef ANIMATE_HPP_
#define ANIMATE_HPP_

#include "Animation.hpp"

#include <memory>

struct Animate : entityx::Component<Animate>
{
	Animate(Animation&& sprite) : sprite_(std::move(sprite)), enabled_(true) {};
	Animation sprite_;
	bool enabled_;
};

#endif /* ANIMATE_HPP_ */
