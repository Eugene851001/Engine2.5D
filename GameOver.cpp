#include "GameOver.h"

GameOver::GameOver()
{
	itemPointer = goipMainMenu;
}

GameState GameOver::Move(int direction)
{
	GameState gameState = gsGameOver;
	if (direction)
	{
		itemPointer = (GOItemPointer)!itemPointer;
	}
	else
	{
		if (!itemPointer)
			gameState = gsMainMenu;
		else
			gameState = gsQuit;
	}
	return gameState;
}

GOItemPointer GameOver::GetGOIP()
{
	return itemPointer;
}
