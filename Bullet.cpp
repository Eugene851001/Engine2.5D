#include "Bullet.h"
//#include "Enemy.h"//replace with 'object'
#include "Engine.h"

Bullet::Bullet(float x, float y, float dx, float dy)
{
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	fspeed = 0.002f;
	isDestroy = false;
}

void Bullet::Move(const std::string& Map, float time, std::list<Enemy*> enemyes)
{
	if (!isDestroy)
	{
		x += dx * fspeed * time;
		y += dy * fspeed * time;
		if (Map[int(y) * MapWidth + int(x)] == '#')
			isDestroy = true;
		if (enemyes.size() > 0)
		{
			std::list<Enemy*>::iterator pEnemy = enemyes.begin();
			int size = (*pEnemy)->getSize();
			while (pEnemy != enemyes.end())
			{
				if (x > (*pEnemy)->getX() - size && x < (*pEnemy)->getX() + size
					&& y >(*pEnemy)->getY() - size && y < (*pEnemy)->getY() + size)
					isDestroy = true;
				pEnemy++;
			}
		}
	}
}
