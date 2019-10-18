#ifndef BULLET_H
#define BULLET_H
#include "Enemy.h"
#include <iostream>
#include <list>

class Bullet
{
	int Damage;
	float x, y;
	float fspeed;
	float dx, dy;
	bool isDestroy;

public:
	
	Bullet(float x, float y, float dx, float dy);
	void Move(const std::string& Map, float time, std::list<Enemy*> enemyes);

	float getX()
	{
		return x;
	}

	float getY()
	{
		return y;
	}

	bool getDestroy()
	{
		return isDestroy;
	}

	void setDestroy(bool value)
	{
		isDestroy = value;
	}
};

#endif
