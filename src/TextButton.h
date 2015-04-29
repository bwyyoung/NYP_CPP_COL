#pragma once
#include "Button.h"
#include <string>

using namespace std;

class BitmappedFont;

class TextButton : public Button
{
public:
	TextButton(BaseApp * theApp, char * ButtonText, int FontSize);
	~TextButton();

	virtual void Render();
	virtual void Render(float x, float y);
protected:
	int mFontSize;
	string mButtonText;
	BitmappedFont * mFont;
	Image * mFontTexture;
};