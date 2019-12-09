#pragma once
#include <iostream>
#include "Engine.h"


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
	int ObjectsNum;//кол-во объектов на уровне
	Object* objects;//казатель на массив объектов
	int MapWidth, MapHeight;//высота и ширина карты
	std::string map;//сама карта
	float PlayerX, PlayerY;//координаты, в которых появляется игрок при загрузке карты 
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

