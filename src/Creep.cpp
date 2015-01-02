/*
 * Creep.cpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#include "components/Creep.hpp"

#include "ResourceManager.hpp"

#include "components/Draw.hpp"
#include "components/Animate.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Health.hpp"
#include "components/LifeTime.hpp"

#include "events/DestroyEvent.hpp"

#include <SDL2/SDL_mixer.h>


Creep::Creep(entityx::Entity entity, const CreepInfo& info) :
	entity_(entity),
	info_(info)
{
	reloadComponents();
}

void Creep::reloadComponents()
{
	double angle = 0;
	if (entity_.has_component<Draw>())
	{
		angle = entity_.component<Draw>()->angle_;
		entity_.remove<Draw>();
	}
	entity_.assign<Draw>(Texture(info_.image_));
	entity_.component<Draw>()->angle_ = angle;
	if(entity_.has_component<Animate>())
		entity_.remove<Animate>();

	entity_.assign<Animate>(Animation(Texture(info_.image_), 46, 46 , info_.animationFrames_));
	entity_.assign<Velocity>(0, 0, info_.speed_);
	entity_.assign<Health>(info_.health_);
}

void Creep::onDead(entityx::EntityManager& es, entityx::EventManager& events)
{
	auto posX = entity_.component<Position>()->x_ - entity_.component<Animate>()->sprite_.frameWidth();
	auto posY = entity_.component<Position>()->y_ - entity_.component<Animate>()->sprite_.frameHigh();

	events.emit<DestroyEvent>(entity_, es);
	entity_.destroy();

	auto explosion = es.create();
	explosion.assign<Position>(posX, posY);
	explosion.assign<Draw>(Texture("resource/explosion_spritesheet.png"));
	explosion.assign<Animate>(Animation(Texture("resource/explosion_spritesheet.png"), 128, 128,
			std::vector<float> (10, 0.1)));
	explosion.assign<LifeTime>(1, true);

	Mix_Chunk* sound = ResourceManager::getInstance().getSound("resource/explosion.wav");
	Mix_PlayChannel( -1, sound, 0 );
}

Circle Creep::getSpeedAuraRange() const
{
	auto position = entity_.component<Position>();
	return Circle { (int)position->x_ + 23, (int)position->y_ + 23, info_.speedAura_.area_ };
}

Circle Creep::getHealAuraRange() const
{
	auto position = entity_.component<Position>();
	return Circle { (int)position->x_ + 23, (int)position->y_ + 23, info_.healAura_.area_ };
}
