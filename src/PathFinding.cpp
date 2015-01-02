
#include "PathFinding.hpp"
#include "Operations.hpp"

#include <algorithm>
#include <iostream>

bool operator==(const TileNode& a, const TileNode& b)
{
	return SDL_RectEquals(&a.rect, &b.rect);
}

bool operator<(const TileNode& a, const TileNode& b)
{
	return a.f() < b.f();
}


int clearance(const SDL_Rect& tile, const TileMap& map, const std::vector<SDL_Rect> colliders)
{
	auto hitsBoundary = [&](const SDL_Rect& tile)
	{
		return tile.x + tile.w > map.wp || tile.y + tile.h > map.hp;
	};
	auto expand = [&](int range)
	{
		for (int c = 0; c < range; c++)
		{
			for (int r = 0; r < range; r++)
			{
				SDL_Rect neighbor{ tile.x + (map.tileW * c), tile.y + (map.tileH * r), map.tileW, map.tileH };
				auto it = std::find_if(colliders.begin(), colliders.end(),
					[&](const SDL_Rect& collider) {
					return collidesWith(neighbor, collider);
				});

				if (it != colliders.end() || hitsBoundary(neighbor))
					return false;
			}
		}
		return true;
	};

	int result = 1;
	int range = 2;
	while (expand(range++))
		result++;

	return result;
}


int manhattanHeuristic(const SDL_Rect& node, const SDL_Rect& finalNode, const TileMap& map)
{
	int distX = finalNode.x - node.x;
	distX = ceil(distX / map.tileW);
	if (distX < 0)
		distX = distX * -1;

	int distY = finalNode.y - node.y;
	distY = ceil(distY / map.tileH);
	if (distY < 0)
		distY = distY * -1;

	return distX + distY;
}

std::vector<TileNode> getNeighborhood(const TileNode& node, const SDL_Rect& finalNode, const TileMap& map, const std::vector<SDL_Rect> colliders)
{
	std::vector<TileNode> neighbourhood;

	auto inBounds = [&](const SDL_Rect& rect)
	{ return rect.x >= 0 &&
	rect.x + rect.w <= map.wp &&
	rect.y >= 0 &&
	rect.y + rect.h <= map.hp; };

	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			if (r == 1 && c == 1) // its the center node
				continue;

			SDL_Rect rect{ node.rect.x + (int)((c - 1) * node.rect.w), node.rect.y + (int)((r - 1) * node.rect.h), node.rect.w, node.rect.h };
			if (inBounds(rect))
			{
				auto it = std::find_if(colliders.begin(), colliders.end(),
					[&](const SDL_Rect& collider) {
					return collidesWith(rect, collider);
				});

				if (it == colliders.end())
				{
					bool diagonal = (r == 0 && c == 0) || (r == 0 && c == 2) || (r == 2 && c == 0) || (r == 2 && c == 2);
					bool skip = false;
					if (diagonal) // check if the diagonal tile has a collider neighbour, if so skip it.
					{
						for (int r1 = 0; r1 < 3; r1++)
						{
							for (int c1 = 0; c1 < 3; c1++)
							{
								if (r1 == 1 && c1 == 1) // its the center node
									continue;

								SDL_Rect rect1{ rect.x + (int)((c1 - 1) * rect.w), rect.y + (int)((r1 - 1) * rect.h), rect.w, node.rect.h };

								auto it1 = std::find_if(colliders.begin(), colliders.end(),
									[&](const SDL_Rect& collider) {
									return collidesWith(rect1, collider);
								});

								skip = it1 != colliders.end();

								if (skip)
									break;
							}
							if (skip)
								break;
						}
					}

					if (!skip)
					{
						TileNode neighbourNode;
						neighbourNode.rect = rect;
						neighbourNode.g = (diagonal ? 14 : 10) + node.g;
						neighbourNode.h = manhattanHeuristic(rect, finalNode, map) * 10;
						neighbourNode.parent = (TileNode *)&node;
						neighbourhood.push_back(neighbourNode);
					}
				}
			}
		}
	}
	return neighbourhood;
}

std::vector<SDL_Rect> getPath(const SDL_Rect& start, const SDL_Rect& goal, const TileMap& map, const std::vector<SDL_Rect> colliders)
{
	TileNode initialNode;
	initialNode.rect = start;
	initialNode.parent = nullptr;
	initialNode.g = 0;
	initialNode.h = manhattanHeuristic(start, goal, map) * 10;


	//	std::vector<TileNode> openList { initialNode };
	std::vector<TileNode> openList;
	openList.reserve(map.h * map.w);
	openList.push_back(initialNode);
	std::vector<TileNode> closedList;
	closedList.reserve(map.h * map.w);

	std::vector<SDL_Rect> path;
	bool done = false;
	while (!done)
	{
		auto currentIt = std::min_element(openList.begin(), openList.end());

		if (currentIt == openList.end())
		{
			std::cout << "DEU PAU" << std::endl;
			for (auto& e : openList)
			{
				std::cout << e.rect.x << "," << e.rect.y << std::endl;
			}

			return path;
		}
		else
		{
		}

		closedList.push_back(*currentIt);
		TileNode& current = closedList.back();

		SDL_Rect currentRect = current.rect;
		int currentG = current.g;
		
		if ( SDL_RectEquals(&currentRect, &goal) )
		{
			path.push_back(goal);
			TileNode *parent = current.parent;
			while (parent)
			{
				path.push_back(parent->rect);
				parent = parent->parent;
			}
			done = true;
			break;
		}
		auto new_end = std::remove(openList.begin(), openList.end(), current);
		openList.erase(new_end, openList.end());

		//		std::cout << std::endl << "ATUAL: " << currentRect.x << "," << currentRect.y << std::endl;
		auto neighbourhood = getNeighborhood(current, goal, map, colliders);
		for (auto& neighbour : neighbourhood)
		{
			//			std::cout << "VIZINHO: " << neighbour.rect.x << "," << neighbour.rect.y << std::endl;
			if (std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
				continue;

			if (std::find(openList.begin(), openList.end(), neighbour) != openList.end())
			{
				int difX = ceil((neighbour.rect.x - currentRect.x) / map.tileW);
				int difY = ceil((neighbour.rect.y - currentRect.y) / map.tileH);
				bool diagonal = difX != 0 && difY != 0;

				int score = currentG + (diagonal ? 14 : 10);
				if (score < neighbour.g)
				{
					neighbour.parent = &current;
				}
			}
			else
			{
			//	if (clearance(neighbour.rect, map, colliders) > 1 || SDL_RectEquals(&neighbour.rect, &goal))
				{
					openList.push_back(neighbour);
				}

			}
		}
	}
	return path;
}
