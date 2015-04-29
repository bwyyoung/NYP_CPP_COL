#include "TitleState.h"
#include "Macs.h"
#include "PlaceHolder.h"
#include "ObjectManager.h"
#include "TileGenerator.h"
#include "BaseApp.h"
#include "Image.h"
#include "TextButton.h"
#include "WinAPI.h"
#include "Camera.h"

TitleState::TitleState(BaseApp * theApp):State(theApp)
{
	mObjectManager	= 0;
	mTileGenerator  = 0;
	mPredatorSprite = 0;
	mPreySprite		= 0;
	mGrassSprite	= 0;
	mPlaceHolders	= 0;
	mLogo			= 0;
	mAppearColour   = 0;
	mWaitTimer		= 0;
	mBackground		= 0;
	mNewGame		= 0;
}

TitleState::~TitleState()
{
	SAFE_DELETE(mObjectManager);
	SAFE_DELETE(mTileGenerator);

	SAFE_DELETE(mGrassSprite);
	SAFE_DELETE(mPreySprite);
	SAFE_DELETE(mBackground);
	SAFE_DELETE(mPredatorSprite);

	SAFE_DELETE(mLogo);

	for (int i = 0; i < 30; i ++)
	{
		SAFE_DELETE(mPlaceHolders[i]);
	}
	SAFE_DELETEA(mPlaceHolders);

	SAFE_DELETE(mNewGame);
}

void TitleState::Init()
{
	mCamera = mApp->CreateCamera();

	mObjectManager = new ObjectManager(mApp);

	float yInc = 60;

	mNewGame = new TextButton(mApp, "New Scene", 10);
	mNewGame->SetPosition(350, 200 + yInc);
	mIdealScene = new TextButton(mApp, "Create Ideal Scene", 10);
	mIdealScene->SetPosition(300, 230 + yInc);
	mExit = new TextButton(mApp, "Exit", 10);
	mExit->SetPosition(380, 260 + yInc);

	//load images..
	mGrassSprite = mApp->CreateImage("../data/Grass.png");
	mPreySprite = mApp->CreateImage("../data/Prey.png");
	mPredatorSprite = mApp->CreateImage("../data/Predator.png");

	mLogo = mApp->CreateImage("../data/CircleOfLifeLogo.png");
	mLogo->SetColor(Color(0, 0, 0, 0));

	mBackground = mApp->CreateImage("../data/HungerBarEmpty.png");
	mBackground->SetColor(Color(0, 0, 0, 170));
	mBackground->SetSize(300, 200);
	mBackground->SetPosition((800-300)/2, (600-200)/2);

	mTileGenerator = new TileGenerator(mApp);
	mTileGenerator->LoadTiles("../data/Tileset.png");
	mTileGenerator->GenerateTiles(25, 19);

	//add creatures..
	mPlaceHolders = new PlaceHolder *[30];
	for (int i = 0; i < 30; i ++)
	{
		int choice = rand()%3;
		if (choice == 0)
		{
			mPlaceHolders[i] = new PlaceHolder(mApp, mPredatorSprite);
			mPlaceHolders[i]->SetAnimation(0, ET_PREDATOR);
		}
		else if (choice == 1)
		{
			mPlaceHolders[i] = new PlaceHolder(mApp, mPreySprite);
			mPlaceHolders[i]->SetAnimation(0, ET_PREY);
		}
		else
		{
			mPlaceHolders[i] = new PlaceHolder(mApp, mGrassSprite);
			mPlaceHolders[i]->SetAnimation(0, ET_GRASS);
		}

		mPlaceHolders[i]->SetSize(32, 32);

		//now that these are set.. random the pos.
		mPlaceHolders[i]->SetPosition(static_cast<float>(rand()%800), static_cast<float>(rand()%600));
		mObjectManager->AddObject(mPlaceHolders[i]);
	}

	mObjectManager->AddTiledMap(mTileGenerator);
}

void TitleState::Render()
{
	mCamera->Set2DMode();
	mObjectManager->Render();
	mBackground->Render();
	mLogo->Render();
	mNewGame->Render();
	mIdealScene->Render();
	mExit->Render();
}

void TitleState::Update()
{
	int Clicked;

	if ((Clicked = mNewGame->Updateb()) == 2) 
	{
		mApp->mIdealScene = false;
		mApp->CleanoutState(CSTitleState, CSGameState);
	}

	if ((Clicked = mIdealScene->Updateb()) == 2) 
	{
		mApp->mIdealScene = true;
		mApp->CleanoutState(CSTitleState, CSGameState);
	}

	if ((Clicked = mExit->Updateb()) == 2) 
		mRunning = false;

	if (mWaitTimer != 3000)
	{
		if (mAppearColour < 950)
			mAppearColour += mApp->mTimeDelta;
		else
			mAppearColour = 950;
	}
	else
	{
		if (mAppearColour > 0)
			mAppearColour -= mApp->mTimeDelta;
		else
			mAppearColour = 0;
	}

	if (mAppearColour == 950)
		mWaitTimer += mApp->mTimeDelta;

	if (mWaitTimer > 3000)
		mWaitTimer = 3000;

	int Col = static_cast<int>(static_cast<float>(mAppearColour)/1000.0f * 255.0f);

	if (Col > 255)
		Col = 255;
	else if (Col < 0)
		Col = 0;

	mLogo->SetColor(Color(Col, Col, Col, Col));
	mObjectManager->UpdateObjects();
}