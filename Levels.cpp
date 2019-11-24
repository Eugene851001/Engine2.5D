#include "Levels.h"

Levels::Levels()
{
	LevelNum = 0;
	LevelList = new Level[MaxLevel];
	LevelList[0].ObjectsNum = 2;
	LevelList[0].objects = new Object[2];
	LevelList[0].objects[0] = { otEnemy, 1.0f, 1.0f };
	LevelList[0].objects[1] = { otEnemySoldier,  3.0f, 4.0f };
	LevelList[0].MapWidth = 7;
	LevelList[0].MapHeight = 9;
	LevelList[0].PlayerX = 1;
	LevelList[0].PlayerY = 7;
	LevelList[0].map += "#######";
	LevelList[0].map += "#     #";
	LevelList[0].map += "# ### #";
	LevelList[0].map += "# # # #";
	LevelList[0].map += "# #   #";
	LevelList[0].map += "# # # #";
	LevelList[0].map += "# ### #";
	LevelList[0].map += "#     #";
	LevelList[0].map += "#######";
	LevelList[1].ObjectsNum = 0;
	LevelList[1].MapHeight = 5;
	LevelList[1].MapWidth = 5;
	LevelList[1].map += "#####";
	LevelList[1].map += "#   #";
	LevelList[1].map += "#   #";
	LevelList[1].map += "#   #";
	LevelList[1].map += "#####";
	LevelList[1].PlayerX = 2;
	LevelList[1].PlayerY = 2;	
}

Level Levels::GetLevel()
{
	return LevelList[LevelNum];
}

int Levels::GetLevelNum()
{
	return LevelNum;
}

GameState Levels::Move(int direction)
{
	if (direction)
	{
		LevelNum = (LevelNum + MaxLevel + direction) % MaxLevel;
		return gsChooseLevel;
	}
	else
		return gsLoadLevel;
}

int Levels::GetMaxLevel()
{
	return MaxLevel;
}
