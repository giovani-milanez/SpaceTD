/*
 * ResourceManager.cpp
 *
 *  Criado em: 11/08/2014
 *      Autor: Giovani Milanez Espindola
 */

#include "ResourceManager.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <sstream>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	for(auto& sound : sounds)
	{
		Mix_FreeChunk( sound.second );
	}
	sounds.clear();
	//for(auto& texture : textures)
	//{
	//	delete texture.second;
	//}
	textures.clear();

	for(auto& font : fonts)
	{
		TTF_CloseFont(font.second);
	}
	fonts.clear();
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& text, SDL_Color color, TTF_Font *font)
{
	auto i = textures.find(text);
	if(i == textures.end())
	{
		//Texture *t = new Texture(text, color, font);
		//textures.insert(std::pair<std::string, Texture *>(text, t));
		//return std::shared_ptr<Texture>(t);

		auto t = std::make_shared<Texture>(text, color, font);
		textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(text, t));
		return t;

	}
	return i->second;
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& filename)
{
	auto i = textures.find(filename);
	if(i == textures.end())
	{
		//Texture *t = new Texture(filename);
		//textures.insert(std::pair<std::string, Texture *>(filename, t));
		//return std::shared_ptr<Texture>(t);

		auto t = std::make_shared<Texture>(filename);
		textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(filename, t));
		return t;
	}
	return i->second;
}

TTF_Font* ResourceManager::getFont(const std::string& filename, int fontSize, bool bold)
{

	std::stringstream ss;
	ss << fontSize << bold;
	std::map<std::string, TTF_Font*>::iterator i = fonts.find(filename+ss.str());
	if(i == fonts.end())
	{
		TTF_Font *font = TTF_OpenFont(filename.c_str(), fontSize);
		if (font == nullptr)
			throw std::runtime_error("Error while loading font: " + filename + " " + TTF_GetError());
		
		if (bold)
			TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		else
			TTF_SetFontStyle(font, TTF_STYLE_NORMAL);


		fonts.insert(std::pair<std::string, TTF_Font*>(filename+ss.str(), font));

		return font;
	}
	return i->second;
}

Mix_Chunk* ResourceManager::getSound(const std::string& filename)
{
	std::map<std::string, Mix_Chunk*>::iterator i = sounds.find(filename);
	if(i == sounds.end())
	{
		Mix_Chunk *sound = Mix_LoadWAV(filename.c_str());
		if (sound == nullptr)
			throw std::runtime_error("Error while sound: " + filename + " " + Mix_GetError());

		sounds.insert(std::pair<std::string, Mix_Chunk*>(filename, sound));

		return sound;
	}
	return i->second;
}
