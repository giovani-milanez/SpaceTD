/*
 * GameState.hpp
 *
 *  Criado em: 11/08/2014
 *      Autor: Giovani Milanez Espindola
 */

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

#include <string>

union SDL_Event;

enum WindowType
{
	FULL, MODAL
};

class GameState
{
public:
	virtual ~GameState(){};
	virtual void handleEvents(const float deltaTime) = 0;
	virtual void update(const float deltaTime) = 0;
	virtual void render(const float deltaTime) = 0;
	virtual void restart() = 0;
	virtual std::string name() const = 0;
	virtual WindowType getType() const = 0;
};

#endif /* GAMESTATE_HPP_ */
