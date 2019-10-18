#ifndef ENEMY_SOLDIER_H
#define ENEMY_SOLDIER_H
#include "Enemy.h"
#include "Bullet.h"

extern std::list<Bullet*> bullets;

//std::list<Bullet*> EnBullets;

class EnemySoldier: public Enemy
{
protected:
	float tmAfterShoot, tmReload; 
	void Attack(float plX, float plY, float time);
	
public:
	EnemySoldier(float x, float y, float size): Enemy(x, y, size)
	{
		tmAfterShoot = 1000.0f;
		InitTexture();
	}
	void Move(float plX, float plY, float time, std::string &map);
	void InitTexture(); 
};

#endif
