/*
 * BuyState.hpp
 *
 *  Created on: 16/10/2014
 *      Author: giovani
 */

#ifndef BUYSTATE_HPP_
#define BUYSTATE_HPP_

#include "GameState.hpp"
#include "TurretInfo.hpp"
#include "events/InputEvent.hpp"
#include "entityx/entityx.h"

class Game;
class GamePlay;

class BuyState : public GameState, public entityx::EntityX, public entityx::Receiver<InputEvent>
{
public:
	BuyState(entityx::Entity turret, const TurretInfo& ti, GamePlay* gamePlay);
	virtual ~BuyState();

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
	GamePlay* gamePlay_;
	Game& game_;
	TurretInfo ti_;
	entityx::Entity turret_, menu_;
};

#endif /* BUYSTATE_HPP_ */
