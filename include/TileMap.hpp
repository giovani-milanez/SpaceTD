/*
 * TileMap.hpp
 *
 *      Author: Giovani Milanez Espindola
 *	   Contact: giovani.milanez@gmail.com
 *	Created on: 11/08/2014
 */

#ifndef TILEMAP_HPP_
#define TILEMAP_HPP_

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <map>

struct TileSet
{
	int firstgid, lastgid;
	std::string imagePath, name;
	int imageheight, imagewidth, tileheight, tilewidth;
	int margin, spacing;
};

class Tile
{
public:
	Tile(int pos, int gid, int h, int w, int mapW, int mapTileW, int mapTileH, TileSet& ts);
	int gid;
	SDL_Rect rect;
	SDL_Rect crop;
	//TileSet& ts;
	std::string imagePath;
};

struct TileObject
{
	int x, y, w, h;
	std::string name, type;
	bool visible;
	double opacity;
};

struct TileLayer
{
	int x, y, w, h;
	std::string name;
	bool visible;
	double opacity;
	std::vector<Tile> tiles;
	std::vector<TileObject> objects;
};


class TileMap
{
public:
	int w, h, tileW, tileH, wp, hp;
	std::vector<TileLayer> layers;
	std::vector<TileSet> tilesets;
	std::map<std::string, std::string> properties;
	void print();
};

#endif /* TILEMAP_HPP_ */
