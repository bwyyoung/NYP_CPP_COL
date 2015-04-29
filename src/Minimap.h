#pragma once
#include "Panel.h"

class Sprite;
class BaseApp;
class Image;

class Minimap : public Panel
{
public:
	Minimap(BaseApp * theApp);
	~Minimap();

	virtual void Update();
	bool Updateb();
	virtual void Render();
	virtual void Render(float x, float y);

	int MINIMAP_GRASS;
	int MINIMAP_DIRT;
	int MINIMAP_PLANT;
	int MINIMAP_CHUCK;
	int MINIMAP_PREDATOR;
	int MINIMAP_PREY;
	int	MINIMAP_MOOSE;
protected:
	Sprite * mMinimapSprite;
	Image * mMinimapBG;
	Image * mCurView;

	bool MouseDown;
	bool Selected;
};