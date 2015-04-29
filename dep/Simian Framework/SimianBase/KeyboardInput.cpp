#include "KeyboardInput.h"
#include <Windows.h>

KeyboardInput::KeyboardInput()
{
	for (int i = 0; i < 256; i ++)
		mKeyMap[i] = false;
}

KeyboardInput::~KeyboardInput()
{

}

void KeyboardInput::Update()
{
	// Update the key map..
	for (int i = 0; i < 256; i ++)
		if (GetAsyncKeyState(i))
			mKeyMap[i] = true;
		else
			mKeyMap[i] = false;
}

bool KeyboardInput::GetKeyDown(int KeyCode)
{
	if (KeyCode > 256)
	{
		if (GetAsyncKeyState(KeyCode))
			return true;
		else
			return false;
	}
	else
		return mKeyMap[KeyCode];
}

void KeyboardInput::GetKeyMap(bool *KeyMap)
{
	//prevent shallow copying..
	for (int i = 0; i < 256; i ++)
		KeyMap[i] = mKeyMap[i];
}