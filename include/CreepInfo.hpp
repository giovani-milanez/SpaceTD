/*
 * CreepInfo.hpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#ifndef CREEPINFO_HPP_
#define CREEPINFO_HPP_

#include <string>
#include <vector>

enum CreepColor
{
	BLUE = 1,
	GREEN = 2,
	RED = 3,
	YELLOW = 4
};

enum CreepLevel
{
	ONE = 1,
	TWO = 2,
	THREE = 3
};

struct HealAura
{
	int area_;
	int healPerSecond_;
};

struct SpeedAura
{
	int area_;
	double percentage_;
};

std::string creepColor(CreepColor color);
CreepColor creepColor(std::string color);

class CreepInfo
{
public:
	CreepInfo();
	CreepInfo(CreepColor color, CreepLevel level);
	virtual ~CreepInfo();

	bool hasSpeedAura() const;
	bool hasHealAura() const;
	bool hasStunResistance() const;
	bool hasSlowResistance() const;
	bool hasBleedResistance() const;

	CreepColor color_;
	CreepLevel level_;

	std::string image_;
	std::vector<float> animationFrames_;
	double health_;
	int speed_, gold_;
	double stunResistance_, slowResistance_, bleedResistance_;
	HealAura healAura_;
	SpeedAura speedAura_;

	static const std::vector<float> level1;
	static const std::vector<float> level2;
	static const std::vector<float> level3;
};

#endif /* CREEPINFO_HPP_ */
