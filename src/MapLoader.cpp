/*
 * MapLoader.cpp
 *
 *      Author: Giovani Milanez Espindola
 *	   Contact: giovani.milanez@gmail.com
 *	Created on: 10/08/2014
 */

#include "MapLoader.hpp"
#include "Operations.hpp"

#include "picojson.h"

#include <iostream>
#include <fstream>

TileMap JSONMapLoader::loadMap(const std::string& file) const
{
	TileMap map;

//    std::ifstream ifs(file);
//    picojson::value v;
//    ifs >> v;

	std::string fileStr = readFile(file);
	picojson::value v;
	std::string err;
	picojson::parse(v, fileStr.begin(), fileStr.end(), &err);

    picojson::object& obj = v.get<picojson::object>();

	map.h = obj["height"].get<double>();
	map.w = obj["width"].get<double>();
	map.tileW = obj["tilewidth"].get<double>();
	map.tileH = obj["tileheight"].get<double>();
	map.hp = map.h * map.tileH;
	map.wp = map.w * map.tileW;

	auto tilesets = obj["tilesets"].get<picojson::array>();
	map.tilesets.reserve(tilesets.size());
	for(unsigned int i = 0; i < tilesets.size(); i++)
	{
		auto tileset = tilesets[i].get<picojson::object>();
		TileSet ts;
		ts.firstgid = tileset["firstgid"].get<double>();
		ts.name = tileset["name"].get<std::string>();
		ts.imagePath = tileset["image"].get<std::string>();
		ts.imageheight = tileset["imageheight"].get<double>();
		ts.imagewidth = tileset["imagewidth"].get<double>();
		ts.tilewidth = tileset["tilewidth"].get<double>();
		ts.tileheight = tileset["tileheight"].get<double>();
		ts.margin = tileset["margin"].get<double>();
		ts.spacing = tileset["spacing"].get<double>();
		ts.lastgid = floor(ts.imagewidth / ts.tilewidth) * floor(ts.imageheight / ts.tileheight) + ts.firstgid - 1;
		map.tilesets.push_back(ts);
	}

	auto layers = obj["layers"].get<picojson::array>();
	map.layers.reserve(layers.size());
	for(unsigned int i = 0; i < layers.size(); i++)
	{
		auto layer = layers[i].get<picojson::object>();

		TileLayer l;
		l.name = layer["name"].get<std::string>();
		l.visible = layer["visible"].get<bool>();
		l.h = layer["height"].get<double>();
		l.w = layer["width"].get<double>();
		l.opacity = layer["opacity"].get<double>();
		l.x = layer["x"].get<double>();
		l.y = layer["y"].get<double>();

		if(layer["type"].get<std::string>() == "tilelayer")
		{
			auto tiles = layer["data"].get<picojson::array>();
			l.tiles.reserve(tiles.size());
			for(unsigned int t = 0; t < tiles.size(); t++)
			{
				int gid = tiles[t].get<double>();
				for(auto& ts : map.tilesets)
				{
					if(gid >= ts.firstgid && gid <= ts.lastgid)
					{
						l.tiles.push_back(Tile(t, gid, ts.tileheight, ts.tilewidth, map.w, map.tileW, map.tileH, ts));
						break;
					}
				}
			}
		}
		else if(layer["type"].get<std::string>() == "objectgroup")
		{
			auto objects = layer["objects"].get<picojson::array>();
			l.objects.reserve(objects.size());
			for(unsigned int o = 0; o < objects.size(); o++)
			{
				auto object = objects[o].get<picojson::object>();
				TileObject ob;
				ob.h = object["height"].get<double>();
				ob.w = object["width"].get<double>();
				ob.x = object["x"].get<double>();
				ob.y = object["y"].get<double>();
				ob.name = object["name"].get<std::string>();
				ob.type = object["type"].get<std::string>();
				ob.visible = object["visible"].get<bool>();
				l.objects.push_back(ob);
			}
		}

		map.layers.push_back(l);
	}

	auto prop = obj["properties"];

	if(!prop.is<picojson::null>())
	{
		const picojson::object& o = prop.get<picojson::object>();
		for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i)
		{
			map.properties.insert(std::make_pair(i->first, i->second.get<std::string>()));
		}
	}
	return map;
}
