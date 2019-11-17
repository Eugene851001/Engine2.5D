#pragma once
#include "Engine.h"
#include <iostream>
#define DOWN -1
#define UP 1
#define IN 0
const int MenuWidth = 12;
const int MenuHeight = 9;

enum ItemPointer {ipStart, ipLevels, ipOptions, ipQuit, ipAmount};

class Menu
{
	ItemPointer itemPointer;
	std::string map;
public:
	Menu(std::string &map);
	GameState Move(int dir);
	ItemPointer GetIP();
};

