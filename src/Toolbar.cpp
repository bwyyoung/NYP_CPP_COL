#include "Toolbar.h"
#include "Sprite.h"
#include "BaseApp.h"
#include "Image.h"
#include "ToolbarButton.h"
#include "Macs.h"
#include "Entity.h"
#include "MouseInput.h"
#include "Prey.h"
#include "Grass.h"
#include "Predator.h"
#include "ObjectManager.h"
#include "Chuck Norris.h"
#include "GameState.h"
#include "Moose.h"

Toolbar::Toolbar(BaseApp * theApp) : Panel(theApp)
{
	// Load sprites..
	mChuckNorrisPic		= new Sprite(theApp, theApp->mGameState->GetImageDatabase()["Chuck Norris"]);
	mPreyPic			= new Sprite(theApp, theApp->mGameState->GetImageDatabase()["Prey"]);
	mPredatorPic		= new Sprite(theApp, theApp->mGameState->GetImageDatabase()["Predator"]);
	mPlantPic			= new Sprite(theApp, theApp->mGameState->GetImageDatabase()["Grass"]);
	mMoosePic			= new Sprite(theApp, theApp->mGameState->GetImageDatabase()["Moose"]);
	mCurDrop			= 0;

	// Load buttons..
	mChuckNorrisPic->SetFrames(4, 6);
	int ChuckDefault[] = { 6, 8, 7 };
	mChuckNorrisPic->AddAnimation(ChuckDefault, 3);
	mChuckNorrisPic->SetAnimationSpeed(300);
	mChuckButton = new ToolbarButton(theApp, mChuckNorrisPic);

	mPreyPic->SetFrames(9, 6);
	int PreyDefault[] = { 0, 1, 0, 2 };
	mPreyPic->AddAnimation(PreyDefault, 4);
	mPreyPic->SetAnimationSpeed(200);
	mPreyButton = new ToolbarButton(theApp, mPreyPic);

	mPredatorPic->SetFrames(4, 6);
	int PredatorDefault[] = { 6, 7, 6, 8 };
	mPredatorPic->AddAnimation(PredatorDefault, 4);
	mPredatorPic->SetAnimationSpeed(300);
	mPredatorButton = new ToolbarButton(theApp, mPredatorPic);

	mPlantPic->SetFrames(2, 3);
	int PlantDefault[] = { 0, 1 };
	mPlantPic->AddAnimation(PlantDefault, 2);
	mPlantPic->SetAnimationSpeed(200);
	mPlantButton = new ToolbarButton(theApp, mPlantPic);

	mMoosePic->SetFrames(6, 3);
	int MooseDefault[] = {5,4,5,3};
	mMoosePic->AddAnimation(MooseDefault, 4);
	mMoosePic->SetAnimationSpeed(200);
	mMooseButton = new ToolbarButton(theApp, mMoosePic);

	// Load BG Image..
	mToolbarBG = theApp->CreateImage("../data/ToolbarBG.png", "Toolbar Background");
	SetSize(mToolbarBG->GetWidth(), mToolbarBG->GetHeight());
	SetHandle(0, 0, 50, 10);
}

Toolbar::~Toolbar()
{
	delete mChuckNorrisPic;
	delete mPreyPic;
	delete mPredatorPic;
	delete mPlantPic;
	delete mMoosePic;
	delete mChuckButton;
	delete mPreyButton;
	delete mPredatorButton;
	delete mPlantButton;
	delete mMooseButton;
}

void Toolbar::Update()
{

}

