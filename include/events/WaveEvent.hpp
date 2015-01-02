/*
 * WaveEvent.hpp
 *
 *  Created on: 16/10/2014
 *      Author: giovani
 */

#ifndef WAVEEVENT_HPP_
#define WAVEEVENT_HPP_

#include "entityx/entityx.h"

#include "CreepInfo.hpp"

struct WaveBegin : entityx::Event<WaveBegin>
{
	WaveBegin() {}
};

struct WaveEnd : entityx::Event<WaveEnd>
{
	WaveEnd() {}
};

struct WaveDone : entityx::Event<WaveDone>
{
	WaveDone() {}
};

struct WavePrepare : entityx::Event<WavePrepare>
{
	WavePrepare() {}
};

struct WaveSpawn : entityx::Event<WaveSpawn>
{
	WaveSpawn(const CreepInfo& ci, int quantity) : ci_(ci), quantity_(quantity) {}

	CreepInfo ci_;
	int quantity_;
};

#endif /* WAVEEVENT_HPP_ */
