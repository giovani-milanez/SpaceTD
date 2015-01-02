/*
 * BulletSystem.cpp
 *
 *  Created on: 07/10/2014
 *      Author: giovani
 */

#include "systems/BulletSystem.hpp"

#include "Operations.hpp"
#include "Game.hpp"

#include "components/Bullet.hpp"
#include "components/Position.hpp"
#include "components/Draw.hpp"
#include "components/Target.hpp"
#include "components/Damage.hpp"
#include "components/Health.hpp"
#include "components/Modifier.hpp"
#include "components/Creep.hpp"

#include "events/DestroyEvent.hpp"

BulletSystem::BulletSystem() :
	game_(Game::getInstance())
{
}

BulletSystem::~BulletSystem()
{
}

void BulletSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
	processBleedEffect(es, events, dt);

	for (auto bullet : es.entities_with_components<Bullet, Target>())
	{
		auto target = bullet.component<Target>()->target_;
		if(collidesWith(bullet, target))
		{
			hit(es, events, target, bullet);
			bullet.destroy();
		}
	}
}

void BulletSystem::hit(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet)
{
	auto targetHealth = target.component<Health>();
	auto bulletDamage = bullet.component<Damage>();

	std::uniform_int_distribution<uint32_t> dist(bulletDamage->from_, bulletDamage->to_);
	auto dmg = dist(game_.rng_);
	targetHealth->damage(dmg + bulletDamage->bonus_);

	if(targetHealth->isDead())
	{
		target.component<Creep>()->onDead(es, events);
	}
	else
	{
		applyBulletEffects(es, events, target, bullet);
	}
}

void BulletSystem::applyBulletEffects(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet)
{
	applyStun(es, events, target, bullet);
	applySlow(es, events, target, bullet);
	applyBleed(es, events, target, bullet);
	applySplash(es, events, target, bullet);
}

void BulletSystem::applyStun(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet)
{
	if(bullet.has_component<Stun>())
	{
		auto stun = bullet.component<Stun>();
		std::bernoulli_distribution d(stun->chance_);
		if(d(game_.rng_))
		{
			if (target.has_component<Stun>())
				target.remove<Stun>();

			target.assign<Stun>(*stun.get());
		}
	}
}

void BulletSystem::applySlow(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet)
{
	if(bullet.has_component<Slow>())
	{
		auto slow = bullet.component<Slow>();
		if (target.has_component<Slow>())
			target.remove<Slow>();

		target.assign<Slow>(*slow.get());
	}
}

void BulletSystem::applyBleed(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet)
{
	if(bullet.has_component<Bleed>())
	{
		if (target.has_component<Bleed>())
			target.remove<Bleed>();

		auto bleed = bullet.component<Bleed>();
		target.assign<Bleed>(*bleed.get());
	}
}

void BulletSystem::applySplash(entityx::EntityManager &es, entityx::EventManager& events, entityx::Entity& target, entityx::Entity& bullet)
{
	if(bullet.has_component<Splash>())
	{
		auto bulletPos = bullet.component<Position>();
		auto bulletDraw = bullet.component<Draw>();
		auto splash = bullet.component<Splash>();

		Circle rangeCircle { (int)bulletPos->x_ + bulletDraw->sprite_.width_ / 2, (int)bulletPos->y_ + bulletDraw->sprite_.height_ / 2, splash->range_  };
		bullet.remove<Splash>();

		for(auto creep : es.entities_with_components<Creep>())
		{
			if(creep != target)
			{
				SDL_Rect creepRect = getRect(creep);
				if(collidesWith(rangeCircle, creepRect))
				{
					hit(es, events, creep, bullet);
				}
			}
		}
	}
}

void BulletSystem::processBleedEffect(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
	for(auto creep : es.entities_with_components<Bleed, Creep, Health>())
	{
		auto creepE = creep.component<Creep>();
		auto bleed = creep.component<Bleed>();
		auto health = creep.component<Health>();
		if(!bleed->lifeTime_.done())
		{
			bleed->lifeTime_.update(dt);
			health->damage(bleed->dps_ * dt * (1 - creepE->info_.bleedResistance_));
			if(health->isDead())
			{
				creepE->onDead(es, events);
			}
		}
		else
		{
			creep.remove<Bleed>();
		}
	}
}
