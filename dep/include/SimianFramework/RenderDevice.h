#pragma once

class SimianBase;

class RenderDevice
{
public:
	RenderDevice(SimianBase * theApp);

	virtual void Init() = 0;
	virtual void Render() = 0;
	virtual void Cleanup() = 0;
	virtual void ChangeSize(int Width, int Height) = 0;
protected:
	SimianBase * mApp;
	int mWidth;
	int mHeight;
};