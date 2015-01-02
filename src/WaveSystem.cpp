/*
 * WaveSystem.cpp
 *
 *  Created on: 16/10/2014
 *      Author: giovani
 */

#include "systems/WaveSystem.hpp"
#include "events/WaveEvent.hpp"
#include "CreepInfo.hpp"
#include "Operations.hpp"

#include "picojson.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

Waves loadWave(const std::string& waveFile)
{
	Waves waves;

//	std::ifstream ifs(waveFile);
//	picojson::value v;
//	ifs >> v;

	std::string fileStr = readFile(waveFile);
	picojson::value v;
	std::string err;
	picojson::parse(v, fileStr.begin(), fileStr.end(), &err);

	picojson::object& obj = v.get<picojson::object>();
	auto wavesArray = obj["waves"].get<picojson::array>();

	for (picojson::array::const_iterator i = wavesArray.begin(); i != wavesArray.end(); ++i)
	{

		Wave w;
		auto waveObj = (*i).get<picojson::object>();
		auto creepsArray = waveObj["creeps"].get<picojson::array>();

		for (picojson::array::const_iterator creeps = creepsArray.begin(); creeps != creepsArray.end(); ++creeps)
		{
			auto creepObj = (*creeps).get<picojson::object>();

			w.push_back(
					WaveCreep( creepObj["name"].get<std::string>(), creepObj["quantity"].get<double>(), creepObj["delay"].get<double>() )
					);
		}
		waves.push_back(w);
	}

	return waves;
}

WaveSystem::WaveSystem(entityx::EventManager &events) :
		events_(events),
		waves_(loadWave("resource/waves.json")),
		timer_(waves_[0][0].getDelay()),
		startNextWave_(true),
		signalBegin_(true),
		destroyedCreepsAmount_(0),
		currentWave_(0),
		wavesCreepsAmount_(),
		totalWaves_(waves_.size())
{

	for(auto& wave : waves_)
	{
		int spawnedCreepsAmount = 0;
		for(auto& spawn : wave)
			spawnedCreepsAmount += spawn.getQuantity();

		wavesCreepsAmount_.push_back(spawnedCreepsAmount);
	}

}

void WaveSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
	if(waves_.size() == 0)
		return;

	auto currentWave = waves_.begin();
	auto currentCreep = currentWave->begin();

	if(startNextWave_ && timer_.done())
	{
		if(signalBegin_)
		{
			events.emit<WaveBegin>();
			signalBegin_ = false;
		}
		// spawn creep
		auto name = currentCreep->getName();

		std::size_t posLevel = name.find("-");
		auto level = name.substr(posLevel+1, 1);

		std::size_t posColor = name.rfind("-");
		auto color = name.substr(posColor+1);

		events.emit<WaveSpawn>(CreepInfo(creepColor(color), (CreepLevel)atoi(level.c_str())), currentCreep->getQuantity());

		currentWave->erase(currentCreep);
		if(currentWave->size() == 0 ) // All the creeps from the current wave have been spawn
		{
			waves_.erase(currentWave);
			signalBegin_ = true;
			startNextWave_ = false; // wait untill all the creeps to be killed
		}
		else // There is still creeps to be spawn from the current wave
		{
			auto nextCreep = currentWave->begin();
			timer_.reset(nextCreep->getDelay());
		}
	}
	timer_.update(dt);
}

void WaveSystem::receive(const DestroyEvent &evt)
{
	incrementCreepDown();
}

void WaveSystem::receive(const AutoMoveEndEvent &evt)
{
	incrementCreepDown();
}

void WaveSystem::incrementCreepDown()
{
	destroyedCreepsAmount_++;

	if(destroyedCreepsAmount_ == wavesCreepsAmount_[currentWave_])
	{
		if(currentWave_ == totalWaves_ - 1)
		{
			events_.emit<WaveDone>();
		}
		else
		{
			timer_.reset(5);
			startNextWave_ = true;
			events_.emit<WaveEnd>();
			currentWave_++;
			destroyedCreepsAmount_ = 0;
		}
	}
}
