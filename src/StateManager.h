#pragma once

class State;
class BaseApp;

class StateManager
{
public:
	StateManager(BaseApp * theApp);
	~StateManager();

	void Init();
	void Render();
	void Update();
	void SetState(State * theState);
protected:
	State * mCurState;
	BaseApp * mApp;
};