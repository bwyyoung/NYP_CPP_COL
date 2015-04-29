#include "MouseInput.h"
#include <windows.h>

MouseInput::MouseInput()
{
	mX = mY = 0;
}

MouseInput::~MouseInput()
{

}

void MouseInput::Update(int X, int Y)
{
	mX = X;
	mY = Y;
}

void MouseInput::GetMousePosition(int * X, int * Y)
{	
	*X = mX;
	*Y = mY;
}

bool MouseInput::GetButtonDown(Mouse_Button Button)
{
	if (GetAsyncKeyState((int)Button))
		return true;
	else
		return false;
}

int MouseInput::GetMouseX()
{
	return mX;
}

int MouseInput::GetMouseY()
{
	return mY;
}