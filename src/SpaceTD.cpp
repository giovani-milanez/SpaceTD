//============================================================================
// Name        : SpaceTD.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Game.hpp"
#include "GamePlay.hpp"

int main(int argc, char* argv[])
{
	try{
		Game& game = Game::getInstance();
		if(game.initGame("Space TD"))
		{
			game.pushState(std::make_shared<GamePlay>());
			game.loop();
		}
	}catch(const std::exception& ex){
		std::cout << ex.what() << std::endl;
	}
	system("pause");
}
