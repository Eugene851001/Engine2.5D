#include <cmath>
#include "Enemy.h"
#include "Engine.h"

Enemy::Enemy(float x, float y, float size)
{
	this->x = x;
	this->y = y;
	this->size = size;
	fspeed = 0.0005f;
	textureWidth = 4;
	textureHeight = 4;
	fViewRad = 5.0f;
	isDestroy = false;
	InitTexture();
}

void Enemy::InitTexture()
{
	char tempTexture[4][4] = { {'$', '$', '$', '$'},
							{' ', '$', '$', ' '},
							{'$', '$', '$', '$'},
							{'$', ' ', ' ', '$'} };
	int i;
	int j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			texture[i][j] = tempTexture[i][j];
		}
}

void Enemy::Move(float plX, float plY, float time, std::string &map)
{
	if (!isDestroy)
	{
		float distance = sqrt(pow(x - plX, 2) + pow(y - plY, 2));
		if (distance < fViewRad)
			if (distance > size + 0.2f)
			{
				float dx = (plX - x) / distance;
				float dy = (plY - y) / distance;
				float testX = x + dx * fspeed * time;
				float testY = y + dy * fspeed * time;
		//		if (map[testY * MapWidth + testX] != '#')
		//		{
					x = testX;
					y = testY;	
		//		}
			}
	}
}

extern player Player;

bool Enemy::operator < (const Enemy &en2)
{
	float dist1 = sqrt(pow(Player.fX - this->getX(), 2) + pow(Player.fY - this->getY(), 2));
	float dist2 = sqrt(pow(Player.fX - en2.getX(), 2) + pow(Player.fY - en2.getY(), 2));
	return dist1 < dist2;	
}

bool Enemy::operator>(Enemy &en2)
{
	float dist1 = sqrt(pow(Player.fX - this->getX(), 2) + pow(Player.fY - this->getY(), 2));
	float dist2 = sqrt(pow(Player.fX - en2.getX(), 2) + pow(Player.fY - en2.getY(), 2));
	return dist1 > dist2;	
}

bool Enemy::operator==(Enemy &en2)
{
	float dist1 = sqrt(pow(Player.fX - this->getX(), 2) + pow(Player.fY - this->getY(), 2));
	float dist2 = sqrt(pow(Player.fX - en2.getX(), 2) + pow(Player.fY - en2.getY(), 2));
	return dist1 == dist2;	
}

