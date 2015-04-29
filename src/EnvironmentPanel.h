#pragma once
#include "panel.h"
#include "PointerButton.h"

class Image;
class BaseApp;

class EnvironmentPanel : public Panel
{
public:
	EnvironmentPanel(BaseApp * theApp);
	~EnvironmentPanel();

	virtual void Render();
	virtual void Render(float x, float y);
	virtual void	Update();
	bool			Updateb();
protected:
	Image * mBGImage;
	PointerButton * mSunnyButton;
	PointerButton * mCloudyButton;
	PointerButton * mDarkButton;
	PointerButton * mRainButton;
	PointerButton * mNightButton;

};