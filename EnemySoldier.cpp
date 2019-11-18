#include "EnemySoldier.h"
#include <cmath>

void EnemySoldier::InitTexture()
{
	char tempTexture[4][4] = {  {'$', '$', '$', '$'},
								{'$', ' ', ' ', '$'},
								{'$', ' ', ' ', '$'},
								{'$', '$', '$', '$'} };
	int i;
	int j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			texture[i][j] = tempTexture[i][j];
		}
}

void EnemySoldier::InitFrames()
{

}

extern std::list<Bullet> bullets;

void EnemySoldier::Attack(float plX, float plY, float time)
{
	if (tmAfterShoot > tmReload)
	{
		float distance = sqrt(pow(plX - x, 2) + pow(plY - y, 2));
		float dx = (plX - x) / distance;
		float dy = (plY - y) / distance;
		//Bullet *myBullet = new Bullet(x, y, dx, dy);
		bullets.push_back(Bullet(x + (size + 0.1f) * dx, 
			y + (size + 0.1f) * dy, dx, dy));
		tmAfterShoot = 0;	
	}
	else
		tmAfterShoot += time;
}

void EnemySoldier::Move(float plX, float plY, float time, std::string &map)
{
	if (!isDestroy)
	{
		float distance = sqrt(pow(plX - x, 2) + pow(plY - y, 2));
		if ((distance < fViewRad) && (distance > size + 0.2f))
		{
			//x = y = 2;
		//	Attack(plX, plY, time);
		}	
	}
}
