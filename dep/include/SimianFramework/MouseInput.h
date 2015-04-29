#pragma once
#include "Macros.h"

enum Mouse_Button
{
	LBUTTON = 0x01, MBUTTON = 0x04, RBUTTON = 0x02
};

class SIM_EXP MouseInput
{
public:
	MouseInput();
	~MouseInput();

	void GetMousePosition(int * X, int * Y);
	bool GetButtonDown(Mouse_Button Button);
	void Update(int X, int Y);

	int GetMouseX();
	int GetMouseY();
protected:
	int mX, mY;
};