#pragma once
#include <iostream>
#include "Engine.h"
#define BACK 2

const int LvlMenuWidth = 12;
const int LvlMenuHeight = 9;

enum ObjectType {otEnemy, otEnemySoldier};


typedef struct _Object
{
	ObjectType type;
	float x, y;
} Object;

typedef struct _Level
{
	int ObjectsNum;
	Object* objects;
	int MapWidth, MapHeight;
	std::string map;
	float PlayerX, PlayerY;
} Level;

class Levels
{
	const int MaxLevel = 3;
	int LevelNum;
	Level *LevelList;
public:
	Levels();
	int GetLevelNum();
	int GetMaxLevel();
	Level GetLevel();
	GameState Move(int direction);
};

