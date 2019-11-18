#include "Bullet.h"
//#include "Enemy.h"//replace with 'object'

Bullet::Bullet(float x, float y, float dx, float dy)
{
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	Damage = 5;
	fspeed = 0.002f;
	isDestroy = false;
}

void Bullet::Move(const std::string& Map, float time, std::list<Enemy*> enemyes)
{
	if (!isDestroy)
	{
		x += (float)(dx * fspeed * time);
		y += (float)(dy * fspeed * time);
		if (x > MapWidth && y > MapHeigth)
			isDestroy = true;
		else
			if (Map[int(y) * MapWidth + int(x)] == '#')
				isDestroy = true;
	}
}
