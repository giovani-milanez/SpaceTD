/*
 * Target.hpp
 *
 *  Created on: 06/10/2014
 *      Author: giovani
 */

#ifndef TARGET_HPP_
#define TARGET_HPP_

#include <entityx/entityx.h>

struct Target : entityx::Component<Target>
{
	Target(entityx::Entity target) : target_(target)
	{
	}
	~Target()
	{
	}
	entityx::Entity target_;

};


#endif /* TARGET_HPP_ */
