#include <cmath>
#include "Enemy.h"
#include "Engine.h"

Enemy::Enemy(float x, float y, float size)
{
	this->x = x;
	this->y = y;
	this->size = size;
	fspeed = 0.0005f;
	textureWidth = 11;
	textureHeight = 8;
	fViewRad = 3.0f;
	isDestroy = false;
	tmLastFrame = 0.0f;
	tmFramePeriod = 1000.0f;
	FrameAmount = 2;
	InitTexture();
	InitFrames();
}

void Enemy::InitTexture() 
{
	texture = new char*[textureHeight];
	for (int i = 0; i < textureWidth; i++)
		texture[i] = new char[textureWidth];
	char tempTexture[8][11] = { {' ', ' ', '$', ' ', ' ', ' ', ' ', ' ', '$', ' ', ' '},
								{' ', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', ' '},
								{' ', ' ', '$', '$', '$', '$', '$', '$', '$', ' ', ' '},
								{' ', '$', '$', '0', '$', '$', '$', '0', '&', '$', ' '}, 
								{'$', '$', '$', '$', '$', '$', '$', '$', '$', '$', ' '},
								{'$', ' ', '$', '$', '$', '$', '$', '$', '$', ' ', '$'},
								{'$', ' ', '$', ' ', ' ', ' ', ' ', ' ', '$', ' ', '$'},
								{' ', ' ', ' ', '$', '$', ' ', '$', '$', ' ', ' ', ' '} };
	for (int i = 0; i < textureHeight; i++)
		for (int j = 0; j < textureWidth; j++)
		{
			texture[i][j] = tempTexture[i][j];
		}
}

void Enemy::InitFrames()
{
	Frames = new char**[2];
	for (int i = 0; i < 2; i++)
	{
		Frames[i] = new char*[textureHeight];
		for (int j = 0; j < textureHeight; j++)
		{
			Frames[i][j] = new char[textureWidth];
		}
	}
	char TempFrame[8][11] = { {' ', ' ', '$', ' ', ' ', ' ', ' ', ' ', '$', ' ', ' '},
								{' ', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', ' '},
								{' ', ' ', '$', '$', '$', '$', '$', '$', '$', ' ', ' '},
								{' ', '$', '$', '0', '$', '$', '$', '0', '&', '$', ' '},
								{'$', '$', '$', '$', '$', '$', '$', '$', '$', '$', ' '},
								{'$', ' ', '$', '$', '$', '$', '$', '$', '$', ' ', '$'},
								{'$', ' ', '$', ' ', ' ', ' ', ' ', ' ', '$', ' ', '$'},
								{' ', ' ', ' ', '$', '$', ' ', '$', '$', ' ', ' ', ' '} };
	for (int i = 0; i < textureHeight; i++)
		for (int j = 0; j < textureWidth; j++)
			Frames[0][i][j] = TempFrame[i][j];
	char TempFrame2[8][11] = { {' ', ' ', ' ', ' ', '$', ' ', '$', ' ', ' ', ' ', ' '},
								{' ', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', ' '},
								{'$', ' ', '$', '$', '$', '$', '$', '$', '$', ' ', '$'},
								{'$', '$', '$', '0', '$', '$', '$', '0', '&', '$', '$'},
								{'$', '$', '$', '$', '$', '$', '$', '$', '$', '$', '$'},
								{' ', ' ', '$', '$', '$', '$', '$', '$', '$', ' ', ' '},
								{' ', ' ', '$', ' ', ' ', ' ', ' ', ' ', '$', ' ', ' '},
								{' ', ' ', ' ', '$', '$', ' ', '$', '$', ' ', ' ', ' '} };
	for (int i = 0; i < textureHeight; i++)
		for (int j = 0; j < textureWidth; j++)
			Frames[1][i][j] = TempFrame2[i][j];
}

void Enemy::Animate(float time)
{
	if (tmLastFrame > tmFramePeriod)
	{
		if (++FrameNum >= FrameAmount)
		{
			FrameNum = 0;
		}
		texture = Frames[FrameNum];
		tmFramePeriod = 0.0f;
	}
	else
		tmLastFrame += time;
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
		Animate(time);
	}
}

extern Player player;

bool Enemy::operator < (const Enemy &en2) const
{
	float dist1 = sqrt(pow(player.fX - this->getX(), 2) + pow(player.fY - this->getY(), 2));
	float dist2 = sqrt(pow(player.fX - en2.getX(), 2) + pow(player.fY - en2.getY(), 2));
	return dist1 < dist2;	
}

bool Enemy::operator>(const Enemy &en2) const
{
	float dist1 = sqrt(pow(player.fX - this->getX(), 2) + pow(player.fY - this->getY(), 2));
	float dist2 = sqrt(pow(player.fX - en2.getX(), 2) + pow(player.fY - en2.getY(), 2));
	return dist1 > dist2;	
}

bool Enemy::operator==(const Enemy &en2) const
{
	float dist1 = sqrt(pow(player.fX - this->getX(), 2) + pow(player.fY - this->getY(), 2));
	float dist2 = sqrt(pow(player.fX - en2.getX(), 2) + pow(player.fY - en2.getY(), 2));
	return dist1 == dist2;	
}

