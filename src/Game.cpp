/*
 * Game.cpp
 *
 *  Criado em: 11/08/2014
 *      Autor: Giovani Milanez Espindola
 */

#include "Game.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <algorithm>
#include <iostream>
#include <chrono>

bool operator ==(const SDL_Rect& a, const SDL_Rect& b)
{
	return a.x == b.x && a.y == b.y && a.h == b.h && a.w == b.w;
}

Game::Game() :
	window_(nullptr),
	renderer_(nullptr),
	screen_width_(15 * 46 + 12),
	screen_height_(11 * 46),
	quit_(false),
	new_time_(0),
	old_time_(0)
{
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Game::initGame(const std::string& title)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	Uint32 flags = SDL_WINDOW_SHOWN;

#ifdef ANDROID_BUILD
	float idealW = 702;
	float idealH = 506;

	flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 0, 0, flags);

	SDL_GetWindowSize(window_, &screen_width_, &screen_height_);

	scaleW_ = screen_width_ / idealW;
	scaleH_ = screen_height_ / idealH;

	screen_width_ = idealW;
	screen_height_ = idealH;
#else
	window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, screen_width_, screen_height_, flags);
	scaleW_ = 1;
	scaleH_ = 1;
#endif

	
	if(window_ == NULL)
	{
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	if(renderer_ == NULL)
	{
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	/*float idealW = 702;
	float idealH = 506;*/
#ifdef ANDROID_BUILD
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer_, screen_width_, screen_height_);
	SDL_RenderSetScale(renderer_, scaleW_, scaleH_);
#endif

	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		SDL_DestroyWindow(window_); window_ = nullptr;
		SDL_DestroyRenderer(renderer_); renderer_ = nullptr;
		return false;
	}

	 //Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );

		SDL_DestroyWindow(window_); window_ = nullptr;
		SDL_DestroyRenderer(renderer_); renderer_ = nullptr;
		return false;
	}

	int isCapture = 0;
	int n = SDL_GetNumAudioDevices(isCapture);
	std::cout << "Audio devices: " << n << std::endl;
	for (int i=0; i<n; i++) {
		auto name = SDL_GetAudioDeviceName(i, isCapture);
		std::cout << "Audio: " << name << std::endl;
	}

	if (TTF_Init() != 0){
		printf( "TTF_Init could not initialize! TTF_Init Error: %s\n", TTF_GetError() );
		SDL_DestroyWindow(window_); window_ = nullptr;
		SDL_DestroyRenderer(renderer_); renderer_ = nullptr;
		return false;
	}
	auto s = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rng_.seed(s);

	return true;
}

void Game::loop()
{
	int countedFrames = 0;

	while(quit_ == false)
	{
		old_time_ = new_time_;
		new_time_ = SDL_GetTicks();
		if(new_time_ > old_time_)
		{
			float deltaTime = (float)(new_time_ - old_time_) / 1000.0f;

			handleEvents(deltaTime);
			update(deltaTime);
			render(deltaTime);

			if (toPop_ != "")
			{
				removeState(toPop_);
				toPop_ = "";
			}
		}
		countedFrames++;
		fps_ = (int) (countedFrames / ( new_time_ / 1000.f ));
		if( fps_ > 2000000 )
			fps_ = 0;
	}
	quit();
}


void Game::update(const float deltaTime)
{
	if(states_.size() > 0)
	{
		if(states_.back()->getType() == WindowType::FULL)
		{
			states_.back()->update(deltaTime);
		}
		else
		{

			std::vector<std::shared_ptr<GameState>> toUpdate;

			for (std::vector<std::shared_ptr<GameState>>::reverse_iterator i = states_.rbegin(); i != states_.rend(); ++i )
			{
				if((*i)->getType() == WindowType::MODAL)
				{
					toUpdate.push_back((*i));
				}
				else
				{
					(*i)->update(deltaTime);
					break;
				}
			}
			for(auto& state : toUpdate)
			{
				state->update(deltaTime);
			}
		}
	}
}

void Game::render(const float deltaTime)
{
	SDL_RenderClear(renderer_);

//	float idealW = 720;
//	float idealH = 555;

	//float idealW = 702;
	//float idealH = 506;
	//
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	//SDL_RenderSetLogicalSize(renderer_, idealW, idealH);
	//SDL_RenderSetScale(renderer_, screen_width_ / idealW, screen_height_ / idealH);

	if(states_.size() > 0)
	{
		if(states_.back()->getType() == WindowType::FULL)
		{
			states_.back()->render(deltaTime);
		}
		else
		{
			std::vector<std::shared_ptr<GameState>> toRender;

			for (std::vector<std::shared_ptr<GameState>>::reverse_iterator i = states_.rbegin(); i != states_.rend(); ++i )
			{
				if((*i)->getType() == WindowType::MODAL)
				{
					toRender.push_back((*i));
				}
				else
				{
					(*i)->render(deltaTime);
					break;
				}
			}
			for(auto& state : toRender)
			{
				state->render(deltaTime);
			}
		}
	}
	SDL_RenderPresent(renderer_);
}

void Game::handleEvents(const float deltaTime)
{
	if(states_.size() > 0)
	{
		states_.back()->handleEvents(deltaTime);
	}
}

void Game::quit()
{
	std::cout << "quitting" << std::endl;
	while(states_.size() > 0)
	{
//		states_.back()->quit();

		/*we need to delete the dynamically allocated space*/
//		delete states_.back();

		states_.pop_back();
	}

}

void Game::pushState(const std::shared_ptr<GameState>& state)
{
	//std::cout << "pushing state " << state->name() << std::endl;
	states_.push_back(state);
//	if(state->init() == false)
//	{
//		quit_ = true;
//	}
}

void Game::popState()
{
	removeState(states_.back()->name());
}

void Game::removeState(const std::string& stateName)
{
	auto it = std::find_if(states_.begin(), states_.end(), [&](const std::shared_ptr<GameState>& state){ return state->name() == stateName; });
	if(it != states_.end())
	{
		//std::cout << "removendo state " << stateName << std::endl;
		states_.erase(it);
		if(states_.size() == 0)
		{
			quit_ = true;
		}
	}
}

std::shared_ptr<GameState> Game::getState(const std::string& stateName)
{
	auto it = std::find_if(states_.begin(), states_.end(), [&](const std::shared_ptr<GameState>& state){ return state->name() == stateName; });
	if (it != states_.end())
	{
		return *it;
	}
	return std::shared_ptr < GameState >() ;
}

std::shared_ptr<GameState> Game::peekState()
{
	if(states_.size() > 0)
		return states_.back();

	return std::shared_ptr<GameState>();
}

