#include <cmath>
#include "Enemy.h"
//#include "Engine.h"

Enemy::Enemy(float x, float y, float size)
{
	this->x = x;
	this->y = y;
	this->size = size;
	fspeed = 0.0f;//0.0001f;
	textureWidth = 4;
	textureHeight = 4;
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

void Enemy::Move(int plX, int plY, float time)
	{
		if (!isDestroy)
		{
			float distance = sqrt(pow(x - plX, 2) + pow(y - plY, 2));
			if (distance > size + 0.2f)
			{
				float dx = (plX - x) / distance;
				float dy = (plY - y) / distance;
				x += dx * fspeed * time;
				y += dy * fspeed * time;
			}
		}
	}

/*bool operator<(Enemy& en1, Enemy& en2)
{
	float dist1 = sqrt(pow(Player.fX - en1.getX(), 2) + pow(Player.fY - en1.getY(), 2));
	float dist2 = sqrt(pow(Player.fX - en2.getX(), 2) + pow(Player.fY - en2.getY(), 2));
	return dist1 < dist2;
}

int operator>(Enemy& en1, Enemy& en2)
{
	float dist1 = sqrt(pow(Player.fX - en1.getX(), 2) + pow(Player.fY - en1.getY(), 2));
	float dist2 = sqrt(pow(Player.fX - en2.getX(), 2) + pow(Player.fY - en2.getY(), 2));
	return dist1 > dist2;
}

int operator==(Enemy& en1, Enemy& en2)
{
	float dist1 = sqrt(pow(Player.fX - en1.getX(), 2) + pow(Player.fY - en1.getY(), 2));
	float dist2 = sqrt(pow(Player.fX - en2.getX(), 2) + pow(Player.fY - en2.getY(), 2));
	return dist1 == dist2;
}*/

