#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
const char EMPTY = ' ';

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
	int FrameNum, FrameAmount;
	float tmLastFrame, tmFramePeriod;
	char ***Frames;
	virtual void InitFrames();
	void Animate(float time);
public:
	bool operator==(const Enemy &en2) const; 
	bool operator>=(const Enemy &en2)const;
	bool operator < (const Enemy &en2)const;
	bool operator>(const Enemy &en2)const;
	char **texture;/* = { {'$', '$', '$', '$'},
						   {' ', '$', '$', ' '},
						   {'$', '$', '$', '$'},
						   {'$', ' ', ' ', '$'} };*/
	Enemy(float x, float y, float size);
	
	virtual void Move(float plX, float plY, float time, std::string &map);
	
	virtual void InitTexture(); 
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
