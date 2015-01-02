/*
 * RenderSystem.hpp
 *
 *  Created on: 10/10/2014
 *      Author: giovani
 */

#ifndef MODIFIERSYSTEM_HPP_
#define MODIFIERSYSTEM_HPP_

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

class Game;
class ResourceManager;
typedef struct _TTF_Font TTF_Font;

class ModifierSystem : public entityx::System<ModifierSystem>
{
public:
	ModifierSystem();
	virtual ~ModifierSystem();

	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt);

	Game& game_;
	ResourceManager& rm_;
	TTF_Font *font_;
private:
	SDL_Color green;
	SDL_Color blue;
	SDL_Color red;
	SDL_Color yellow;
};

#endif /* MODIFIERSYSTEM_HPP_ */
