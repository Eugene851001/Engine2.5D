#include "Pause.h"

Pause::Pause()
{
	itemPointer = ippContinue;
}

GameState Pause::Move(int direction)
{
	GameState gameState = gsPause;
	if (direction)
	{
		itemPointer = (ItemPausePointer)((itemPointer + direction + ippAmount) % ippAmount);
	}
	else
	{
		switch (itemPointer)
		{
		case ippMainMenu:
			gameState = gsMainMenu;
			break;
		case ippContinue:
			gameState = gsRun;
			break;
		case ippQuit:
			gameState = gsQuit;
			break;
		}
	}
	return gameState;
}

ItemPausePointer Pause::GetIPP()
{
	return itemPointer;
}