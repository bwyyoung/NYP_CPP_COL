#pragma once
#include "RenderDevice.h"
#include <d3d9.h>

class SimianBase;

class D3D9Device : public RenderDevice
{
public:
	D3D9Device(SimianBase * theApp);
	
	virtual void Init();
	virtual void Render();
	virtual void Cleanup();
	virtual void ChangeSize(int Width, int Height);
protected:
	LPDIRECT3D9 mD3D9;
	LPDIRECT3DDEVICE9 mD3DD9;
	D3DPRESENT_PARAMETERS d3dpp;
};

extern LPDIRECT3DDEVICE9 D3DD;