#include "Records.h"

Records::Records()
{

}


Record Records::GetRecord(int n)
{
	if (n < 0 || n >= MaxLevelNum)
		n = 0;
	return values[n];
}

void Records::SetDefaultRecords()
{
	for (int i = 0; i < MaxLevelNum; i++)
	{
		values[i] = defaultRecord;
	}
}

GameState Records::Move(int direction)
{
	GameState gameState = gsShowRecords;
	if (direction == BACK)
	{
		gameState = gsMainMenu;
	}
	return gameState;
}

void Records::SetRecord(int n, Record value)
{
	if (n < 0 || n >= MaxLevelNum)
		n = 0;
	values[n] = value;
}