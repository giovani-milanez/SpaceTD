/*
 * Operations.hpp
 *
 *  Created on: 06/10/2014
 *      Author: giovani
 */

#include "Operations.hpp"

#include "Game.hpp"

#include "components/Position.hpp"
#include "components/Draw.hpp"
#include "components/Animate.hpp"
#include "components/Turret.hpp"
#include "components/Creep.hpp"

double distanceSquared( int x1, int y1, int x2, int y2 )
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

SDL_Rect getRect(const entityx::Entity& entity)
{
	auto position = entity.component<Position>();
	auto draw = entity.component<Draw>();
	int w = 0, h = 0;
	if(entity.has_component<Animate>())
	{
		auto animation = entity.component<Animate>();
		w = animation->sprite_.frameWidth();
		h = animation->sprite_.frameHigh();
	}
	else
	{
		w = draw->sprite_.width_;
		h = draw->sprite_.height_;
	}
	return SDL_Rect{ (int)position->x_, (int)position->y_, w, h };
}

bool collidesWith(const Circle& a, const SDL_Rect& b)
{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if( a.x < b.x )
	{
		cX = b.x;
	}
	else if( a.x > b.x + b.w )
	{
		cX = b.x + b.w;
	}
	else
	{
		cX = a.x;
	}

	//Find closest y offset
	if( a.y < b.y )
	{
		cY = b.y;
	}
	else if( a.y > b.y + b.h )
	{
		cY = b.y + b.h;
	}
	else
	{
		cY = a.y;
	}

	return distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r;
}

bool collidesWith(const SDL_Rect& a, const SDL_Rect& b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x; // 114
	rightA = a.x + a.w ; // 116
	topA = a.y; // 295
	bottomA = a.y + a.h ; // 297

	//Calculate the sides of rect B
	leftB = b.x; // 135
	rightB = b.x + b.w ; // 211
	topB = b.y; // 276
	bottomB = b.y + b.h ;  // 351

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool collidesWith(const entityx::Entity& a, const entityx::Entity& b)
{
	return collidesWith(getRect(a), getRect(b));
}


void rotateToTarget(entityx::Entity& entityToRotate, entityx::Entity& target, double dt)
{
	auto posCreep = target.component<Position>();
	Position& posCreepRef = *posCreep.get();

	auto posTurret = entityToRotate.component<Position>();
	Position& posTurretRef = *posTurret.get();

    // calculate the angle theta from the deltaY and deltaX values
    // (atan2 returns radians values from [-PI,PI])
    // 0 currently points EAST.
    // NOTE: By preserving Y and X param order to atan2,  we are expecting
    // a CLOCKWISE angle direction.
    double theta = atan2(posCreepRef.y_ - posTurretRef.y_, posCreepRef.x_ - posTurretRef.x_);

    // rotate the theta angle clockwise by 90 degrees
    // (this makes 0 point NORTH)
    // NOTE: adding to an angle rotates it clockwise.
    // subtracting would rotate it counter-clockwise
    theta += PI/2.0;

    // convert from radians to degrees
    // this will give you an angle from [0->270],[-180,0]
    double rotation = theta * 180 / PI;

    // convert to positive range [0-360)
    // since we want to prevent negative angles, adjust them now.
    // we can assume that atan2 will not return a negative value
    // greater than one partial rotation
    if (rotation < 0) {
    	rotation += 360;
    }
    rotateToAngle(entityToRotate, rotation, dt);

}

void rotateToAngle(entityx::Entity& entityToRotate, double angle, double dt)
{
	auto draw = entityToRotate.component<Draw>();
	int diff = angle - draw->angle_;
	int inverseDiff = diff - 360;
	diff = std::abs(diff) < std::abs(inverseDiff) ? (int) diff : (int)inverseDiff;

	const double rotateVel = 150;

//    	t->ready_ = diff < 1 && diff > -1;

	if(diff > 0)
		draw->angle_ = draw->angle_ + rotateVel * dt;
	else if(diff < 0)
		draw->angle_ = draw->angle_ - rotateVel * dt;
}

std::vector<entityx::Entity> creepsInRange(entityx::EntityManager &entityManager, const Circle& range)
{
	std::vector<entityx::Entity> creepsFound;
	for(auto neighbor : entityManager.entities_with_components<Creep>())
	{
		SDL_Rect creepRect = getRect(neighbor);
		if(collidesWith(range, creepRect))
		{
			creepsFound.push_back(neighbor);
		}
	}
	return creepsFound;
}

std::string readFile(const std::string& filename)
{
    SDL_RWops *rw = SDL_RWFromFile(filename.c_str(), "r");
    if(rw == nullptr)
    {
        SDL_Log("Could not open file \"%s\".\n", filename.c_str());
        return "";
    }

	/* Seek to 0 bytes from the end of the file */
	Sint64 length = SDL_RWseek(rw, 0, RW_SEEK_END);
	if (length < 0)
	{
        SDL_Log("Could not seek inside \"%s\".\n", filename.c_str());
        return "";
	}

	SDL_RWseek(rw, 0, SEEK_SET);
	unsigned char* data = (unsigned char*)malloc(length);
	long total = 0;
	long len = 0;
	while((len = SDL_RWread(rw, data, 1, length)) > 0 && (total < length))
	{
	    total += len;
	}
	std::string ret((const char *)data, length);
//	SDL_Log("ARQUIVO LIDO: %s", ret.c_str());
	free(data);
	SDL_RWclose(rw);

	return ret;
}
