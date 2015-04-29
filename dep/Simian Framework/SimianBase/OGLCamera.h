#pragma once
#include "Camera.h"

class OGLCamera : public Camera
{
public:
	OGLCamera(float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
	~OGLCamera();

	virtual void Set3DMode();
	virtual void Set2DMode();
protected:
};