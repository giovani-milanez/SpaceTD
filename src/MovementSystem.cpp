/*
 * MovementSystem.cpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#include "systems/MovementSystem.hpp"

#include "events/AutoMoveEvent.hpp"

#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/AutoMove.hpp"
#include "components/Controllable.hpp"
#include "components/Target.hpp"
#include "components/Modifier.hpp"
#include "components/Creep.hpp"

void MovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
	for (auto entity : es.entities_with_components<Position, Velocity>())
	{
		if(entity.has_component<Creep>() && entity.has_component<Stun>())
		{
			auto creepE = entity.component<Creep>();

			auto stun = entity.component<Stun>();
			if(stun->lifeTime_.done())
			{
				entity.remove<Stun>();
			}
			else
			{
				stun->lifeTime_.update(dt * (1 + creepE->info_.stunResistance_));
				continue;
			}
		}

		auto pos = entity.component<Position>();
		auto vel = entity.component<Velocity>();
		if (entity.has_component<AutoMove>())
		{
			entityx::ComponentHandle<AutoMove> autoMove = entity.component<AutoMove>();
			if (autoMove->paths_.empty())
			{
				std::cout << "Fim automove" << std::endl;
				events.emit<AutoMoveEndEvent>(entity);
				vel->x_ = 0;
				vel->y_ = 0;
				if(autoMove->destroyOnFinish_)
				{
					entity.destroy();
					continue;
				}
				entity.remove<AutoMove>();
			}
			else
			{
				SDL_Rect& nextRect = autoMove->paths_.back();
				//					std::cout << "atual " << position->position_.x_ << ", " << position->position_.y_ << std::endl;
				//					std::cout << "next " << nextRect.x << ", " << nextRect.y << std::endl;

				int difX = nextRect.x - pos->x_;
				int difY = nextRect.y - pos->y_;

				//					std::cout << "dif x: " << difX << std::endl;
				//					std::cout << "dif y: " << difY << std::endl;
				if (difX == 0 && difY == 0 && !autoMove->paths_.empty())
				{
					//						std::cout << "removendo" << std::endl;
					autoMove->paths_.pop_back();
				}
				else
				{
					follow(entity, nextRect.x, nextRect.y);
				}
			}
		}

		if (entity.has_component<Target>())
		{
			auto target = entity.component<Target>();
			auto pos = target->target_.component<Position>();
			follow(entity, pos->x_ + 23, pos->y_ + 23);
		}

		double slowRate = 1;
		if(entity.has_component<Creep>() && entity.has_component<Slow>())
		{
			auto creepE = entity.component<Creep>();
			auto slow = entity.component<Slow>();
			if(!slow->lifeTime_.done())
			{
				slowRate = (1.0 - slow->pecentage_);
				slowRate *= (1 - creepE->info_.slowResistance_);
				slow->lifeTime_.update(dt);
			}
			else
			{
				entity.remove<Slow>();
			}
		}
		pos->x_ += vel->x_ * static_cast<float>(dt) * slowRate;
		pos->y_ += vel->y_ * static_cast<float>(dt) * slowRate;
	}
}

void MovementSystem::follow(entityx::Entity entity, int targetX, int targetY)
{
	auto pos = entity.component<Position>();
	auto vel = entity.component<Velocity>();

	int difX = targetX - pos->x_;
	int difY = targetY - pos->y_;

	if (difX > 0)
		vel->x_ = 0.5f * vel->speed();
	else if (difX < 0)
		vel->x_ = -0.5f * vel->speed();
	else
		vel->x_ = 0;

	if (difY > 0)
		vel->y_ = 0.5f * vel->speed();
	else if (difY < 0)
		vel->y_ = -0.5f * vel->speed();
	else
		vel->y_ = 0;
}



