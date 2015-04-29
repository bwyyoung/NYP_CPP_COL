#pragma once
#include <vector>

using namespace std;

class RenderObject;
class TileGenerator;
class BaseApp;
class Entity;

class ObjectManager
{
public:
	ObjectManager(BaseApp * theApp);
	~ObjectManager();

	void AddObject(RenderObject * theObject);
	void AddTiledMap(TileGenerator * theTiles);
	void Update();
	void UpdateObjects();
	void Render();
	void Select();

	friend class Minimap;
	friend class Toolbar;
	friend class Environment;
protected:
	BaseApp * mApp;
	int mScrollX, mScrollY;
	TileGenerator * mTiles;
	vector<RenderObject *> mObjects;

	bool PickingCreature;
};

extern Entity * mCurSelection;