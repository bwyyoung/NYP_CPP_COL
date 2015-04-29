#include "GameState.h"
#include "BaseApp.h"
#include <Windows.h>
#include "Image.h"
#include "WinAPI.h"
#include "Predator.h"
#include "Chuck Norris.h"
#include "Entity.h"
#include "Camera.h"
#include "Macs.h"
#include "Math.h"
#include "Prey.h"
#include "Grass.h"
#include "TileGenerator.h"
#include "ObjectManager.h"
#include <iostream>
#include "MouseInput.h"
#include "Sprite.h"
#include "Minimap.h"
#include "StatusPane.h"
#include "Toolbar.h"
#include "Moose.h"
#include "MessageBoard.h"
#include "Environment.h"
#include "EnvironmentPanel.h"
#include "Disaster.h"

bool USEHWMOUSE = true;

bool MessageCheck(UINT uMsg)
{
	switch(uMsg)
	{
	case WM_SETCURSOR:
		return true;
	}
	return false;
}

GameState::GameState(BaseApp * theApp) : State(theApp)
{
	mApp = theApp;
	TimePassed = 0;
	Duration = 0;
	DEBUG = true;
}

GameState::~GameState()
{
	for (int i = 0; i < ENTITY_LISTSIZE; i ++) {
		SAFE_DELETE(mPredators[i]);
		SAFE_DELETE(mChucklets[i]);
		SAFE_DELETE(mPrey[i]);
		SAFE_DELETE(mGrass[i]);
		SAFE_DELETE(mMoose[i]);
	}
	SAFE_DELETEA(mPredators);
	SAFE_DELETEA(mChucklets);
	SAFE_DELETEA(mPrey);
	SAFE_DELETEA(mGrass);
	SAFE_DELETEA(mMoose);
	SAFE_DELETEA(mAllTargets);
	SAFE_DELETE(mEnvironment);

	for (map<string, Image *>::iterator i = mImageDatabase.begin(); i != mImageDatabase.end(); i ++)
	{
		if (i->second)
			delete i->second;
	}

	SAFE_DELETE(mMinimap);
	SAFE_DELETE(mStatusPane);
	SAFE_DELETE(mDisasterPanel);
	SAFE_DELETE(mToolbar);

	delete mTileGenerator;
}

