#include "StateManager.h"
#include "State.h"

StateManager::StateManager(BaseApp * theApp)
{
	mCurState = 0;
	mApp = theApp;
}

StateManager::~StateManager()
{

}

void StateManager::Init()
{
	if (mCurState)
		mCurState->Init();
}

void StateManager::Render()
{
	if (mCurState)
		mCurState->Render();
}

void StateManager::Update()
{
	if (mCurState)
		mCurState->Update();
}

void StateManager::SetState(State * theState)
{
	mCurState = theState;
}