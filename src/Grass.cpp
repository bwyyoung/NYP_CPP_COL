#include "Grass.h"
#include "BaseApp.h"
#include "Macs.h"
#include "GameState.h"

#include <iostream>
using namespace std;



Grass::Grass(SimianBase * theApp, char * theName) : Entity(theApp, theName)
{
	SetType(ET_GRASS);
	SetState(ES_IDLE);

	SetFrames(2,3);
	//Growing Sprites.
	AnimationSwitch = 0;

	//Animation information for the grass.
	int SeedlingGrowing[] = { 3 ,3 ,3 };
	int SeedlingGrowingLengths[] = {3 ,3, 3, 3};

	int SproutGrowing[] = { 4, 4, 4};
	int SproutGrowingLengths[] = { 3, 3, 3, 3};

	int YoungGrowing[] = { 5, 5, 5};
	int YoungGrowingLengths[] = { 3, 3, 3, 3};

	int Eating[] = { 6 , 5 , 4};
	int EatingLengths[] = {3 ,3 ,3,3};

	int Grown[] = {0,1};
	int GrownLengths[] = {2,2,2,2};

	//Creating Animations for the grass.
	ANIMATION_SEEDGROWING	=	AddAnimation(SeedlingGrowing,SeedlingGrowing,SeedlingGrowing,	SeedlingGrowing,SeedlingGrowingLengths);
	ANIMATION_SPROUTGROWING =	AddAnimation(SproutGrowing,	 SproutGrowing,	 SproutGrowing,		SproutGrowing,  SproutGrowingLengths);
	ANIMATION_YOUNGGROWING	=	AddAnimation(YoungGrowing,	 YoungGrowing,	 YoungGrowing,		YoungGrowing,   YoungGrowingLengths);
	ANIMATION_EATING		=	AddAnimation(Eating,		 Eating,		 Eating,			Eating,			EatingLengths);
	ANIMATION_GROWN			=	AddAnimation(Grown,			 Grown,			 Grown,				Grown,			GrownLengths);

	SetAnimationSpeed(500);
	PlayAnimation(ANIMATION_SEEDGROWING,true);

	//Other variables.
	PastMaturity =false;
	EatenStateSwtich = false;
	OccupiedStateSwitch = false;

	//Eaten Function vairbles.
	IsEaten = false;
	IsOccupied = false;
	FoodLevel = 10;
	FoodLimit = 0;
	MaxFoodLimit = 120;
	EatingFoodLimitStage1 = 10;
	EatingFoodLimitStage2 = 50;
	EatingFoodLimitStage3 = 100;
	AgeUp = 1000;
	FoodDeath = 0;

}

Grass::~Grass()
{
}
bool Grass::GetEaten()
{
	return IsEaten;
}
bool Grass::GetOccupied()
{
	return IsOccupied;
}
bool Grass::GetMature()
{
	return PastMaturity;
}



void Grass::Update()
{
	Entity::Update();

	if (!mAlive)
	{
		return;
	}

	

	/*(BaseApp*)mApp)->mGameState->;*/
	//Check if the plant is alive. ( This is the plant's age limit condition).
	/*if (mApp->mGameState->mEnvironment->GetState())
	{
		cout << "Please Please work!" <<endl;
	}*/


	mState =((BaseApp*)mApp)->mGameState->mEnvironment->GetEnvState();

	if (mState == esRainMedium || mState == esRainLight || mState == esRainHeavy)
	{
		mAge = mAge + 1;
		//cout << "The Grass knows it is raining!! woot!" <<endl;
	}
	if (mAge > 80)
	{
		if ( FoodLevel <= 0)
		{
			SetAlive(false);
		}
	}
}
void Grass::Render()
{
	Entity::Render();
}

