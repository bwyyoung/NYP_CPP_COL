#pragma once
#include "State.h"

class BaseApp;
class PlaceHolder;
class ObjectManager;
class TileGenerator;
class Image;
class TextButton;
class Camera;

class TitleState : public State
{
public:
	TitleState(BaseApp * theApp);
	~TitleState();

	virtual void	Init();
	virtual void	Render();
	virtual void	Update();
protected:
	Image *			mGrassSprite;
	Image *			mPreySprite;
	Image *			mPredatorSprite;
	Image *			mLogo;
	Image *			mBackground;

	TextButton *	mNewGame;
	TextButton *	mIdealScene;
	TextButton *	mExit;

	int				mAppearColour;
	int				mWaitTimer;

	Camera *		mCamera;

	ObjectManager *	mObjectManager;
	PlaceHolder **	mPlaceHolders;
	TileGenerator *	mTileGenerator;
};