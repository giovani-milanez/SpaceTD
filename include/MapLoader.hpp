/*
 * MapLoader.hpp
 *
 *      Author: Giovani Milanez Espindola
 *	   Contact: giovani.milanez@gmail.com
 *	Created on: 10/08/2014
 */

#ifndef MAPLOADER_HPP_
#define MAPLOADER_HPP_

#include "TileMap.hpp"

#include <string>

class MapLoader
{
public:
	virtual ~MapLoader(){};
	virtual TileMap loadMap(const std::string& filePath) const = 0;
};

class JSONMapLoader : public MapLoader
{
public:
	TileMap loadMap(const std::string& filePath) const;
};


#endif /* MAPLOADER_HPP_ */