void GameState::Init()
{
	mCamera = mApp->CreateCamera();

	//put loading code here.
	mApp->CreateImage("../data/SelectionRing.png", "SelectionRing");
	mApp->CreateImage("../data/Predator.png", "Predator");
	mApp->CreateImage("../data/Prey.png", "Prey");
	mApp->CreateImage("../data/ChuckNorris.png", "Chuck Norris");
	mApp->CreateImage("../data/Rage.png", "Rage");
	mApp->CreateImage("../data/Grass.png" , "Grass");
	mApp->CreateImage("../data/WOD.png", "WOD");
	mApp->CreateImage("../data/High.png", "High");
	mApp->CreateImage("../data/Moose.png", "Moose");

	mObjectManager = new ObjectManager(mApp);

	mEnvironment = new Environment(mApp);
	mEnvironment->Init();
	mEnvironment->SetEnvState(esClear);

	mPredators = new Predator*[ENTITY_LISTSIZE];
	for (int i = 0; i < ENTITY_LISTSIZE; i ++)
	{
		mPredators[i] = new Predator(mApp, "Predator");
		mPredators[i]->SetSize(32, 32);
		mPredators[i]->SetPosition(float(rand()%1600), float(rand()%1600));

		if (i < NumPredators)
			mPredators[i]->SetAlive(true);
		else
			mPredators[i]->SetAlive(false);

		int Gender = rand()%2;
		if (Gender)
			mPredators[i]->SetGender(true);
		else
			mPredators[i]->SetGender(false);

		mObjectManager->AddObject(mPredators[i]);
	}
	mMoose = new Moose*[ENTITY_LISTSIZE];
	for (int i = 0;i < ENTITY_LISTSIZE; i ++)
	{
		mMoose[i] = new Moose(mApp, "Moose");
		mMoose[i]->SetSize(32,32);
		mMoose[i]->SetPosition(float(rand()%1600), float(rand()%1600));

		if (i < NumMoose)
			mMoose[i]->SetAlive(true);
		else
			mMoose[i]->SetAlive(false);

		int Gender = rand()%2;
		if (Gender)
			mMoose[i]->SetGender(true);
		else
			mMoose[i]->SetGender(false);
		mObjectManager->AddObject(mMoose[i]);
	}
	mPrey = new Prey*[ENTITY_LISTSIZE];

	for (int i = 0; i < ENTITY_LISTSIZE; i++)
	{
		mPrey[i] = new Prey(mApp,"Prey");
		mPrey[i]->SetSize(32,32);
		mPrey[i]->SetPosition(float(rand()%1600), float(rand()%1600));
		mPrey[i]->SetAlive(false);
		if (i %5 == 0)
		{
			mPrey[i]->MakeLeader(true);
		}
		else
		{
			mPrey[i]->MakeLeader(false);
		}

		if (i<NumPrey)
			mPrey[i]->SetAlive(true);
		mObjectManager->AddObject(mPrey[i]);
		mPrey[i]->SetID(i);
	}

	mChucklets = new Chuck_Norris*[ENTITY_LISTSIZE];
	for (int i = 0; i < ENTITY_LISTSIZE; i++)
	{
		mChucklets[i] = new Chuck_Norris(mApp,"Chuck Norris");
		mChucklets[i]->SetSize(32,32);
		mChucklets[i]->SetPosition(float(rand()%1600), float(rand()%1600));
		mChucklets[i]->SetAlive(false);
		if (i<NumChucks)
			mChucklets[i]->SetAlive(true);

		mChucklets[i]->SetGender(GENDER_MALE);
		mObjectManager->AddObject(mChucklets[i]);
	}
	mGrass = new Grass*[ENTITY_LISTSIZE];
	for (int i = 0; i < ENTITY_LISTSIZE; i++)
	{
		mGrass[i] = new Grass(mApp,"Grass");
		mGrass[i]->SetSize(32,32);
		mGrass[i]->SetPosition(float(rand()%1600), float(rand()%1600));
		mGrass[i]->SetAlive(false);
		if ( i < 150 )
		{
			mGrass[i]->SetAlive(true);
		}
		mObjectManager->AddObject(mGrass[i]);
	}

	int j = 0;
	mAllTargets = new Entity* [ENTITY_LISTSIZE*5];
	for (int i = 0; i < ENTITY_LISTSIZE*5; i++)
	{
		if (i < ENTITY_LISTSIZE)
			mAllTargets[i] = mPredators[j];
		else if (i < ENTITY_LISTSIZE * 2)
			mAllTargets[i] = mPrey[j];
		else if (i < ENTITY_LISTSIZE * 3)
			mAllTargets[i] = mGrass[j];
		else if (i < ENTITY_LISTSIZE * 4)
			mAllTargets[i] = mChucklets[j];
		else if (i < ENTITY_LISTSIZE * 5)
			mAllTargets[i] = mMoose[j];

		j++;
		if (j >= ENTITY_LISTSIZE)
			j = 0;
	}
	NewPrey = new Entity*[ENTITY_LISTSIZE*2];
	for (int i = 0; i<ENTITY_LISTSIZE; i++)
	{
		NewPrey[i] = mChucklets[i];
	}
	for (int i = 0; i<ENTITY_LISTSIZE; i++)
	{
		NewPrey[i+ENTITY_LISTSIZE] = mPredators[i];
	}
	mTileGenerator = new TileGenerator(mApp);
	mTileGenerator->LoadTiles("../data/Tileset.png");
	mTileGenerator->GenerateTiles(50, 50);
	mObjectManager->AddTiledMap(mTileGenerator);
	ProcessMessage(MessageCheck);

	//Loading hardware cursors.
	cFingerPoint = LoadCursorFromFileA("../data/MightyHandPoint.cur");
	cFingerDown = LoadCursorFromFileA("../data/MightyHandPointDown.cur");
	cHand = LoadCursorFromFileA("../data/MightyHand.cur");
	cHandGrab = LoadCursorFromFileA("../data/MightyHandPick.cur");

	//Loading image cursors.
	iFingerPoint.Img = mApp->CreateImage("../data/MightyHandPoint.png", "CursorFinger"); iFingerPoint.Img->SetSize(32,32); iFingerPoint.X = 8; iFingerPoint.Y = 10;
	iFingerDown.Img = mApp->CreateImage("../data/MightyHandPointDown.png", "CursorFingerDown"); iFingerDown.Img->SetSize(32,32); iFingerDown.X = 8; iFingerDown.Y = 10;
	iHand.Img = mApp->CreateImage("../data/MightyHand.png", "CursorHand"); iHand.X = 14; iHand.Y = 10;
	iHandGrab.Img = mApp->CreateImage("../data/MightyHandPick.png", "CursorHandGrab"); iHandGrab.X = 14; iHandGrab.Y = 10;

	if (!USEHWMOUSE)
		ShowCursor(FALSE);

	mMinimap = new Minimap(mApp);
	mMinimap->SetPosition(800 - 180, 10);

	mStatusPane = new StatusPane(mApp);
	mStatusPane->SetPosition(800-180, 20 + 160);

	mToolbar = new Toolbar(mApp);
	mToolbar->SetPosition((800 - 260)/2, 600 - 70 - 10);

	mEnvironmentPanel = new EnvironmentPanel(mApp);
	mEnvironmentPanel->SetPosition(800-180, 20 + 160 + 200 + 10);

	mDisasterPanel = new Disaster(mApp);
	mDisasterPanel->SetPosition(0, 0);

	Mode = 0;

	//no loading code beyond this point.
}

