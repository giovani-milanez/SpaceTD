/*
 * TurretInfo.hpp
 *
 *  Created on: 10/10/2014
 *      Author: giovani
 */

#ifndef TURRETINFO_HPP_
#define TURRETINFO_HPP_

#include <string>

enum TurretType
{
	T1 = 1,
	T2 = 2,
	T3 = 3,
	T4 = 4,
	T5 = 5,
	T6 = 6,
	T7 = 7
};

enum TurretLevel
{
	L1 = 1,
	L2 = 2,
	L3 = 3
};

struct TurretInfo
{
	TurretInfo(TurretType type);

	void levelUp();
	TurretType type_;
	TurretLevel level_;
	std::string name_, image_, bullet_image_;
	double damge_from_, damge_to_, cooldown_, range_;
	double splash_range_, stun_duration_, stun_chance_, slow_duration_, slow_percentage_, bleed_duration_, bleed_dps_;
	int cost_;
};


#endif /* TURRETINFO_HPP_ */
