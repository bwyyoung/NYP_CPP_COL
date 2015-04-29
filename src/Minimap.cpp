#include "Minimap.h"
#include "BaseApp.h"
#include "Sprite.h"
#include "ObjectManager.h"
#include "TileGenerator.h"
#include "Macs.h"
#include "Prey.h"
#include "Image.h"
#include "Chuck Norris.h"
#include "Predator.h"
#include "MouseInput.h"
#include "Grass.h"
#include "GameState.h"
#include "Moose.h"

Minimap::Minimap(BaseApp * theApp) : Panel(theApp)
{
	mMinimapBG = mApp->CreateImage("../data/MinimapBG.png", "MinimapBG");
	mCurView = mApp->CreateImage("../data/Scope.png", "MinimapScope");
	mCurView->SetSize(25 * 3, 19 * 3);
	mMinimapSprite = new Sprite(theApp, mApp->CreateImage("../data/Minimap.png", "Minimap"));
	mMinimapSprite->SetFrames(1, 7);
	int grass[] = {0};
	int dirt[]	= {1};
	int prey[]  = {2};
	int pred[]	= {3}; 
	int chuck[]	= {4};
	int plant[]	= {5};
	int moose[] = {6};
	mMinimapSprite->SetSize(3,3);
	MINIMAP_GRASS = mMinimapSprite->AddAnimation(grass, 1);
	MINIMAP_DIRT = mMinimapSprite->AddAnimation(dirt, 1);
	MINIMAP_PREY = mMinimapSprite->AddAnimation(prey, 1);
	MINIMAP_PREDATOR = mMinimapSprite->AddAnimation(pred, 1);
	MINIMAP_CHUCK = mMinimapSprite->AddAnimation(chuck, 1);
	MINIMAP_PLANT = mMinimapSprite->AddAnimation(plant, 1);
	MINIMAP_MOOSE = mMinimapSprite->AddAnimation(moose, 1);
	mMinimapSprite->SetAnimationSpeed(1000);
	mWidth = 170;
	mHeight = 160;
	SetHandle(0,0,10,55);
	MouseDown = false;
	Selected = false;
}

Minimap::~Minimap()
{
	delete mMinimapSprite;
}

void Minimap::Update()
{

}

bool Minimap::Updateb()
{
	//check if the mouse was pressed..
	if (mApp->mMouseInput->GetButtonDown(LBUTTON))
	{
		//mouse button is down, check if its within the bounds..
		int MouseX, MouseY;
		mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);

		if (!MouseDown)
		{
			Selected = false;

			if (MouseX > mX + 5 && MouseX < mX + 155)
				if (MouseY > mY + 5 && MouseY < mY + 155)
				{
					Selected = true;
				}
		}

		MouseDown = true;

		if (Selected)
			if (MouseX > mX + 5 && MouseX < mX + 155)
				if (MouseY > mY + 5 && MouseY < mY + 155)
				{
					//mouse is in the area..
					//recalculate scroll positions..
					int CurPosX = MouseX - ((int)mX + 5) - 40;
					int CurPosY = MouseY - ((int)mY + 5) - 28;

					

					//now that curposx and y has been set.. we can check what the percentage of this is.. and multiply it by the map's witdth
					mApp->mGameState->mObjectManager->mScrollX = (CurPosX * 1600)/150;
					if (mApp->mGameState->mObjectManager->mScrollX < 0)
						mApp->mGameState->mObjectManager->mScrollX = 0;
					else if (mApp->mGameState->mObjectManager->mScrollX > 1600 - 800)
						mApp->mGameState->mObjectManager->mScrollX = 1600 - 800;
					mApp->mGameState->mObjectManager->mScrollY = (CurPosY * 1600)/150;
					if (mApp->mGameState->mObjectManager->mScrollY < 0)
						mApp->mGameState->mObjectManager->mScrollY = 0;
					else if (mApp->mGameState->mObjectManager->mScrollY > 1600 - 600)
						mApp->mGameState->mObjectManager->mScrollY = 1600 - 600;
				}

	}
	else
	{
		MouseDown = false;
		Selected = false;
	}

	return Panel::Updateb();
}

void Minimap::Render()
{
	Render(mX, mY);
}

#include <iostream>

using namespace std;