void GameState::Render()
{
	
	mCamera->Set2DMode();
	mObjectManager->Render();
	mEnvironment->Render();
	mStatusPane->Render();
	mToolbar->Render();
	mMinimap->Render();
	mEnvironmentPanel->Render();
	mDisasterPanel->Render();

	if (!USEHWMOUSE)
	{
		int MouseX, MouseY;

		mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);

		if (Mode == 0)
		{
			if (mApp->mMouseInput->GetButtonDown(LBUTTON))
				iFingerDown.Img->Render((float)(MouseX - iFingerDown.X), (float)(MouseY - iFingerDown.Y));
			else
				iFingerPoint.Img->Render(float(MouseX - iFingerPoint.X), float(MouseY - iFingerPoint.Y));
		}
		else if (Mode == 1)
		{
			if (mApp->mMouseInput->GetButtonDown(LBUTTON))
				iHandGrab.Img->Render((float)(MouseX - iHandGrab.X), (float)(MouseY - iHandGrab.Y));
			else
				iHand.Img->Render((float)(MouseX - iHand.X), (float)(MouseY - iHand.Y));
		}
	}
}

void GameState::Update()
{


	if (DEBUG)
	{
		TimePassed += (float)mApp->mTimeDelta;
		if (TimePassed>120000)
		{
			Duration+= 2;
			TimePassed = 0;
			int PreyCount=0;
			int PreyLeaderCount = 0;
			int PreyFollowerCount = 0;
			int PredatorCount=0;
			int MooseCount=0;
			int GrassCount=0;

			for (int i = 0; i<ENTITY_LISTSIZE; i++)
			{
				if (mGrass[i]->GetAlive())
					GrassCount++;
				if (mPrey[i]->GetAlive())
				{
					PreyCount++;
					if (mPrey[i]->CheckLeader())
						PreyLeaderCount++;
					else
						PreyFollowerCount++;
				}
				if (mPredators[i]->GetAlive())
					PredatorCount++;
				if (mMoose[i]->GetAlive())
					MooseCount++;
			}
			cout<<"Creature Count Report at: "<<Duration<<" Minutes"<<endl;
			cout<<"============================"<<endl;
			cout<<"Prey Count:		         "<<PreyCount<<endl;
			cout<<"		Leader Count:     "<<PreyLeaderCount<<endl;
			cout<<"		Follower Count: "<<PreyFollowerCount<<endl;
			cout<<"Predator Count:     "<<PredatorCount<<endl;
			cout<<"Moose Count:		 "<<MooseCount<<endl;
			cout<<"Grass Count:            "<<GrassCount<<endl;
			cout<<endl;
		}
	}


	mObjectManager->Update();
	bool MinimapClicked = mMinimap->Updateb();
	bool StatusClicked = mStatusPane->Updateb();
	bool ToolbarClicked = mToolbar->Updateb();
	bool EnvironmentClicked = mEnvironmentPanel->Updateb();
	bool DisasterClicked = mDisasterPanel->Updateb();

	if (!MinimapClicked && !StatusClicked && !ToolbarClicked && !EnvironmentClicked && !DisasterClicked)
		mObjectManager->Select();

	mEnvironment->Update();

	if (USEHWMOUSE)
	{
		if (Mode == 0)
		{
			if (mApp->mMouseInput->GetButtonDown(LBUTTON))
				SetCursor(cFingerDown);
			else
				SetCursor(cFingerPoint);
		}
		else if (Mode == 1)
		{
			if (mApp->mMouseInput->GetButtonDown(LBUTTON))
				SetCursor(cHandGrab);
			else
				SetCursor(cHand);
		}
	}
}

Image * GameState::CreateImage(char * thePath, char * theName)
{
	Image * CurImage = ((SimianBase *)mApp)->CreateImage(thePath);

	if (!CurImage)
		return 0;
	else
	{
		map<string, Image *>::iterator it;
		it = mImageDatabase.find(theName);
		if (it == mImageDatabase.end())
		{
			mImageDatabase[theName] = CurImage;
			return CurImage;
		}

		return 0;
	}
}

map<string, Image *> GameState::GetImageDatabase()
{
	return mImageDatabase;
}
