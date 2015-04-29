#pragma once
#include "Macros.h"

#define KEY_DOWN true;
#define KEY_UP false;

class SIM_EXP KeyboardInput
{
public:
	KeyboardInput();
	~KeyboardInput();

	void Update();
	bool GetKeyDown(int KeyCode);
	void GetKeyMap(bool * KeyMap);
protected:
	bool mKeyMap[256];
};