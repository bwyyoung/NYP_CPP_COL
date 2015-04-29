#pragma once
#include "SimianBase.h"
#include <map>
#include <String>

using namespace std;

class Image;
class GameState;
class StateManager;
class Entity;
class TitleState;
class State;

enum CleanableStates
{
	CSGameState, CSTitleState
};

struct CleanState
{
	bool Clean;
	CleanableStates mClean;
	CleanableStates mNew;
};

class BaseApp : public SimianBase
{
public:
	BaseApp();
	~BaseApp();

	virtual void Update();
	virtual void Render();
	virtual void Cleanup();

	void Start();

	Image * CreateImage(char * thePath, char * theName);
	Image * CreateImage(char * thePath);
	map<string, Image *> GetImageDatabase();

	void ShowFatalError(char * ErrorMessage);

	GameState * mGameState;
	TitleState * mTitleState;

	CleanState mCleaner;

	bool mIdealScene;

	void CleanoutState(CleanableStates Old, CleanableStates New);
	void ProcessCleanStates();

	State * GetState(CleanableStates theState, bool GenState = 0);
protected:
	StateManager * mStateManager;
};
