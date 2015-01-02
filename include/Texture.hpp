/*
 * Texture.hpp
 *
 *  Criado em: 12/08/2014
 *      Autor: Giovani Milanez Espindola
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include "Game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

class Texture {
public:
	Texture(const std::string text, SDL_Color color, TTF_Font *font);
	explicit Texture(SDL_Texture* texture);
	explicit Texture(const std::string& filePath);
	virtual ~Texture();

	Texture(Texture&& src);
	Texture& operator=(Texture&& src);

	//Set color modulation
	void setColor( Uint8 red, Uint8 green, Uint8 blue );

	//Set blending
	void setBlendMode( SDL_BlendMode blending );

	//Set alpha modulation
	void setAlpha( Uint8 alpha );

	//Renders texture at given point
	void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE ) const;

	//Image dimensions
	int width_;
	int height_;

	SDL_Texture* texture_;
private:
	Game& game;

};

#endif /* TEXTURE_HPP_ */
