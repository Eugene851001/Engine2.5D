#include "EventHadler.h"

EventHandler::EventHandler()
{
	unsigned char i;
	for (i = 0; i < 255; i++)
	{
		KeysState[i].isKeyDown = false;
		KeysState[i].isKeyUp = false;
		KeysState[i].Key = i;
	}
}

bool EventHandler::isKeyDown(char Key)
{
	return KeysState[Key].isKeyDown;
}

bool EventHandler::isKeyUp(char Key)
{
	return KeysState[Key].isKeyUp;
}


void EventHandler::Update()
{
	unsigned char i;
	for (i = 0; i < 255; i++)
		if (GetAsyncKeyState(KeysState[i].Key))
		{
			KeysState[i].isKeyDown = true;
			KeysState[i].isKeyUp = false;
		}
		else
		{
			if (KeysState[i].isKeyDown)
				KeysState[i].isKeyUp = true;
			else
				KeysState[i].isKeyUp = false;
			KeysState[i].isKeyDown = false;
		}
}