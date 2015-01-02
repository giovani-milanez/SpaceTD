/*
 * ResourceManager.hpp
 *
 *  Criado em: 11/08/2014
 *      Autor: Giovani Milanez Espindola
 */

#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include "Game.hpp"
#include "Texture.hpp"

#include <SDL2/SDL.h>

#include <map>
#include <memory>

typedef struct _TTF_Font TTF_Font;
struct Mix_Chunk;

class ResourceManager
{
public:
	virtual ~ResourceManager();

	std::shared_ptr<Texture> getTexture(const std::string& text, SDL_Color color, TTF_Font* font);
	std::shared_ptr<Texture> getTexture(const std::string& filename);
	TTF_Font* getFont(const std::string& filename, int fontSize, bool bold = false);
	Mix_Chunk* getSound(const std::string& filename);

	static ResourceManager& getInstance()
	{
		static ResourceManager rm;
		return rm;
	}

private:
	ResourceManager();
	std::map<std::string, std::shared_ptr<Texture>> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Chunk*> sounds;
};

#endif /* RESOURCEMANAGER_HPP_ */
