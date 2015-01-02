/*
 * CreepInfo.cpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#include "CreepInfo.hpp"
#include "Operations.hpp"

#include "picojson.h"

#include <sstream>
#include <fstream>

std::string creepColor(CreepColor color)
{
	std::string cor;
	switch (color) {
		case BLUE:
			cor = "blue";
			break;
		case GREEN:
			cor = "green";
			break;
		case RED:
			cor = "red";
			break;
		case YELLOW:
			cor = "yellow";
			break;
		default:
			break;
	}
	return cor;
};

CreepColor creepColor(std::string color)
{
	CreepColor cor = BLUE;
	if(color == "green")
		cor = GREEN;
	else if(color == "red")
		cor = RED;
	else if(color == "yellow")
		cor = YELLOW;
	return cor;
};

const std::vector<float> CreepInfo::level1(std::vector<float>{ 1.0f, 0.2f, 0.2f, 1.0f, 0.1f, 0.2f });
const std::vector<float> CreepInfo::level2(std::vector<float>{ 0.2f, 1.0f, 0.2f, 0.6f });
const std::vector<float> CreepInfo::level3(std::vector<float>{ 1.0f, 0.2f, 0.2f, 0.2f });


CreepInfo::CreepInfo() :
		color_(CreepColor::BLUE),
		level_(CreepLevel::ONE),
		animationFrames_(level1)
{
}

CreepInfo::CreepInfo(CreepColor color, CreepLevel level) :
		color_(color),
		level_(level),
		animationFrames_(level_ == ONE ? level1 : level_ == TWO ? level2 : level3)
{
	std::stringstream ss;
	ss << "creep-" << level_ << "-" << creepColor(color);

	image_ = "resource/creep/" + ss.str() + "/sheet.png";


//	std::ifstream ifs("resource/creeps.json");
//	picojson::value v;
//	ifs >> v;

	std::string fileStr = readFile("resource/creeps.json");
	picojson::value v;
	std::string err;
	picojson::parse(v, fileStr.begin(), fileStr.end(), &err);

	picojson::object& obj = v.get<picojson::object>();
	auto& creep = obj[ss.str()].get<picojson::object>();
	auto& heal_aura = creep["heal_aura"];
	auto& speed_aura = creep["speed_aura"];
	auto& stunResistance = creep["stunResistance"];
	auto& bleedResistance = creep["bleedResistance"];
	auto& slowResistance = creep["slowResistance"];

	health_ = creep["health"].get<double>();
	speed_ = creep["speed"].get<double>();
	gold_ = creep["gold"].get<double>();

	if(!heal_aura.is<picojson::null>())
	{
		auto& healObj = heal_aura.get<picojson::object>();
		healAura_ = HealAura { (int)healObj["area"].get<double>(), (int)healObj["hps"].get<double>() };
	}
	else
		healAura_ = HealAura {0, 0};

	if(!speed_aura.is<picojson::null>())
	{
		auto& speedObj = speed_aura.get<picojson::object>();
		speedAura_ = SpeedAura { (int)speedObj["area"].get<double>(), speedObj["percentage"].get<double>() };
	}
	else
		speedAura_ = SpeedAura { 0, 0 };

	if(!stunResistance.is<picojson::null>())
		stunResistance_ = stunResistance.get<double>();
	else
		stunResistance_ = 0;

	if(!bleedResistance.is<picojson::null>())
		bleedResistance_ = bleedResistance.get<double>();
	else
		bleedResistance_ = 0;

	if(!slowResistance.is<picojson::null>())
		slowResistance_ = slowResistance.get<double>();
	else
		slowResistance_ = 0;

}

CreepInfo::~CreepInfo()
{
}

bool CreepInfo::hasSpeedAura() const
{
	return speedAura_.area_ > 0;
}

bool CreepInfo::hasHealAura() const
{
	return healAura_.area_ > 0;
}

bool CreepInfo::hasStunResistance() const
{
	return stunResistance_ != 0;
}

bool CreepInfo::hasSlowResistance() const
{
	return slowResistance_ != 0;
}

bool CreepInfo::hasBleedResistance() const
{
	return bleedResistance_ != 0;
}
