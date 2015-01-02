/*
 * Range.hpp
 *
 *  Created on: 08/10/2014
 *      Author: giovani
 */

#ifndef RANGE_HPP_
#define RANGE_HPP_


#include <entityx/entityx.h>

struct Range : entityx::Component<Range>
{
	Range(int range) : range_(range), bonus_(0), draw_(false) {};
	int range_;
	int bonus_;

	bool draw_;
};


#endif /* RANGE_HPP_ */
