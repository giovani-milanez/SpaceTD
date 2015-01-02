/*
 * ViewPort.hpp
 *
 *  Created on: 10/10/2014
 *      Author: giovani
 */

#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

struct ViewPort : entityx::Component<ViewPort>
{
	ViewPort(const SDL_Rect& viewPort) : viewPort_(viewPort) {};
	SDL_Rect viewPort_;
};


#endif /* VIEWPORT_HPP_ */
