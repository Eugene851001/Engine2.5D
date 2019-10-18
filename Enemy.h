#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>

class Enemy
{
//	int HP = 20;
protected:
	float x, y;
	float fspeed;// = 0.0f;//0.0001f;
	float size;
	float fViewRad;
	int textureWidth;// = 4;
	int textureHeight;// = 4;
	bool isDestroy;// = false;
	bool operator < (const Enemy &en2);
	bool operator>(Enemy &en2);
	bool operator==(Enemy &en2);
	bool operator>=(Enemy &en2);
public:

	char texture[4][4];/* = { {'$', '$', '$', '$'},
						   {' ', '$', '$', ' '},
						   {'$', '$', '$', '$'},
						   {'$', ' ', ' ', '$'} };*/
	Enemy(float x, float y, float size);
	
	virtual void Move(float plX, float plY, float time, std::string &map);
	
	void InitTexture(); 
	float getX() const
	{
		return x;
	}
	float getY() const
	{
		return y;
	}
	int getTextureWidth() const
	{
		return textureWidth;
	}

	int getTextureHeight() const
	{
		return textureHeight;
	}

	void setDestroy(bool value)
	{
		isDestroy = value;
	}

	bool getDestroy()
	{
		return isDestroy;
	}

	float getSize()
	{
		return size;
	}
};

#endif
