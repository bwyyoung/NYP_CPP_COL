#pragma once
#include "Macros.h"

class SIM_EXP Vector2D
{
protected:
	float mX;
	float mY;
public:
	Vector2D(float X = 0.0f, float Y = 0.0f);
	~Vector2D();

	float Magnitude();
	float Angle();

	Vector2D operator+(Vector2D a);
	Vector2D operator-(Vector2D a);
	Vector2D operator+(float a);
	Vector2D operator-(float a);
};