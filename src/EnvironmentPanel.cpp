#include "EnvironmentPanel.h"
#include "PointerButton.h"
#include "BaseApp.h"
#include "Image.h"
#include "Environment.h"
#include "GameState.h"

EnvironmentPanel::EnvironmentPanel(BaseApp * theApp) : Panel(theApp)
{
	mBGImage = mApp->CreateImage("../data/StatusPaneEnv.png");
	mSunnyButton = new PointerButton(mApp, mApp->CreateImage("../data/Sunny.png"));
	mCloudyButton = new PointerButton(mApp, mApp->CreateImage("../data/Cloudy.png"));
	mDarkButton = new PointerButton(mApp, mApp->CreateImage("../data/DarkClouds.png"));
	mRainButton = new PointerButton(mApp, mApp->CreateImage("../data/ThunderStorm.png"));
	mNightButton = new PointerButton(mApp, mApp->CreateImage("../data/Night.png"));
	mWidth = 170;
	mHeight = 162;
	SetHandle(0, 0, 10, 100);
}

EnvironmentPanel::~EnvironmentPanel()
{
	delete mBGImage;
	delete mSunnyButton;
	delete mCloudyButton;
	delete mDarkButton;
	delete mRainButton;
	delete mNightButton;
}

void EnvironmentPanel::Update()
{

}

bool EnvironmentPanel::Updateb()
{
	int Feedback;
	Feedback = mSunnyButton->Updateb();
	if (Feedback == 2)
	{
		mApp->mGameState->mEnvironment->SetEnvState(esClear);
	}
	Feedback = mCloudyButton->Updateb();
	if (Feedback == 2)
	{
		mApp->mGameState->mEnvironment->SetEnvState(esRainLight);
	}
	Feedback = mDarkButton->Updateb();
	if (Feedback == 2)
	{
		mApp->mGameState->mEnvironment->SetEnvState(esRainMedium);
	}
	Feedback = mRainButton->Updateb();
	if (Feedback == 2)
	{
		mApp->mGameState->mEnvironment->SetEnvState(esRainHeavy);
	}
	Feedback = mNightButton->Updateb();
	if (Feedback == 2)
	{
		mApp->mGameState->mEnvironment->SetEnvState(esNight);
	}
	return Panel::Updateb();
}

void EnvironmentPanel::Render(float x, float y)
{
	mBGImage->Render(x, y);
	mSunnyButton->SetPosition(x + 30, y + 3);
	mSunnyButton->Render();
	mCloudyButton->SetPosition(x + 30 + 50 + 20, y + 3);
	mCloudyButton->Render();
	mDarkButton->SetPosition(x + 30, y + 3 + 50 + 3);
	mDarkButton->Render();
	mRainButton->SetPosition(x + 30 + 50 + 20, y + 3 + 50 + 3);
	mRainButton->Render();
	mNightButton->SetPosition(x + 30, y + 3 + 50 + 3 + 50 +3);
	mNightButton->Render();

}

void EnvironmentPanel::Render()
{
	Render(mX, mY);
}