void Minimap::Render(float tx, float ty)
{
	mMinimapBG->Render(tx, ty);
	float x = tx + 15;
	float y = ty + 5;
	for (int i = 0; i < mApp->mGameState->mObjectManager->mTiles->TileHeight; i ++)
		for (int j = 0; j < mApp->mGameState->mObjectManager->mTiles->TileWidth; j ++)
		{
			//render the tiles..
			if (mApp->mGameState->mObjectManager->mTiles->mTiles[i][j] == 0)
				mMinimapSprite->PlayAnimation(MINIMAP_GRASS);
			else
				mMinimapSprite->PlayAnimation(MINIMAP_DIRT);
			mMinimapSprite->Render(x + j * 3, y + i * 3);
		}

	for (int i = 0; i < ENTITY_LISTSIZE; i ++)
		if (mApp->mGameState->mGrass[i]->GetAlive())
		{
			float X = ((Object2D*)mApp->mGameState->mGrass[i])->GetX();
			float Y = mApp->mGameState->mGrass[i]->GetY();

			X = X/float(mApp->mGameState->mObjectManager->mTiles->TileWidth * 32) * 50.0f;
			Y = Y/float(mApp->mGameState->mObjectManager->mTiles->TileHeight * 32) * 50.0f;

			mMinimapSprite->PlayAnimation(MINIMAP_PLANT);
			mMinimapSprite->Render(x + X * 3, y + Y * 3);
		}

	for (int i = 0; i < ENTITY_LISTSIZE; i ++)
	{
		//render the predators..
		if (mApp->mGameState->mPredators[i]->GetAlive())
		{
			//this predator is alive.. get it's x and y and render it..
			float X = ((Object2D*)mApp->mGameState->mPredators[i])->GetX();
			float Y = mApp->mGameState->mPredators[i]->GetY();

			X = X/float(mApp->mGameState->mObjectManager->mTiles->TileWidth * 32) * 50.0f;
			Y = Y/float(mApp->mGameState->mObjectManager->mTiles->TileHeight * 32) * 50.0f;

			mMinimapSprite->PlayAnimation(MINIMAP_PREDATOR);
			mMinimapSprite->Render(x + X * 3, y + Y * 3);
		}

		//render the prey..
		if (mApp->mGameState->mPrey[i]->GetAlive())
		{
			//this predator is alive.. get it's x and y and render it..
			float X = ((Object2D*)mApp->mGameState->mPrey[i])->GetX();
			float Y = mApp->mGameState->mPrey[i]->GetY();

			X = X/float(mApp->mGameState->mObjectManager->mTiles->TileWidth * 32) * 50.0f;
			Y = Y/float(mApp->mGameState->mObjectManager->mTiles->TileHeight * 32) * 50.0f;

			mMinimapSprite->PlayAnimation(MINIMAP_PREY);
			mMinimapSprite->Render(x + X * 3, y + Y * 3);
		}

		//render chuck norris...
		if (mApp->mGameState->mChucklets[i]->GetAlive())
		{
			//this predator is alive.. get it's x and y and render it..
			float X = ((Object2D*)mApp->mGameState->mChucklets[i])->GetX();
			float Y = mApp->mGameState->mChucklets[i]->GetY();

			X = X/float(mApp->mGameState->mObjectManager->mTiles->TileWidth * 32) * 50.0f;
			Y = Y/float(mApp->mGameState->mObjectManager->mTiles->TileHeight * 32) * 50.0f;

			mMinimapSprite->PlayAnimation(MINIMAP_CHUCK);
			mMinimapSprite->Render(x + X * 3, y + Y * 3);
		}

		if (mApp->mGameState->mMoose[i]->GetAlive())
		{
			//this predator is alive.. get it's x and y and render it..
			float X = ((Object2D*)mApp->mGameState->mMoose[i])->GetX();
			float Y = mApp->mGameState->mMoose[i]->GetY();

			X = X/float(mApp->mGameState->mObjectManager->mTiles->TileWidth * 32) * 50.0f;
			Y = Y/float(mApp->mGameState->mObjectManager->mTiles->TileHeight * 32) * 50.0f;

			mMinimapSprite->PlayAnimation(MINIMAP_MOOSE);
			mMinimapSprite->Render(x + X * 3, y + Y * 3);
		}
	}

	//render the current scope..
	mCurView->Render(x + mApp->mGameState->mObjectManager->mScrollX/32 * 3, y + mApp->mGameState->mObjectManager->mScrollY/32 * 3);
}