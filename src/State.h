#pragma once

class BaseApp;

class State
{
public:
	State(BaseApp * theApp);
	~State();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	BaseApp * mApp;
};