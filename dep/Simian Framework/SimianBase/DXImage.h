#pragma once
#include "Macros.h"
#include "Image.h"
#include <d3d9.h>

class SIM_EXP DXImage:public Image
{
public:
	DXImage();
	~DXImage();

	virtual bool Load(char * thePath);
	virtual void Render();
	virtual void Render(int SrcX, int SrcY, int SrcWidth, int SrcHeight);
	virtual void Render(float x, float y);
	virtual void Render(float x, float y, int SrcX, int SrcY, int SrcWidth, int SrcHeight);
	virtual void Update();
protected:
	LPDIRECT3DTEXTURE9 mTex;
};