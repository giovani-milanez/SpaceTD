/*
 * Turret.cpp
 *
 *  Created on: 07/10/2014
 *      Author: giovani
 */


#include "components/Turret.hpp"

#include "ResourceManager.hpp"

#include "components/Draw.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Target.hpp"
#include "components/Bullet.hpp"
#include "components/Damage.hpp"
#include "components/Modifier.hpp"
#include "components/Cooldown.hpp"
#include "components/Range.hpp"

#include <SDL2/SDL_mixer.h>

Turret::Turret(entityx::Entity entity, const TurretInfo& info) :
	entity_(entity),
	ready_(true),
	info_(info)
{
	reloadComponents();
}

Turret::~Turret()
{
}

void Turret::fire(entityx::Entity target, entityx::EntityManager& es)
{
	auto turretPos = entity_.component<Position>();

	auto bullet = es.create();
	bullet.assign<Position>(turretPos->x_, turretPos->y_);
	bullet.assign<Bullet>();
	bullet.assign<Target>(target);
	bullet.assign<Velocity>(0, 0);
	bullet.component<Velocity>()->speedBonus_ = bullet.component<Velocity>()->speedBase() * 10;
	bullet.assign<Damage>(*entity_.component<Damage>().get());

	bullet.assign<Draw>(Texture(info_.bullet_image_));
	if(info_.splash_range_ != 0)
		bullet.assign<Splash>(info_.splash_range_);
	if(info_.stun_duration_ != 0)
		bullet.assign<Stun>(LifeTime(info_.stun_duration_), info_.stun_chance_);
	if(info_.slow_duration_ != 0)
		bullet.assign<Slow>(LifeTime(info_.slow_duration_), info_.slow_percentage_);
	if(info_.bleed_duration_ != 0)
		bullet.assign<Bleed>(LifeTime(info_.bleed_duration_), info_.bleed_dps_);

	try
	{
		Mix_Chunk* sound = ResourceManager::getInstance().getSound("resource/laser4.wav");
		Mix_PlayChannel( -1, sound, 0 );
	}catch(const std::exception& ex)
	{
		SDL_Log("Erro som: %s", ex.what());
	}
}

void Turret::levelUp()
{
	info_.levelUp();
	reloadComponents();
}


void Turret::reloadComponents()
{
	double angle = 0;
	if (entity_.has_component<Draw>())
	{
		angle = entity_.component<Draw>()->angle_;
		entity_.remove<Draw>();
	}
	entity_.assign<Draw>(Texture(info_.image_));
	entity_.component<Draw>()->angle_ = angle;

	if(entity_.has_component<Damage>())
		entity_.remove<Damage>();
	entity_.assign<Damage>(info_.damge_from_, info_.damge_to_);

	if(entity_.has_component<Range>())
		entity_.remove<Range>();
	entity_.assign<Range>(info_.range_);

	if(entity_.has_component<Cooldown>())
		entity_.remove<Cooldown>();
	entity_.assign<Cooldown>(info_.cooldown_);
}

int Turret::damageLevel() const
{
	return damageLevel(info_.damge_from_, info_.damge_to_);
}

int Turret::cooldownLevel() const
{
	return cooldownLevel(info_.cooldown_);
}

int Turret::rangeLevel() const
{
	return rangeLevel(info_.range_);
}

int Turret::nextDamageLevel() const
{
	return damageLevel(info_.damge_from_ * 1.2, info_.damge_to_ * 1.2);
}

int Turret::nextCooldownLevel() const
{
	return cooldownLevel(info_.cooldown_ > 1.0 ? info_.cooldown_ - 0.5 : info_.cooldown_);
}

int Turret::nextRangeLevel() const
{
	return rangeLevel(info_.range_ + 5);
}

int Turret::upgradeCost() const
{
	return info_.level_ == TurretLevel::L1 ? info_.cost_ * 1.3 :
			info_.level_ == TurretLevel::L2 ? info_.cost_ * 1.55 :
					info_.cost_ * 2;
}

int Turret::damageLevel(int from, int to) const
{
	double mean = (from + to) / 2;
	if(mean >= 22)
		return 4;
	else if(mean >= 17 && mean < 22)
		return 3;
	else if(mean >= 14 && mean < 17)
		return 2;
	else
		return 1;
}

int Turret::cooldownLevel(double cd) const
{
	if(cd <= 1)
		return 4;
	else if(cd > 1 && cd <= 1.5)
		return 3;
	else if(cd > 1.5 && cd <= 2)
		return 2;
	else
		return 1;
}

int Turret::rangeLevel(int range) const
{
	if(range >= 60)
		return 4;
	else if(range < 60 && range >= 55)
		return 3;
	else if(range < 55 && range >= 50)
		return 2;
	else
		return 1;
}
