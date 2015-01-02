/*
 * TileMap.cpp
 *
 *      Author: Giovani Milanez Espindola
 *	   Contact: giovani.milanez@gmail.com
 *	Created on: 11/08/2014
 */

#include "TileMap.hpp"

#include <iostream>

Tile::Tile(int pos, int gid, int h, int w, int mapW, int mapTileW, int mapTileH, TileSet& ts)
	: /*ts(ts)*/
	imagePath(ts.imagePath)
{
	this->gid = gid;
	rect.h = h;
	rect.w = w;

	rect.x = (pos % mapW) * mapTileW;
	rect.y = (pos / mapW) * mapTileH;

	double columns = ts.imagewidth / w;

	int row = ceil((gid - ts.firstgid + 1)/ columns) - 1;
	int column = ((gid - ts.firstgid + 1) % (int)columns);
	column = column == 0 ? (int) columns - 1 : column - 1;

	crop.x = (column * w) + (ts.spacing * (column + 1));
	crop.y = (row * h) + (ts.margin * (row + 1));
	crop.h = h;
	crop.w = w;
}


void TileMap::print()
{
	std::cout << "Map: " << h << "x" << w << " tiles"<< std::endl;
	std::cout << "Tiles: " << tileH << "x" << tileW << std::endl;

	std::cout << "TILESET" << std::endl;
	for(auto& tilset : tilesets)
	{
		std::cout << "First GID: " << tilset.firstgid << std::endl;
		std::cout << "Name: " << tilset.name << std::endl;
		std::cout << "Imagem: " << tilset.imagePath << std::endl;
		std::cout << "Tamanho: " << tilset.imagewidth << "x" << tilset.imageheight << std::endl;
	}

	for(auto& layer : layers)
	{
		std::cout << "Name: " << layer.name << std::endl;
		for(auto& tile : layer.tiles)
		{
			std::cout << "GID - " << tile.gid << " (x,y): " << tile.rect.x << "," << tile.rect.y << " SRC " << tile.crop.x << ", " << tile.crop.y << std::endl;
		}
	}
}
