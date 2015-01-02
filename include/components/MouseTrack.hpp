/*
 * MouseTrack.hpp
 *
 *  Created on: 10/10/2014
 *      Author: giovani
 */

#ifndef MOUSETRACK_HPP_
#define MOUSETRACK_HPP_


#include <entityx/entityx.h>

struct MouseTrack : entityx::Component<MouseTrack>
{
	virtual ~MouseTrack(){};
	virtual void onClick() = 0;
	//virtual void onMotion() = 0;
};


#endif /* MOUSETRACK_HPP_ */
