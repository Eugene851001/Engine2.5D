#ifndef ENEMY_SOLDIER_H
#define ENEMY_SOLDIER_H
#include "Enemy.h"

class EnemySoldier: public Enemy
{
public:
	EnemySoldier(float x, float y, float size): Enemy(x, y, size)
	{
		InitTexture();
	}
	//void Move(int plX, int plY, float time);
	void InitTexture(); 
};

#endif
