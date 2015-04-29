#include "BaseApp.h"
#include <Windows.h>
#include "Image.h"
#include "WinAPI.h"
#include "StateManager.h"
#include "GameState.h"
#include "TitleState.h"

BaseApp::BaseApp():SimianBase(RENDERER_DIRECTX)
{
	mGameState = 0;
	mTitleState = 0;
	mCleaner.Clean = false;
	mIdealScene = false;
}

BaseApp::~BaseApp()
{
	Cleanup();
}

void BaseApp::CleanoutState(CleanableStates Old, CleanableStates New)
{
	mCleaner.Clean = true;
	mCleaner.mNew = New;
	mCleaner.mClean = Old;
}

State * BaseApp::GetState(CleanableStates theState, bool GenState)
{
	if (theState == CSGameState)
	{
		if (!GenState)
			return mGameState;
		else
			return (mGameState = new GameState(this));
	}
	if (theState == CSTitleState)
	{
		if (!GenState)
			return mTitleState;
		else
			return (mTitleState = new TitleState(this));
	}

	return 0;
}

#include <iostream>

using namespace std;

void BaseApp::ProcessCleanStates()
{
	if (mCleaner.Clean)
	{
		mCleaner.Clean = false;
		
		//check the last state..
		State * mOldState;
		State * mNewState;

		mOldState = GetState(mCleaner.mClean);
		SAFE_DELETE(mOldState);

		mNewState = GetState(mCleaner.mNew, true);

		if (mNewState == mGameState && mIdealScene)
		{
			mGameState->NumPredators = 70;
			mGameState->NumPrey = 200;
			mGameState->NumChucks = 0;
			mGameState->NumMoose = 10;
		}
		else
		{
			mGameState->NumChucks = mGameState->NumPrey = mGameState->NumPredators = 0;
			mGameState->NumMoose = 0;
		}

		mStateManager->SetState(mNewState);
		mStateManager->Init();
	}
}

void BaseApp::Update()
{
	mStateManager->Update();

	//clean up states..
	ProcessCleanStates();
}

void BaseApp::Render()
{
	mStateManager->Render();
}

void BaseApp::ShowFatalError(char * ErrorMessage)
{
	MessageBoxA(NULL, ((string)("Fatal Error: " + (string)ErrorMessage)).c_str(), "Fatal Error", MB_ICONEXCLAMATION|MB_OK);
	mRunning = false;
	exit(0);
}

void BaseApp::Cleanup()
{
	SAFE_DELETE(mGameState);
	SAFE_DELETE(mTitleState);
	SAFE_DELETE(mStateManager);
}

void BaseApp::Start()
{
	mStateManager = new StateManager(this);
	//mGameState = new GameState(this);
	mTitleState = new TitleState(this);

	mStateManager->SetState(mTitleState);
	mStateManager->Init();

	SetUpdateRate(70);

	SimianBase::Start();
}

Image * BaseApp::CreateImage(char *thePath, char *theName)
{
	return mGameState->CreateImage(thePath, theName);
}

Image * BaseApp::CreateImage(char * thePath)
{
	return SimianBase::CreateImage(thePath);
}

map<string, Image *> BaseApp::GetImageDatabase()
{
	return mGameState->GetImageDatabase();
}
