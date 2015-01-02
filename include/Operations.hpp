/*
 * Operations.hpp
 *
 *  Created on: 06/10/2014
 *      Author: giovani
 */

#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_

#include "Circle.hpp"
#include "entityx/entityx.h"

#include <SDL2/SDL.h>

#include <vector>

const float PI = 3.14159265;

SDL_Rect getRect(const entityx::Entity& entity);

bool collidesWith(const entityx::Entity& a, const entityx::Entity& b);
bool collidesWith(const Circle& a, const SDL_Rect& b);
bool collidesWith(const SDL_Rect& a, const SDL_Rect& b);

void rotateToTarget(entityx::Entity& entityToRotate, entityx::Entity& target, double dt);
void rotateToAngle(entityx::Entity& entityToRotate, double angle, double dt);

std::vector<entityx::Entity> creepsInRange(entityx::EntityManager &entityManager, const Circle& range);

std::string readFile(const std::string& filename);


#endif /* OPERATIONS_HPP_ */
