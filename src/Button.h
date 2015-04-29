#pragma once
#include "RenderObject.h"

class BaseApp;
class Image;

class Button : public RenderObject
{
public:
	Button(BaseApp * theApp, Image * UINormal, Image * UIDown);
	~Button();

	virtual void	Update();
	virtual int		Updateb();
	virtual void	Render(float x, float y);
	virtual void	Render();
protected:
	BaseApp *		mApp;
	Image *			mUIPicture;
	Image *			mUIDownPicture;
	bool			mButtonDown;
	bool			mMouseDown;
};