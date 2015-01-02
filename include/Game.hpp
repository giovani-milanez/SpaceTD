/*
 * Game.hpp
 *
 *  Criado em: 11/08/2014
 *      Autor: Giovani Milanez Espindola
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include "GameState.hpp"

#include <SDL2/SDL.h>

#include <vector>
#include <string>
#include <memory>
#include <random>

bool operator ==(const SDL_Rect& a, const SDL_Rect& b);

class Game
{
public:
	Game();
	virtual ~Game();

	SDL_Window* window_;
	SDL_Renderer* renderer_;
	int screen_width_;
	int screen_height_;
	float scaleW_, scaleH_;

	bool initGame(const std::string& title);
	void loop();

	void pushState(const std::shared_ptr<GameState>& state);
	void popState();
	void removeState(const std::string& stateName);
	std::shared_ptr<GameState> peekState();
	std::shared_ptr<GameState> getState(const std::string& stateName);

	static Game& getInstance()
	{
		static Game game;
		return game;
	}
	bool quit_;
	std::mt19937 rng_;
	std::string toPop_;
	int fps_;
private:
	SDL_Event event_;
	Uint32 new_time_;
	Uint32 old_time_;

	std::vector<std::shared_ptr<GameState>> states_;

	void update(const float deltaTime);
	void render(const float deltaTime);
	void handleEvents(const float deltaTime);

	void quit();
};

#endif /* GAME_HPP_ */
