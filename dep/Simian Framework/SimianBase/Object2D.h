#pragma once
#include "Object.h"

class SIM_EXP Object2D : public Object
{
public:
	Object2D(float X = 0, float Y = 0, int Width = 0, int Height = 0);
	~Object2D();

	void SetPosition(float X, float Y);
	void SetX(float X);
	void SetY(float Y);
	void SetSize(int Width, int Height);
	void SetWidth(int Width);
	void SetHeight(int Height);

	float GetX();
	float GetY();
	int GetWidth();
	int GetHeight();
protected:
	float mX, mY;
	int mWidth, mHeight;
};