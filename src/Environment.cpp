#include "Environment.h"
#include "Grass.h"
#include "Prey.h"
#include "Moose.h"
#include "Predator.h"
#include "GameState.h"
#include "BaseApp.h"
#include "Macs.h"
#include "ObjectManager.h"

#include <iostream>
using namespace std;


#define DROPS 2000
#define SPAWN_TIME 50
#define RAIN_SPEED 8
#define DROPS_PER_VOLLEY 0

Environment::Environment(BaseApp * theApp)
{
	mApp = theApp;
	rain = new rainDrop[DROPS];
	mBlackMask = mRainDroplet = 0;
	for (int i = 0; i < DROPS; i ++)
	{
		rain[i].x = 0;
		rain[i].y = 0;
		rain[i].render = false;
	}
	mTimePassed = 0;
	mDayTimer = 0;

	mSpawnDropTime = SPAWN_TIME;
	mRainSpeed = RAIN_SPEED;
	mDropsPerVolley = DROPS_PER_VOLLEY;
	mLightningDeadly = false;
	mLightningTime = 0;
	mCurLightningTime = 0;
	mLightningAppearTime = 0;
	SetEnvState(esClear);
	WaterPlants = false;
	curNightAlphaVal = 0.0f;
}

Environment::~Environment()
{
	delete mRainDroplet;
	delete mBlackMask;
	delete mWhiteMask;
	delete mLightning;
	delete [] rain;
}

void Environment::Init()
{
	mRainDroplet = mApp->CreateImage("../data/Rain.png");
	mBlackMask = mApp->CreateImage("../data/black.png");
	mBlackMask->SetSize(800, 600);
	mWhiteMask = mApp->CreateImage("../data/white.png");
	mWhiteMask->SetSize(800, 600);
	mWhiteMask->SetColor(Color(255,255,255,0));
	mLightning = mApp->CreateImage("../data/Lightning.png");
	mLightning->SetColor(Color(255,255,255,0));
}

void Environment::Update()
{
	mTimePassed += mApp->mTimeDelta;
	mDayTimer += mApp->mTimeDelta;

	if (mTimePassed > mSpawnDropTime)
	{
		// spawn a new rain drop.
		for (int j = 0; j < mDropsPerVolley; j ++)
			for (int i = 0; i < DROPS; i ++)
				if (rain[i].render == false)
				{
					rain[i].y = -3;
					rain[i].x = rand()%800;
					rain[i].render = true;
					break;
				}
	}

	for (int i = 0; i < DROPS; i ++)
	{
		if (rain[i].render)
			rain[i].y += mRainSpeed;

		if (rain[i].y > 600)
			rain[i].render = false;
	}

	if (mLightningTime > 0)
	{
		mCurLightningTime -= mApp->mTimeDelta;
		if (mCurLightningTime <= 0)
		{
			mCurLightningTime = mLightningTime - 1000 + rand()%2000;
			Lightning();
		}

		LightningUpdate();
	}
	if ( mDayTimer > 30000 && mDayTimer <= 40000)
	{
		SetEnvState(esEveningTime);
	}
	if ( mDayTimer > 40000 && mDayTimer <= 70000)
	{
		SetEnvState(esNight);
	}
	if (mDayTimer > 70000 && mDayTimer <= 80000)
	{
		SetEnvState(esEveningTime);
	}
	else if ( mDayTimer > 80000)
	{
		SetEnvState(esClear);
		mDayTimer = 0;
	}
}

void Environment::Render(float x, float y)
{
	for (int i = 0; i < DROPS; i ++)
	{
		if (rain[i].render)
		{
			mRainDroplet->Render((float)rain[i].x, (float)rain[i].y);
		}
	}
	mBlackMask->Render(x, y);
	mLightning->Render();
	mWhiteMask->Render(x, y);
}

void Environment::Render()
{
	Render(mX, mY);
}

void Environment::SetEnvState(EnvironmentState S)
{
	mState = S;
	CreateEnvironmentEffect(S);
}

EnvironmentState Environment::GetEnvState()
{
	return mState;
}

void Environment::CreateEnvironmentEffect(EnvironmentState es)
{
	if (es == esClear)
	{
		mDropsPerVolley = 0;
		SetDarkness(0);
		Lightning(false, 0);
		WaterPlants = false;
	}
	if (es == esRainLight)
	{
		mDropsPerVolley = 1;
		SetDarkness(0.1f);
		Lightning(false, 0);
		WaterPlants = false;
	}
	if (es == esRainMedium)
	{
		mDropsPerVolley = 3;
		SetDarkness(0.5f);
		Lightning(false, 10000);
		WaterPlants = true;

	}
	if (es == esRainHeavy)
	{
		mDropsPerVolley = 5;
		SetDarkness(1.0f);
		Lightning(true, 3000);
		WaterPlants = false;

	}
	if (es == esNight)
	{
		mDropsPerVolley = 0;
		SetDarkness(1.0f);
		Lightning(false, 0);
		WaterPlants = false;	
	}
	if (es == esEveningTime)
	{
		mDropsPerVolley = 0;
		SetDarkness(curNightAlphaVal);
		Lightning(false,0);
		WaterPlants = false;
	}
}

void Environment::SetDarkness(float perc)
{
	if (mBlackMask)
		mBlackMask->SetColor(Color(255,255,255,(unsigned char)(perc * 80.0f)));
}

void Environment::Lightning(bool KillCreatures, int LightningTime)
{
	mLightningTime = LightningTime;
	mCurLightningTime = LightningTime;
	mLightningDeadly = KillCreatures;
}

void Environment::Lightning()
{
	mLightningUpdating = true;
	mLightningAlpha = 0.0f;
	mLightningAppearTime = 800;
	
	int random = rand()%3;
	if (random == 0)
	{
		random = rand()%ENTITY_LISTSIZE;
		mLightningTarget = mApp->mGameState->mPrey[random];
	}
	else if (random == 1)
	{
		random = rand()%ENTITY_LISTSIZE;
		mLightningTarget = mApp->mGameState->mPredators[random];
	}
	else if (random == 2)
	{
		random = rand()%ENTITY_LISTSIZE;
		mLightningTarget = mApp->mGameState->mMoose[random];
	}
}

void Environment::LightningUpdate()
{
	if (mLightningUpdating)
		mLightningAlpha += 30.0f;
	else
		mLightningAlpha -= 10.0f;

	if (mLightningAlpha < 0.0f)
		mLightningAlpha = 0.0f;
	else if (mLightningAlpha > 255.0f)
	{
		mLightningAlpha = 255.0f;
		mLightningUpdating = false;
	}

	mWhiteMask->SetColor(Color(255,255,255,(unsigned char)mLightningAlpha));

	if (mLightningAppearTime > 0 && mLightningDeadly)
	{
		mLightningAppearTime -= mApp->mTimeDelta;
		float loffsetx = -100.0f;
		float loffsety = -420.0f;
		mLightning->SetColor(Color(255,255,255,255));
		mLightning->SetPosition(mLightningTarget->GetX() - mApp->mGameState->mObjectManager->mScrollX + loffsetx, mLightningTarget->GetY() - mApp->mGameState->mObjectManager->mScrollY + loffsety);

		if (mLightningAppearTime < 0)
		{
			mLightning->SetColor(Color(255,255,255,0));
			mLightningAppearTime = 0;
			mLightningTarget->SetAlive(false);
		}
	}
}