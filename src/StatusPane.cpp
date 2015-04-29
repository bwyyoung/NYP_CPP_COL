#include "StatusPane.h"
#include "BaseApp.h"
#include "Image.h"
#include "MouseInput.h"
#include "BitmappedFont.h"
#include "Entity.h"
#include "PointerButton.h"
#include <string>
#include "GameState.h"
#include "Prey.h"

using namespace std;

StatusPane::StatusPane(BaseApp *theApp) : Panel(theApp)
{
	mFont			= new BitmappedFont(theApp, mApp->CreateImage("../data/BitmapFont.png", "StatusPaneFont"));
	mBGImage		= mApp->CreateImage("../data/StatusPane.png", "StatusPaneBG");
	mChuckNorris	= mApp->CreateImage("../data/ChuckDisplay.png", "ChuckNorrisPic");
	mWolf			= mApp->CreateImage("../data/WolfDisplay.png", "WolfPic");
	mDeer			= mApp->CreateImage("../data/DeerDisplay.png", "DeerPic");
	mMoose			= mApp->CreateImage("../data/MooseDisplay.png", "MoosePic");
	mHungerBar		= mApp->CreateImage("../data/HungerBarEmpty.png", "HungerBarEmpty");
	mHungerBarBack	= mApp->CreateImage("../data/HungerBarFull.png", "HungerBarFull");
	mCurEntity		= 0;
	mWidth			= 170;
	mHeight			= 200;
	mApp->CreateImage("../data/ButtonNormal.png", "ButtonUp");
	mApp->CreateImage("../data/ButtonDown.png", "ButtonDown");
	mPointerButton	= new PointerButton(theApp, theApp->GetImageDatabase()["CursorFinger"]);
	mGrabberButton	= new PointerButton(theApp, theApp->GetImageDatabase()["CursorHand"]);
	SetHandle(0, 0, 10, 50);
}

StatusPane::~StatusPane()
{
	delete mFont;
	delete mPointerButton;
	delete mGrabberButton;
}

void StatusPane::Update()
{

}

bool StatusPane::Updateb()
{
	int Feedback;
	Feedback = mPointerButton->Updateb();
	if (Feedback == 2)
	{
		//set pointer..
		mApp->mGameState->Mode = 0;
	}
	Feedback = mGrabberButton->Updateb();
	if (Feedback == 2)
	{
		//set pointer..
		mApp->mGameState->Mode = 1;
	}
	return Panel::Updateb();
}
void StatusPane::Render()
{
	Render(mX, mY);
	mPointerButton->SetPosition(mX + 10 + 160/2 - 55, mY + 125);
	mPointerButton->Render();
	mGrabberButton->SetPosition(mPointerButton->GetX() + 60, mY + 125);
	mGrabberButton->Render();
}

void StatusPane::SetEntity(Entity * theEntity)
{
	mCurEntity = theEntity;
}

void StatusPane::Render(float x, float y)
{
	mBGImage->Render(x, y);

	//render the cur entity's information.
	if (mCurEntity && mCurEntity->GetType() != ET_GRASS)
	{
		Image * RenderTarget;
		string Name;
		string Name2;
		if (mCurEntity->GetType() == ET_CHUCK)
		{
			RenderTarget = mChuckNorris;
			Name = "Chuck";
			Name2 = "Norris";
		}
		else if (mCurEntity->GetType() == ET_PREY)
		{
			RenderTarget = mDeer;
			Name = "Deer";
			if (mCurEntity->GetGender() == true)
				Name2 = "Male";
			else
				Name2 = "Female";
		}
		else if (mCurEntity->GetType() == ET_PREDATOR)
		{
			RenderTarget = mWolf;
			Name = "Wolf";
			if (mCurEntity->GetGender() == true)
				Name2 = "Male";
			else
				Name2 = "Female";
		}
		else if (mCurEntity->GetType() == ET_MOOSE)
		{
			RenderTarget = mMoose;
			Name = "Moose";
			if (mCurEntity->GetGender() == true)
				Name2 = "Male";
			else
				Name2 = "Female";
		}
		
		RenderTarget->Render(x + 20, y + 10);
		mFont->DrawString(x + 25 + 60, y + 15, (char *)Name.c_str());
		mFont->DrawString(x + 25 + 60, y + 32, (char *)Name2.c_str(), 9);
		char Age[10];
		_itoa_s(mCurEntity->GetAge(), Age, 10);
		mFont->DrawString(x + 25 + 60, y + 45, (char*)((string)"Age:  " + Age).c_str(), 8);
		//render the entity's current hunger..
		mFont->DrawString(x + 25 + 60, y + 60, "Hunger", 8);
	
		mHungerBar->SetSize(75, 5);
		mHungerBar->Render(x + 25 + 60, y + 70);

		mHungerBarBack->SetSize(75 * mCurEntity->GetHunger()/100, 5);
		mHungerBarBack->Render(x + 25 + 60, y + 70);

		if (mCurEntity->GetState() == ES_IDLE)
			mFont->DrawString(x + 25 + 60, y + 85, "Idle", 8);
		else if (mCurEntity->GetState() == ES_HUNTING)
			mFont->DrawString(x + 25 + 60, y + 85, "Hunting", 8);
		else if (mCurEntity->GetState() == ES_ATTACK)
			mFont->DrawString(x + 25 + 60, y + 85, "Attacking", 8);
		else if (mCurEntity->GetState() == ES_MATING)
			mFont->DrawString(x + 25 + 60, y + 85, "Mating", 8);
		else if (mCurEntity->GetState() == ES_STALKING)
			mFont->DrawString(x + 25 + 60, y + 85, "Stalking", 8);
		else if (mCurEntity->GetState() == ES_EVADING)
			mFont->DrawString(x + 25 + 60, y + 85, "Evading", 8);
		else if (mCurEntity->GetState() == ES_JUMPKICK)
			mFont->DrawString(x + 25 + 60, y + 85, "Jump Kick", 8);
		else if (mCurEntity->GetState() == ES_CHARGING)
			mFont->DrawString(x + 25 + 60, y + 85, "Charging", 8);
		if (mCurEntity->GetType() ==ET_PREY)
		{
			if (((Prey*)mCurEntity)->CheckLeader())
			{
				mFont->DrawString(x + 25 + 20,y+105,"Leader", 15);
			}
			else
			{
				mFont->DrawString(x + 25 + 10,y+105,"Follower", 15);
			}
		}
	}
}