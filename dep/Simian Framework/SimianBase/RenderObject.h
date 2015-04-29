#pragma once
#include "Object2D.h"
#include "Macros.h"

class SIM_EXP RenderObject : public Object2D
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Render(float x, float y) = 0;
};