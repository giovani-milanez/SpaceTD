/*
 * ChooseCreep.hpp
 *
 *  Created on: 15/10/2014
 *      Author: giovani
 */

#ifndef CHOOSECREEP_HPP_
#define CHOOSECREEP_HPP_

#include "GameState.hpp"
#include "CreepInfo.hpp"

#include "events/InputEvent.hpp"

#include "entityx/entityx.h"

class Game;
class GamePlay;

class ChooseCreep : public GameState, public entityx::EntityX, public entityx::Receiver<InputEvent>
{
public:
	ChooseCreep(GamePlay& gamePlay);
	virtual ~ChooseCreep();

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
	}

	void receive(const InputEvent &input);
private:
	GamePlay& gamePlay_;
	Game& game_;
	std::vector<CreepInfo> creeps_;
	std::vector<entityx::Entity> viewPorts_;
};

#endif /* CHOOSECREEP_HPP_ */
