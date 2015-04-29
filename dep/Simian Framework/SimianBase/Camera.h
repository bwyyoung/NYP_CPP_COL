// Camera class to easily set up a camera for:
// * 2D Orthogonal Mode
// * FPS Camera
// * Third Person Camera
// * Manual Camera
#pragma once
#include "Object3D.h"

class SIM_EXP Camera : public Object3D
{
public:
	Camera(float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
	~Camera();

	// Switches to last 3D rendering mode
	virtual void Set3DMode() = 0;
	// Switches to 2D mode
	virtual void Set2DMode() = 0;
protected:

};