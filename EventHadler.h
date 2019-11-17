#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
#include <windows.h>

/*This is not an event handler per se. 
The isKeyDown property determines whether the key is currently pressed. 
The isKeyUp property determines whether the key was omitted at the time
of the last call to the Update function*/

typedef struct _KeyState
{
	char Key;
	bool isKeyDown;
	bool isKeyUp;
} KeyState;

class EventHandler
{
	KeyState KeysState[256];
public:
	EventHandler();
	bool isKeyDown(char Key);
	bool isKeyUp(char Key);
	void Update();
};

#endif

