#pragma once
#include "panel.h"

class BaseApp;
class BitmappedFont;
class Entity;
class Image;
class PointerButton;

class Disaster : public Panel
{
public:
	Disaster(BaseApp * theApp);
	~Disaster();

	virtual void	Render(float x, float y);
	virtual void	Render();
	virtual void	Update();
	bool			Updateb();

private:
	BitmappedFont * mFont;
	Image *			mBGImage;
};