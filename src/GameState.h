#pragma once
#include "State.h"
#include <Windows.h>
#include <map>

class BaseApp;
class Image;
class Predator;
class Prey;
class Grass;
class Chuck_Norris;
class Entity;
class Camera;
class TileGenerator;
class ObjectManager;
class Minimap;
class StatusPane;
class Toolbar;
class Moose;
class MessageBoard;
class EnvironmentPanel;
class Environment;
class Disaster;

using namespace std;

struct ImageCursor
{
	Image * Img;
	int X;
	int Y;
};

class GameState : public State
{
public:
	GameState(BaseApp * theApp);
	~GameState();

	virtual void Init();
	virtual void Render();
	virtual void Update();

	Predator ** mPredators;
	Prey  **mPrey;
	Chuck_Norris **mChucklets;
	Grass **mGrass;
	Moose **mMoose;
	Entity **mAllTargets;
	Entity **NewPrey;
	Environment * mEnvironment;
	float TimePassed;
	float Duration;
	bool DEBUG;
	int NumPredators;
	int NumPrey;
	int NumChucks;
	int NumMoose;
	BaseApp *mApp;

	int Mode;

	Image * CreateImage(char * thePath, char * theName);
	map<string, Image *> GetImageDatabase();

	friend class Minimap;
	friend class ObjectManager;
	friend class Toolbar;
	friend class Environment;
protected:
	map<string, Image *> mImageDatabase;

	ObjectManager * mObjectManager;

	Camera * mCamera;
	TileGenerator * mTileGenerator;

	Minimap * mMinimap;

	friend class Entity;
	friend class RenderObject;

	//HW Cursors
	HCURSOR cFingerPoint;
	HCURSOR cFingerDown;
	HCURSOR cHand;
	HCURSOR cHandGrab;

	//Image Cursors
	ImageCursor iFingerPoint;
	ImageCursor iFingerDown;
	ImageCursor iHand;
	ImageCursor iHandGrab;

	StatusPane * mStatusPane;
	EnvironmentPanel * mEnvironmentPanel;
	Toolbar * mToolbar;
	Disaster * mDisasterPanel;
};