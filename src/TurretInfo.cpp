/*
 * TurretInfo.cpp
 *
 *  Created on: 10/10/2014
 *      Author: giovani
 */

#include "TurretInfo.hpp"
#include "Operations.hpp"

#include "picojson.h"

#include <sstream>
#include <fstream>

TurretInfo::TurretInfo(TurretType type)
{
	level_ = TurretLevel::L1;
	type_ = type;

	std::stringstream ss;
	ss << "turret-" << type;

//	std::ifstream ifs("resource/turrets.json");
//	picojson::value v;
//	ifs >> v;

	std::string fileStr = readFile("resource/turrets.json");
	picojson::value v;
	std::string err;
	picojson::parse(v, fileStr.begin(), fileStr.end(), &err);

	picojson::object& obj = v.get<picojson::object>();
	auto turret = obj[ss.str()].get<picojson::object>();
	auto damage = turret["damage"].get<picojson::object>();
	auto splash = turret["splash_range"];
	auto stun = turret["stun"];
	auto slow = turret["slow"];
	auto bleed = turret["bleed"];

	name_ = turret["name"].get<std::string>();
	image_ = turret["image"].get<std::string>();
	bullet_image_ = turret["bullet_image"].get<std::string>();
	damge_from_ = damage["from"].get<double>();
	damge_to_ = damage["to"].get<double>();
	cooldown_ = turret["cooldown"].get<double>();
	range_ = turret["range"].get<double>();
	cost_ = turret["cost"].get<double>();

	if(!splash.is<picojson::null>())
		splash_range_ = splash.get<double>();
	else
		splash_range_ = 0;

	if(!stun.is<picojson::null>())
	{
		auto stunObj = stun.get<picojson::object>();
		stun_duration_ = stunObj["duration"].get<double>();
		stun_chance_ = stunObj["chance"].get<double>();
	}
	else
	{
		stun_duration_ = 0;
		stun_chance_ = 0;
	}
	if(!slow.is<picojson::null>())
	{
		auto slowObj = slow.get<picojson::object>();
		slow_duration_ = slowObj["duration"].get<double>();
		slow_percentage_ = slowObj["percentage"].get<double>();
	}
	else
	{
		slow_duration_ = 0;
		slow_percentage_ = 0;
	}
	if(!bleed.is<picojson::null>())
	{
		auto bleedObj = bleed.get<picojson::object>();
		bleed_duration_ = bleedObj["duration"].get<double>();
		bleed_dps_ = bleedObj["dps"].get<double>();
	}
	else
	{
		bleed_duration_ = 0;
		bleed_dps_ = 0;
	}
}

void TurretInfo::levelUp()
{
	if (level_ < L3)
	{
		int nextLevel = (int)level_ + 1;
		level_ = (TurretLevel)nextLevel;

		damge_from_ *= 1.2;
		damge_to_ *= 1.2;
		cooldown_ = cooldown_ > 1.0 ? cooldown_ - 0.5 : cooldown_;
		range_ += 5;
		
		std::stringstream ss;
		ss << type_ << "-" << level_;
		image_ = "resource/tower-defense-turrets/turret-"+ss.str()+".png";
	}
}
