#pragma once
#include "Image.h"

class SIM_EXP GLImage : Image
{
public:
	GLImage();
	~GLImage();

	virtual bool Load(char * thePath);
	bool LoadUnfiltered(char * thePath);
	virtual void Render();
	virtual void Render(int SrcX, int SrcY, int SrcWidth, int SrcHeight);
	virtual void Render(float x, float y);
	virtual void Render(float x, float y, int SrcX, int SrcY, int SrcWidth, int SrcHeight);
	virtual void Update();
protected:
	unsigned int mId;
	int mOriginalWidth;
	int mOriginalHeight;
};