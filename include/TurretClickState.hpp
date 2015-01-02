/*
 * TurretClickState.hpp
 *
 *  Created on: 13/10/2014
 *      Author: giovani
 */

#ifndef TURRETCLICKSTATE_HPP_
#define TURRETCLICKSTATE_HPP_

#include "GameState.hpp"
#include "TurretInfo.hpp"

#include "events/InputEvent.hpp"
#include "entityx/entityx.h"

class Game;
class GamePlay;

class TurretClickState : public GameState, public entityx::EntityX, public entityx::Receiver<InputEvent>
{
public:
	TurretClickState(entityx::Entity turret, GamePlay* gamePlay);
	virtual ~TurretClickState();

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

private:
	entityx::Entity turret_, menu_, clickE_;
	GamePlay* gamePlay_;
	Game& game_;
};

#endif /* TURRETCLICKSTATE_HPP_ */
