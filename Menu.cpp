#include "Menu.h"

Menu::Menu(std::string &map)
{
	this->map = map;
	itemPointer = ipStart;
}

GameState Menu::Move(int dir)
{
	GameState gameState = gsMainMenu;
	if (dir)
	{
		itemPointer = (ItemPointer)((itemPointer + dir + ipAmount) % ipAmount);
	}
	else
	{
		switch (itemPointer)
		{
		case ipStart:
			gameState = gsRun;
			break;
		case ipQuit:
			gameState = gsQuit;
			break;
		case ipLevels:
			gameState = gsChooseLevel;
			break;
		default:
			break;
		}
	}
	return gameState;
}

ItemPointer Menu::GetIP()
{
	return itemPointer;
}