void Grass::GrowState()
{
	if (FoodLevel < MaxFoodLimit)
		Recover();
	if (FoodLevel > MaxFoodLimit)
		FoodLevel = MaxFoodLimit;

	if ( mAge < 2 )
	{
		AnimationSwitch = 0;
	}
	else if (mAge >= 2 && mAge < 4 )
	{
		AnimationSwitch = 1;
	}
	else if (mAge >= 4 && mAge < 8)
	{	
		AnimationSwitch =2;	
	}
	else if (mAge >= 8)
	{
		SetAnimationSpeed(500);		
		PlayAnimation(ANIMATION_GROWN,true);
		SetState(ES_REPRODUCTION);
		PastMaturity = true;
		AnimationSwitch = 3;
	}



}
void Grass::ReproductionState()
{	

	//Creates 2 - 6 new plants surrounding the parent plant.
	int Spawn = rand()%5;
	Spawn +=1;

	for (int i = 0; i < Spawn; i ++)
	{
		for (int j = 0; j < ENTITY_LISTSIZE; j ++)
		{
			if (((BaseApp*)mApp)->mGameState->mGrass[j]->mAlive == false)
			{
				float x = (float (rand()%1600)); //rand()%(100) + mX+100%2;
				float y = (float (rand()%1600)); //rand()%(100) + mY+(rand()%(100))%2;

				((BaseApp*)mApp)->mGameState->mGrass[j]->Reset();

				((BaseApp*)mApp)->mGameState->mGrass[j]->SetPosition(x, y); //Must find a way to change the placement of the grass

				break;
			}
		}
	}


}
void Grass::FSM()
{	
	AnimationState();
	switch ( mCurState )
	{
	case ES_IDLE:
		GrowState();
		break;
	case ES_EATEN:
		EatenState();
		break;
	case ES_REPRODUCTION:
		ReproductionState();
		break;
	}
}
void Grass::Reset()
{
	Entity::Reset();

	//Other variables.
	PastMaturity =false;
	EatenStateSwtich = false;
	OccupiedStateSwitch = false;

	//Eaten Function vairbles.
	IsEaten = false;
	IsOccupied = false;
	SetState(ES_IDLE);

}
void Grass::AnimationState()
{
	switch(AnimationSwitch)
	{
	case 0:
		PlayAnimation(ANIMATION_SEEDGROWING,false);
		break;
	case 1:
		PlayAnimation(ANIMATION_SPROUTGROWING,false);
		break;
	case 2:
		PlayAnimation(ANIMATION_YOUNGGROWING,false);
		break;
	case 3:
		//SetAnimationSpeed(500);		
		//PlayAnimation(ANIMATION_GROWN,true);
		//Animation here got a problem. It will not loop.
		break;

	}
}




void Grass::Recover()
{
	TempAgeCounter+=mApp->mTimeDelta;
	if (TempAgeCounter>=AgeUp)
	{
		TempAgeCounter=0;
		if (FoodLevel<MaxFoodLimit)
		{
			FoodLevel+=5;
		}
		else if (FoodLevel>MaxFoodLimit)
		{
			FoodLevel=MaxFoodLimit;
		}
	}
}

void Grass::SetEating(bool thestate)
{
	IsEaten = thestate; 
	if (IsEaten)
	{
		SetState(ES_EATEN);
	}
	else
	{
		SetState(ES_IDLE);
	}

}
void Grass::SetOccupied(bool thestate)
{
	IsOccupied = thestate;
}
void Grass::EatenState()
{
	if (!IsEaten)
	{
		IsOccupied = false;
		SetState(ES_IDLE);
	}
	TempAgeCounter += mApp->mTimeDelta;
	if (TempAgeCounter>=AgeUp)
	{
		TempAgeCounter = 0;
		if (FoodLevel<=FoodDeath)
		{
			mAlive=false;
		}
		else
		{
				FoodLevel-=10;
		}
	}

	if (FoodLevel < EatingFoodLimitStage3 && FoodLevel > EatingFoodLimitStage2 )
	{
		AnimationSwitch = 2;
	}
	else if (FoodLevel < EatingFoodLimitStage2 && FoodLevel > EatingFoodLimitStage1)
	{
		AnimationSwitch = 1;
	}
	else if (FoodLevel < EatingFoodLimitStage1)
	{
		AnimationSwitch = 0;
	}
	else
	{
		AnimationSwitch = 3;
	}
}


int Grass::GetFoodMax()
{
	return MaxFoodLimit;
}
//Blank Functions (ignore them).
void Grass::Caught()
{
}
bool Grass::SearchConditionCheck(Entity *curTarget)
{
	return true;
}
