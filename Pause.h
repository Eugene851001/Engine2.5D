#pragma once
#include "Engine.h"

const int PauseWidth = 12;
const int PauseHeight = 9;

enum ItemPausePointer {ippContinue, ippMainMenu, ippQuit, ippAmount};

class Pause
{
	ItemPausePointer itemPointer;
public:
	Pause();
	GameState Move(int direction);
	ItemPausePointer GetIPP();
};