bool Toolbar::Updateb()
{
	int CheckVal;
	
	CheckVal = mPlantButton->Updateb();
	if (CheckVal == 2)
	{
		// plant button was pressed..
		if (mCurDrop == mPlantPic)
			mCurDrop = 0;
		else
			mCurDrop = mPlantPic;
	}
	
	CheckVal = mPreyButton->Updateb();
	if (CheckVal == 2)
	{
		if (mCurDrop == mPreyPic)
			mCurDrop = 0;
		else
			mCurDrop = mPreyPic;
	}

	CheckVal = mMooseButton->Updateb();
	if (CheckVal == 2)
	{
		if (mCurDrop == mMoosePic)
			mCurDrop = 0;
		else
			mCurDrop = mMoosePic;
	}
	
	CheckVal = mPredatorButton->Updateb();
	if (CheckVal == 2)
	{
		// Predator button was pressed, draw a new predator at the mouse cursor..
		if (mCurDrop == mPredatorPic)
			mCurDrop = 0;
		else
			mCurDrop = mPredatorPic;
	}

	CheckVal = mChuckButton->Updateb();
	if (CheckVal == 2)
	{
		if (mCurDrop == mChuckNorrisPic)
			mCurDrop = 0;
		else
			mCurDrop = mChuckNorrisPic;
	}

	if (mApp->mMouseInput->GetButtonDown(LBUTTON) && mCurDrop)
	{
		int MouseX, MouseY;
		mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);

		Entity ** LoopArray;
		Entity_Type Type;

		if (mCurDrop == mPreyPic)
		{
			LoopArray = (Entity **)mApp->mGameState->mPrey;
			Type = ET_PREY;
		}
		else if (mCurDrop == mPredatorPic)
		{
			LoopArray = (Entity **)mApp->mGameState->mPredators;
			Type = ET_PREDATOR;
		}
		else if (mCurDrop == mPlantPic)
		{
			LoopArray = (Entity **)mApp->mGameState->mGrass;
			Type = ET_GRASS;
		}
		else if (mCurDrop == mChuckNorrisPic)
		{
			LoopArray = (Entity **)mApp->mGameState->mChucklets;
			Type = ET_CHUCK;
		}
		else if (mCurDrop == mMoosePic)
		{
			LoopArray = (Entity **)mApp->mGameState->mMoose;
			Type = ET_MOOSE;
		}

		//spawn a new creature at this point..
		for (int i = 0; i < ENTITY_LISTSIZE; i ++)
		{
			if (!LoopArray[i]->GetAlive())
			{
				if (Type == ET_PREY)
					((Prey *) LoopArray[i])->Reset();
				else if (Type == ET_PREDATOR)
					((Predator *) LoopArray[i])->Reset();
				else if (Type == ET_CHUCK)
					((Chuck_Norris *) LoopArray[i])->Reset();
				else if (Type == ET_GRASS)
					((Grass *) LoopArray[i])->Reset();
				else if (Type == ET_MOOSE)
					((Moose *) LoopArray[i])->Reset();
				
				if (Type == ET_PREDATOR || Type == ET_MOOSE)
				{
					//generate a random gender..
					int Gender = rand()%2;

					if (Gender == 1)
						LoopArray[i]->SetGender(true);
					else
						LoopArray[i]->SetGender(false);
				}

				// place the newly created creature at the cursor..
				LoopArray[i]->SetPosition(float(MouseX + mApp->mGameState->mObjectManager->mScrollX - LoopArray[i]->GetWidth()/2), float(MouseY + mApp->mGameState->mObjectManager->mScrollY - LoopArray[i]->GetHeight()/2));
				mCurDrop = 0;
				if (mCurSelection)
					mCurSelection->Deselect();
				mCurSelection = LoopArray[i];
				break;
			}
		}
	}

	return Panel::Updateb();
}

void Toolbar::Render()
{
	Render(mX, mY);
}

void Toolbar::Render(float x, float y)
{
	mToolbarBG->Render(x, y);

	mPlantButton->SetPosition(x + 10, y + 20);
	mPlantButton->Render();
	mPreyButton->SetPosition(x + 60, y + 20);
	mPreyButton->Render();
	mPredatorButton->SetPosition(x + 110, y + 20);
	mPredatorButton->Render();
	mChuckButton->SetPosition(x + 160, y + 20);
	mChuckButton->Render();
	mMooseButton->SetPosition(x + 210, y + 20);
	mMooseButton->Render();

	if (mCurDrop)
	{
		int MouseX, MouseY;
		mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);
		mCurDrop->Render(float(MouseX - mCurDrop->GetWidth()/2), float(MouseY - mCurDrop->GetHeight()/2));
	}
}