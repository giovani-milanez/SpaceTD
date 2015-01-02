/*
 * RenderSystem.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef RENDERSYSTEM_HPP_
#define RENDERSYSTEM_HPP_

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

class Game;
class ResourceManager;
typedef struct _TTF_Font TTF_Font;
class Texture;

class RenderSystem : public entityx::System<RenderSystem>
{
public:
	RenderSystem();
	virtual ~RenderSystem();

	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt);

	Game& game_;

	ResourceManager& rm_;
	TTF_Font *font_;
	SDL_Rect vp_;
private:
	SDL_Color green;
	SDL_Color blue;
	SDL_Color red;
	SDL_Color yellow;
	
//	std::shared_ptr<Texture> stun, slow, bleed;
};

#endif /* RENDERSYSTEM_HPP_ */
