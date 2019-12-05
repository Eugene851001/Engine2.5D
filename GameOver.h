#pragma once
#include "Engine.h";

const int GameOverHeight = 12;
const int GameOverWidth = 20;

enum GOItemPointer{goipMainMenu, goipQuit};

class GameOver
{
	GOItemPointer itemPointer;
public:
	GameOver();
	GameState Move(int direction);
	GOItemPointer GetGOIP();
};

