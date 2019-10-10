#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
//	int HP = 20;
	float x, y;
	float fspeed;// = 0.0f;//0.0001f;
	float size;
	int textureWidth;// = 4;
	int textureHeight;// = 4;
	bool isDestroy;// = false;

public:

	char texture[4][4];/* = { {'$', '$', '$', '$'},
						   {' ', '$', '$', ' '},
						   {'$', '$', '$', '$'},
						   {'$', ' ', ' ', '$'} };*/
	Enemy(float x, float y, float size);
	
	void Move(int plX, int plY, float time);
	
	void InitTexture(); 
	float getX()
	{
		return x;
	}
	float getY()
	{
		return y;
	}
	int getTextureWidth()
	{
		return textureWidth;
	}

	int getTextureHeight()
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
