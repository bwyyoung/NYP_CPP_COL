#pragma once
#include "Macros.h"
#include "Camera.h"

class SIM_EXP DXCamera : public Camera
{
public:
	DXCamera();
	~DXCamera();

	virtual void Set3DMode();
	virtual void Set2DMode();
};