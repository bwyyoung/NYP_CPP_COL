#pragma once
#include <string>
#include "Macros.h"
#include "RenderObject.h"
#include "Color.h"

using namespace std;

enum Image_Type
{
	IMAGE_PNG, IMAGE_GIF, IMAGE_JPG
};

class SIM_EXP Image : public RenderObject
{
public:
	Image();
	~Image();

	virtual bool Load(char * thePath);
	virtual void Render() = 0;
	virtual void Render(int SrcX, int SrcY, int SrcWidth, int SrcHeight) = 0;
	virtual void Render(float x, float y) = 0;
	virtual void Render(float x, float y, int SrcX, int SrcY, int SrcWidth, int SrcHeight) = 0;
	virtual void Update() = 0;

	void SetColor(unsigned char R = 255, unsigned char G = 255, unsigned char B = 255, unsigned char A = 255);
	void SetColor(Color theColor);
	
	int GetActualWidth();
	int GetActualHeight();
	Color GetColor();
protected:
	Color mColor;
	Image_Type mType;
	string mPath;
	int mActualWidth, mActualHeight;
};