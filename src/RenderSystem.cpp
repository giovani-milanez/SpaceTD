/*
 * RenderSystem.cpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#include "systems/RenderSystem.hpp"

#include "Game.hpp"
#include "ResourceManager.hpp"

#include "components/Draw.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Turret.hpp"
#include "components/Animate.hpp"
#include "components/Health.hpp"
#include "components/Range.hpp"
#include "components/Modifier.hpp"
#include "components/ViewPort.hpp"
#include "components/Creep.hpp"
#include "components/Box.hpp"
#include "components/LifeTime.hpp"

#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL_ttf.h"

#include <sstream>

RenderSystem::RenderSystem() :
	game_(Game::getInstance()),
	rm_(ResourceManager::getInstance()),
	font_(rm_.getFont("resource/space-age.ttf", 12, true)),
	vp_({ 0, 0, 702, 506 }),
	green({ 0, 255, 0, 255 }),
	blue({ 0, 191, 255, 255 }),
	red({ 255, 0, 0, 255 }),
	yellow({ 255, 255, 0, 255 })
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
	for (auto entity : es.entities_with_components<LifeTime>())
	{
		auto lt = entity.component<LifeTime>();
		if(lt->done() && lt->destroyWhenDone_)
			entity.destroy();
		else
			lt->update(dt);
	}
	for (auto entity : es.entities_with_components<Draw, Position>())
	{
		if(entity.has_component<ViewPort>())
			SDL_RenderSetViewport(game_.renderer_, &entity.component<ViewPort>()->viewPort_);
		else
			SDL_RenderSetViewport(game_.renderer_, &vp_);

		auto draw = entity.component<Draw>();
		auto pos = entity.component<Position>();

		if (entity.has_component<Animate>() && entity.has_component<Velocity>())
		{
			auto vel = entity.component<Velocity>();

			if (vel->x_ > 0)
			{
				draw->angle_ = 0;
				if (vel->y_ < 0)
					draw->angle_ -= 45;
				else if (vel->y_ > 0)
					draw->angle_ += 45;
			}
			else if (vel->x_ < 0)
			{
				draw->angle_ = -180;
				if (vel->y_ < 0)
					draw->angle_ += 45;
				else if (vel->y_ > 0)
					draw->angle_ -= 45;
			}
			else
			{
				if (vel->y_ < 0)
					draw->angle_ = -90;
				else if (vel->y_ > 0)
					draw->angle_ = 90;
			}
		}

		if(entity.has_component<Creep>())
		{
			auto creepE = entity.component<Creep>();
			if(creepE->info_.speedAura_.area_ > 0)
			{
				filledCircleRGBA(game_.renderer_, pos->x_ + 23, pos->y_ + 23, creepE->info_.speedAura_.area_, 0, 0, 255, 64);
			}
			if(creepE->info_.healAura_.area_ > 0)
			{
				filledCircleRGBA(game_.renderer_, pos->x_ + 23, pos->y_ + 23, creepE->info_.healAura_.area_, 0, 255, 0, 64);
			}
		}

		if(entity.has_component<LifeTime>() && draw->fadeOnLifeTime_)
		{
			draw->sprite_.setAlpha( 255 * (1 - entity.component<LifeTime>()->progress()) );
		}

		int width = 0;
		if(entity.has_component<Animate>())
		{
			auto animation = entity.component<Animate>();
			draw->crop_ = animation->sprite_.getRect();
			width = animation->sprite_.frameWidth();
		}
		else
		{
			width = draw->sprite_.width_;
		}

		if(draw->crop_.w != 0 && draw->crop_.h != 0)
			draw->sprite_.render(pos->x_, pos->y_, &draw->crop_, draw->angle_);
		else
			draw->sprite_.render(pos->x_, pos->y_, nullptr, draw->angle_);

		if(draw->fillColor_.a > 0)
		{
			if(draw->crop_.w != 0 && draw->crop_.h != 0)
				boxRGBA(game_.renderer_, pos->x_, pos->y_, pos->x_ + draw->crop_.w, pos->y_ + draw->crop_.h, draw->fillColor_.r, draw->fillColor_.g, draw->fillColor_.b, draw->fillColor_.a);
			else
				boxRGBA(game_.renderer_, pos->x_, pos->y_, pos->x_ + draw->sprite_.width_, pos->y_ + draw->sprite_.height_, draw->fillColor_.r, draw->fillColor_.g, draw->fillColor_.b, draw->fillColor_.a);
		}

		if(entity.has_component<Range>() && entity.component<Range>()->draw_)
		{
			auto range = entity.component<Range>();
			circleRGBA(game_.renderer_, pos->x_ + 20, pos->y_ + 20, range->range_ + range->bonus_, 255, 0, 0, 255);
		}

		if(entity.has_component<Health>())
		{
			auto health = entity.component<Health>();

			// draw red bar
			int beginBar = pos->x_ + 5;
			int endBar = beginBar + width * 0.8;

			int beginSizeBar = pos->y_ - 2;
			int endSizeBar = pos->y_ - 5;
			boxRGBA(game_.renderer_, beginBar, beginSizeBar, endBar, endSizeBar, 255, 0, 0, 255);

			// draw green bar
			double percent = health->currentHealth() * 100 / health->maxHealth();
			percent = percent / 100;

			int greenEndBar = (endBar - beginBar) * percent;
			boxRGBA(game_.renderer_, beginBar, beginSizeBar, beginBar+greenEndBar , endSizeBar, 0, 255, 0, 255);
		}
		
		if (entity.has_component<Creep>())
		{
			int offset = 0;
			if (entity.has_component<Stun>())
			{
				auto icon = rm_.getTexture("resource/stun.gif");
				icon->render(pos->x_ + width + 2, pos->y_ + offset);

				std::shared_ptr<Texture> stun(rm_.getTexture("Stun", yellow, font_));
				auto& lt = entity.component<Stun>()->lifeTime_;
				stun->setAlpha(255 * (1 - lt.progress()));
				stun->render(pos->x_ + width + 2 + icon->width_, pos->y_ + offset);
				offset += 8;
			}
			if (entity.has_component<Slow>())
			{
				auto icon = rm_.getTexture("resource/slow.gif");
				icon->render(pos->x_ + width + 2, pos->y_ + offset);

				std::shared_ptr<Texture> slow(rm_.getTexture("Slow", blue, font_));
				auto& lt = entity.component<Slow>()->lifeTime_;
				slow->setAlpha(255 * (1 - lt.progress()));

				slow->render(pos->x_ + width + 2 + icon->width_, pos->y_ + offset);
				offset += 8;
			}
			if (entity.has_component<Bleed>())
			{
				auto icon = rm_.getTexture("resource/bleed.gif");
				icon->render(pos->x_ + width + 2, pos->y_ + offset);

				std::shared_ptr<Texture> bleed(rm_.getTexture("Bleed", red, font_));
				auto& lt = entity.component<Bleed>()->lifeTime_;
				bleed->setAlpha(255 * (1 - lt.progress()));
				bleed->render(pos->x_ + width + 2 + icon->width_, pos->y_ + offset);
				offset += 8;
			}
		}
	}

	for (auto box : es.entities_with_components<Box>())
	{
		if (box.has_component<ViewPort>())
			SDL_RenderSetViewport(game_.renderer_, &box.component<ViewPort>()->viewPort_);
		else
			SDL_RenderSetViewport(game_.renderer_, &vp_);

		auto boxC = box.component<Box>();
		SDL_Rect r = boxC->box_;
		SDL_Color c = boxC->color_;

		int beginBar = r.x;
		int endBar = r.x + r.w;

		int beginSizeBar = r.y;
		int endSizeBar = r.y + r.h;
				
		roundedBoxRGBA(game_.renderer_, beginBar, beginSizeBar, endBar, endSizeBar, 1, c.r, c.g, c.b, c.a);
	}

//	std::stringstream ss;
//	int x, y;
//	SDL_GetMouseState(&x, &y);
//	ss << x << "," << y;
//	std::shared_ptr<Texture> mousePos(rm_.getTexture(ss.str(), yellow, font_));
//	mousePos->render(0,0);
}
