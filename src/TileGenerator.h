#pragma once;
#include "renderobject.h"

class Image;
class BaseApp;

class TileGenerator: public RenderObject
{
public:
	TileGenerator(BaseApp * theApp);
	~TileGenerator();

	void LoadTiles(char * ImagePath);
	void GenerateTiles(int SizeX, int SizeY);

	virtual void Render();
	virtual void Update();
	virtual void Render(float x, float y);

	friend class Minimap;
protected:
	int ** mTiles;
	int TileWidth, TileHeight;
	BaseApp * mApp;
	Image * mTileSheet;
};