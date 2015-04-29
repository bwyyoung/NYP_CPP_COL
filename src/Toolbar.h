#pragma once
#include "Panel.h"

class BaseApp;
class Sprite;
class ToolbarButton;
class Image;
class Entity;

class Toolbar : public Panel
{
public:
	Toolbar(BaseApp * theApp);
	~Toolbar();

	virtual void		Update();
	bool				Updateb();
	virtual void		Render();
	virtual void		Render(float x, float y);

protected:
	Sprite *			mChuckNorrisPic;
	Sprite *			mPredatorPic;
	Sprite *			mPreyPic;
	Sprite *			mPlantPic;
	Sprite *			mMoosePic;

	ToolbarButton *		mChuckButton;
	ToolbarButton *		mPredatorButton;
	ToolbarButton *		mPreyButton;
	ToolbarButton *		mPlantButton;
	ToolbarButton *		mMooseButton;

	Image *				mToolbarBG;

	Sprite *			mCurDrop;
};