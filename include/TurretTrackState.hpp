/*
 * TurretTrackState.hpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#ifndef TURRETTRACKSTATE_HPP_
#define TURRETTRACKSTATE_HPP_

#include "GameState.hpp"

#include "TurretInfo.hpp"

#include "events/InputEvent.hpp"
#include "entityx/entityx.h"

class Game;
class GamePlay;

class TurretTrackState : public GameState, public entityx::EntityX, public entityx::Receiver<InputEvent>
{
public:
	TurretTrackState(const TurretInfo& ti, GamePlay* gamePlay, int x, int y);
	virtual ~TurretTrackState();

	void handleEvents(const float deltaTime);
	void update(const float deltaTime);
	void render(const float deltaTime);
	void restart();
	std::string name() const;
	WindowType getType() const
	{
		return WindowType::MODAL;
	}

	void configure(entityx::EventManager &event_manager)
	{
		event_manager.subscribe<InputEvent>(*this);
	}

	void receive(const InputEvent &input);
	void quitState();

	GamePlay* gamePlay_;
private:
	Game& game_;
	TurretInfo ti_;
	entityx::Entity trackE_;
	bool quit_;
};

#endif /* TURRETTRACKSTATE_HPP_ */
