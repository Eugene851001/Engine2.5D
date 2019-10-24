#ifndef ENEMY_SOLDIER_H
#define ENEMY_SOLDIER_H
#include "Bullet.h"
#include "Enemy.h"

//extern std::list<Bullet*> bullets;

class EnemySoldier: public Enemy
{
protected:
	float tmAfterShoot, tmReload; 
	void Attack(float plX, float plY, float time);
public:
	EnemySoldier(float x, float y, float size): Enemy(x, y, size)
	{
		tmAfterShoot = 1000.0f;
		tmReload = 1000.0f;
		textureHeight = 4;
		textureWidth = 4;
		InitTexture();
	}
	void Move(float plX, float plY, float time, std::string &map);
	void InitTexture(); 
	void InitFrames();
};

#endif
