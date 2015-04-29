#include "ObjectManager.h"
#include "RenderObject.h"
#include "Predator.h"
#include "BaseApp.h"
#include "Input.h"
#include "TileGenerator.h"
#include "Entity.h"
#include "StatusPane.h"
#include "KeyboardInput.h"
#include "GameState.h"
#include <algorithm>

Entity * mCurSelection = 0;
bool mMouseLastState = false;

ObjectManager::ObjectManager(BaseApp * theApp)
{
	mScrollX = mScrollY = 0;
	mApp = theApp;
	PickingCreature = false;
}

ObjectManager::~ObjectManager()
{

}

bool SortDepth(RenderObject * a, RenderObject * b)
{
	if (a->GetY() < b->GetY())
		return true;
	else
		return false;
}

void ObjectManager::AddObject(RenderObject * theObject)
{
	mObjects.push_back(theObject);
}


#include <iostream>

using namespace std;

void ObjectManager::UpdateObjects()
{
	mTiles->Update();

	for (int i = 0; i < (int)mObjects.size(); i ++)
	{
		mObjects[i]->Update();
		if (mObjects[i]->GetX() > mTiles->GetWidth() - mObjects[i]->GetWidth())
			mObjects[i]->SetX((float)mTiles->GetWidth() - mObjects[i]->GetWidth());
		else if (mObjects[i]->GetX() < 0)
			mObjects[i]->SetX(0);

		if (mObjects[i]->GetY() > mTiles->GetHeight() - mObjects[i]->GetHeight())
			mObjects[i]->SetY((float)mTiles->GetHeight() - mObjects[i]->GetHeight());
		else if (mObjects[i]->GetY() < 0)
			mObjects[i]->SetY(0);
	}
}

void ObjectManager::Update()
{
	//check mouse position..
	int MouseX, MouseY;
	mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);

	if (MouseX > 790)
		mScrollX += 20;
	else if (MouseX < 10)
		mScrollX -= 20;
	if (MouseY > 590)
		mScrollY += 20;
	else if (MouseY < 10)
		mScrollY -= 20;

	// do keyboard scrolling..
	if (mApp->mKeyboardInput->GetKeyDown(VK_LEFT))
		mScrollX -= 20;
	if (mApp->mKeyboardInput->GetKeyDown(VK_RIGHT))
		mScrollX += 20;
	if (mApp->mKeyboardInput->GetKeyDown(VK_UP))
		mScrollY -= 20;
	if (mApp->mKeyboardInput->GetKeyDown(VK_DOWN))
		mScrollY += 20;
	if (mApp->mKeyboardInput->GetKeyDown(VK_DELETE))
	{
		if (mCurSelection)
		{
			mCurSelection->SetAlive(false);
			mCurSelection->Deselect();
			mCurSelection = 0;
			mApp->mGameState->mStatusPane->SetEntity(0);
		}

		//mApp->CleanoutState(CSGameState, CSTitleState);
	}


	if (mScrollX < 0)
		mScrollX = 0;
	if (mScrollY < 0)
		mScrollY = 0;

	if (mScrollX > mTiles->GetWidth() - 800)
		mScrollX = mTiles->GetWidth() - 800;
	if (mScrollY > mTiles->GetHeight() - 600)
		mScrollY = mTiles->GetHeight() - 600;

	mTiles->Update();

	for (int i = 0; i < (int)mObjects.size(); i ++)
	{
		mObjects[i]->Update();
		if (mObjects[i]->GetX() > mTiles->GetWidth() - mObjects[i]->GetWidth())
			mObjects[i]->SetX((float)mTiles->GetWidth() - mObjects[i]->GetWidth());
		else if (mObjects[i]->GetX() < 0)
			mObjects[i]->SetX(0);

		if (mObjects[i]->GetY() > mTiles->GetHeight() - mObjects[i]->GetHeight())
			mObjects[i]->SetY((float)mTiles->GetHeight() - mObjects[i]->GetHeight());
		else if (mObjects[i]->GetY() < 0)
			mObjects[i]->SetY(0);
	}

	if (mCurSelection)
		if (mCurSelection->GetAlive() == false)
			mApp->mGameState->mStatusPane->SetEntity(0);
}

void ObjectManager::Select()
{
	int MouseX, MouseY;
	mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);

	if (mApp->mMouseInput->GetButtonDown(LBUTTON))
	{
		if (mMouseLastState && mApp->mGameState->Mode == 0)
			return;

		if (PickingCreature == true)
		{
			mCurSelection->SetX((float)MouseX - mCurSelection->GetWidth()/2 + mScrollX);
			mCurSelection->SetY((float)MouseY - mCurSelection->GetHeight()/2 + mScrollY);
		}

		if (!PickingCreature)
		{
			for (int i = 0; i < (int)mObjects.size(); i ++)
			{
				//check if the mouse selected anything..
				if (((Entity *)mObjects[i])->GetAlive())
					if (MouseX > mObjects[i]->GetX() - mScrollX && MouseX < mObjects[i]->GetX() - mScrollX + mObjects[i]->GetWidth())
						if (MouseY > mObjects[i]->GetY() - mScrollY && MouseY < mObjects[i]->GetY() - mScrollY + mObjects[i]->GetHeight())
						{
							((Entity *)mObjects[i])->Select();

							if (mCurSelection != (Entity *)mObjects[i] && mCurSelection)
								mCurSelection->Deselect();

							mCurSelection = (Entity *)mObjects[i];
							mApp->mGameState->mStatusPane->SetEntity(mCurSelection);

							mMouseLastState = true;

							if (mApp->mGameState->Mode == 1)
							{
								//pick up the creature..
								PickingCreature = true;
							}
							return;
						}
			}

			//didnt select anything
			if (mCurSelection)
				mCurSelection->Deselect();

			mCurSelection = 0;
			mApp->mGameState->mStatusPane->SetEntity(0);
		}
	}
	else
	{
		mMouseLastState = false;
		PickingCreature = false;
	}
}

void ObjectManager::Render()
{
	mTiles->SetPosition((float)-mScrollX, (float)-mScrollY);
	mTiles->Render();
	//mTiles->SetPosition(mTiles->GetX() + mScrollX, mTiles->GetY() + mScrollY);
	sort(mObjects.begin(), mObjects.end(), SortDepth);
	for (int i = 0; i < (int)mObjects.size(); i ++)
	{
		float X = mObjects[i]->GetX();
		float Y = mObjects[i]->GetY();
		mObjects[i]->Render(X - (float)mScrollX, Y - (float)mScrollY);
	}
}

void ObjectManager::AddTiledMap(TileGenerator *theTiles)
{
	mTiles = theTiles;
}