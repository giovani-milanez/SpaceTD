/*
 * WaveSystem.hpp
 *
 *  Created on: 16/10/2014
 *      Author: giovani
 */

#ifndef WAVESYSTEM_HPP_
#define WAVESYSTEM_HPP_

#include "Timer.hpp"

#include "events/DestroyEvent.hpp"
#include "events/AutoMoveEvent.hpp"

#include <entityx/entityx.h>

#include <tuple>

struct WaveCreep : public std::tuple<std::string, double, double>
{

	//using std::tuple<std::string, double, double>::tuple;
	WaveCreep(std::string& name, double& qtd, double& delay) : std::tuple<std::string, double, double>(name, qtd, delay) {}

	std::string getName() const
	{
		return std::get<0>(*this);
	}

	int getQuantity() const
	{
		return std::get<1>(*this);
	}

	int getDelay() const
	{
		return std::get<2>(*this);
	}
};

typedef std::vector<WaveCreep> Wave;
typedef std::vector<Wave> Waves;

Waves loadWave(const std::string& waveFile);

struct WaveSystem : public entityx::System<WaveSystem>, public entityx::Receiver<DestroyEvent>
{
	WaveSystem(entityx::EventManager &events);
	void configure(entityx::EventManager &event_manager) 
	{
		event_manager.subscribe<DestroyEvent>(*this);
		event_manager.subscribe<AutoMoveEndEvent>(*this);
	}
	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
	
	void receive(const DestroyEvent &evt);
	void receive(const AutoMoveEndEvent &evt);
	void incrementCreepDown();

	entityx::EventManager &events_;
	Waves waves_;

	Timer timer_;
	bool startNextWave_, signalBegin_;
	int destroyedCreepsAmount_;
	int currentWave_;
	std::vector<int> wavesCreepsAmount_;
	int totalWaves_;
};

#endif /* WAVESYSTEM_HPP_ */
