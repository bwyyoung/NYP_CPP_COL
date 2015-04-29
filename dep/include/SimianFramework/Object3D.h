#pragma once
#include "Object.h"

class SIM_EXP Object3D : public Object
{
public:
	Object3D(float theX = 0, float theY = 0, float theZ = 0);
	~Object3D();

	void SetX(float theX);
	void SetY(float theY);
	void SetZ(float theZ);
	void SetPosition(float theX, float theY, float theZ);

	float GetX();
	float GetY();
	float GetZ();
protected:
	float mX, mY, mZ;
};