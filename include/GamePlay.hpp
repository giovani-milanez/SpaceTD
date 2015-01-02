/*
 * GamePlay.hpp
 *
 *  Created on: 30/09/2014
 *      Author: giovani
 */

#ifndef GAMEPLAY_HPP_
#define GAMEPLAY_HPP_

#include "GameState.hpp"
#include "TileMap.hpp"
#include "CreepInfo.hpp"

#include "events/InputEvent.hpp"
#include "events/DestroyEvent.hpp"
#include "events/WaveEvent.hpp"
#include "events/AutoMoveEvent.hpp"

#include "entityx/entityx.h"

class Game;
class TurretInfo;
class CreepInfo;
class ResourceManager;
typedef struct _TTF_Font TTF_Font;
typedef struct _Mix_Music Mix_Music;

class GamePlay : public GameState, public entityx::EntityX, public entityx::Receiver<InputEvent>
{
public:
	GamePlay();
	virtual ~GamePlay();

	void handleEvents(const float deltaTime);
	void update(const float deltaTime);
	void render(const float deltaTime);
	void restart();
	std::string name() const;
	WindowType getType() const
	{
		return WindowType::FULL;
	}

	void configure(entityx::EventManager &event_manager) 
	{
		event_manager.subscribe<InputEvent>(*this);
		event_manager.subscribe<DestroyEvent>(*this);
		event_manager.subscribe<AutoMoveEndEvent>(*this);
		event_manager.subscribe<WaveSpawn>(*this);
		event_manager.subscribe<WaveDone>(*this);
		event_manager.subscribe<WaveBegin>(*this);
		event_manager.subscribe<WaveEnd>(*this);
	}

	void receive(const InputEvent &input);
	void receive(const DestroyEvent &input);
	void receive(const AutoMoveEndEvent &input);
	void receive(const WaveSpawn &input);
	void receive(const WaveDone &input);
	void receive(const WaveBegin &input);
	void receive(const WaveEnd &input);

	void createCreep(const CreepInfo& creepInfo, int quantity = 1);
	void createTurret(const TurretInfo& ti, int posX, int posY);

	entityx::Entity::Id turretToUpgrade_;
	std::vector<SDL_Rect> colliders_;

	Game& game_;
	ResourceManager& rm_;
	TTF_Font* textFont_;
	TileMap map_;
	int playerGold_, playerLife_;
	std::vector<TurretInfo> turrets_;
	std::vector<entityx::Entity> turretsE_;
	SDL_Rect creepStart_, creepEnd_;
	std::vector<SDL_Rect> creepPath_;
	CreepInfo lastSelectedCreep_;
	entityx::Entity fps_, gold_, life_;
	bool loose_;
	bool paused_;

	Mix_Music *music_;
};

#endif /* GAMEPLAY_HPP_ */
