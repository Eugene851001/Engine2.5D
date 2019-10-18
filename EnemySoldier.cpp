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

void EnemySoldier::Attack(float plX, float plY, float time)
{
	if (tmAfterShoot > tmReload)
	{
		float distance = sqrt(pow(plX - x, 2) + pow(plY - y, 2));
		float dx = (plX - x) / distance;
		float dy = (plY - y) / distance;
//		EnBullets.push_back(new Bullet(x, y, dx, dy));
//		bullets.push_back(new Bullet(x, y, dx, dy));
//		bullets.push_back(new Bullet(x, y, dx, dy));	
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
			Attack(plX, plY, time);
		}	
	}
}
