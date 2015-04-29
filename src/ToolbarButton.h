#pragma once
#include "Button.h"

class BaseApp;
class Sprite;

class ToolbarButton : public Button
{
public:
	ToolbarButton(BaseApp * theApp, Sprite * Pic);
	~ToolbarButton();

	int Updateb();
	virtual void Render();
	virtual void Render(float x, float y);
protected:
	Sprite * mPic;
};