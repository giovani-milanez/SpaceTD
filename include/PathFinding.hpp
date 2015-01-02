/*
* PathFinding.hpp
*
*  Created on: 30/09/2014
*      Author: giovani
*/

#ifndef PATHFINDING_HPP_
#define PATHFINDING_HPP_

#include "TileMap.hpp"

#include <SDL2/SDL.h>

#include <vector>

struct TileNode
{
	TileNode* parent;
	SDL_Rect rect;
	float g;
	float h;
	float f() const { return g + h; }
};


bool operator==(const TileNode& a, const TileNode& b);
bool operator<(const TileNode& a, const TileNode& b);

int clearance(const SDL_Rect& tile, const TileMap& map, const std::vector<SDL_Rect> colliders);
int manhattanHeuristic(const SDL_Rect& node, const SDL_Rect& finalNode, const TileMap& map);
std::vector<TileNode> getNeighborhood(const TileNode& node, const SDL_Rect& finalNode, const TileMap& map, const std::vector<SDL_Rect> colliders);
std::vector<SDL_Rect> getPath(const SDL_Rect& start, const SDL_Rect& goal, const TileMap& map, const std::vector<SDL_Rect> colliders);

#endif /* PATHFINDING_HPP_ */
