/*
 * Texture.cpp
 *
 *  Criado em: 12/08/2014
 *      Autor: Giovani Milanez Espindola
 */

#include "Texture.hpp"
#include "Game.hpp"

#include <SDL2/SDL_image.h>


Texture::Texture(const std::string text, SDL_Color color, TTF_Font *font) :
	width_(0),
	height_(0),
	texture_(nullptr),
	game(Game::getInstance())
{
	SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), color);
	texture_ = SDL_CreateTextureFromSurface(game.renderer_, surf);
	SDL_FreeSurface(surf);
	SDL_QueryTexture(texture_, NULL, NULL, &width_, &height_);
}

Texture::Texture(SDL_Texture* texture) :
	width_(0),
	height_(0),
	texture_(texture),
	game(Game::getInstance())
{
	SDL_QueryTexture(texture_, NULL, NULL, &width_, &height_);
}

Texture::Texture(const std::string& filePath) :
	width_(0),
	height_(0),
	texture_(nullptr),
	game(Game::getInstance())
{
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( filePath.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", filePath.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		texture_ = SDL_CreateTextureFromSurface( Game::getInstance().renderer_, loadedSurface );
		if( texture_ == nullptr )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", filePath.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width_ = loadedSurface->w;
			height_ = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
}

Texture::~Texture()
{
	SDL_DestroyTexture( texture_ );
}

Texture::Texture(Texture&& src) :
		width_(src.width_),
		height_(src.height_),
		texture_(src.texture_),
		game(Game::getInstance())
{
	src.texture_ = nullptr;
}
Texture& Texture::operator=(Texture&& src)
{
	SDL_DestroyTexture( texture_ );

	width_ = src.width_;
	height_ = src.height_;
	texture_ = src.texture_;
	src.texture_ = nullptr;

	return *this;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( texture_, red, green, blue );
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode( texture_, blending );
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod( texture_, alpha );
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) const
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width_, height_ };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( game.renderer_, texture_, clip, &renderQuad, angle, center, flip );
}
