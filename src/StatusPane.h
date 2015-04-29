#pragma once
#include "panel.h"

class BaseApp;
class BitmappedFont;
class Entity;
class Image;
class PointerButton;

class StatusPane : public Panel
{
public:
	StatusPane(BaseApp * theApp);
	~StatusPane();

	virtual void	Render(float x, float y);
	virtual void	Render();
	virtual void	Update();
	bool			Updateb();

	void			SetEntity(Entity * theEntity);
protected:
	BitmappedFont * mFont;
	Image *			mBGImage;
	Entity *		mCurEntity;

	Image *			mChuckNorris;
	Image *			mWolf;
	Image *			mDeer;
	Image *			mMoose;
	Image *			mHungerBar;
	Image *			mHungerBarBack;

	PointerButton * mPointerButton;
	PointerButton * mGrabberButton;
};