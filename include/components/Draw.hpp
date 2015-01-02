/*
 * Draw.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef DRAW_HPP_
#define DRAW_HPP_


#include "Texture.hpp"

#include <SDL2/SDL.h>

#include <memory>

struct Draw : entityx::Component<Draw>
{
	Draw(Texture&& sprite, double angle = 0) : sprite_(std::move(sprite)), angle_(angle), fillColor_({ 0, 255, 0, 0}), fadeOnLifeTime_(false)
	{
		crop_ = SDL_Rect{ 0, 0, 0, 0 };
	};
	Texture sprite_;
	double angle_;
	SDL_Rect crop_;
	SDL_Color fillColor_;
	bool fadeOnLifeTime_;
};


#endif /* DRAW_HPP_ */
