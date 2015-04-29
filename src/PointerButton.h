#pragma once
#include "Button.h"

class PointerButton : public Button
{
public:
	PointerButton(BaseApp * theApp, Image * ButtonPic);
	~PointerButton();
	virtual void Render(float x, float y);
	virtual void Render();
protected:
	Image * mPointerPic;
};