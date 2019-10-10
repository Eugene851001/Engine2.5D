#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <Windows.h>
#define PI 3.14159265
#define BULLET_SHADE '@'
#define OUTPUT "Photo.txt"
#define max(a, b) ((a) > (b)) ? a : b

void makeScreenShoot(char* screen);
const int ScreenWidth = 120;
const int ScreenHeight = 40;
const int MapWidth = 16;
const int MapHeigth = 16;

const float fViewAngle = PI / 4.0f;
const float fViewRad = 10.0f;
const float fDepth = 16.0f;

typedef struct MyPlayer
{
	float fX;
	float fY;// = 8.0f;
	float fAngle;// = 0.0f;
	float fSpeed;// = 0.001f;
	float fAngleSpeed;// = 0.001f;
	float tmReload;// = 1000.0f;
	float tmAfterShoot;// = 1000.0f;
	MyPlayer()
	{
		fX = 8.0f;
		fY = 8.0f;
		fAngle = 0.0f;
		fSpeed = 0.001f;
		fAngleSpeed = 0.001f;
		tmReload = 1000.0f;
		tmAfterShoot = 1000.0f;		
	}
} player;

#endif

