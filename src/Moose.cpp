#include "Moose.h"
#include "BaseApp.h"
#include "GameState.h"
#include "Macs.h"
#include "MessageBoard.h"
#include <../Include/math.h>

Moose::Moose(SimianBase * theApp, char * theName) : Entity(theApp, theName)
{
	SetType(ET_MOOSE);
	SetState(ES_IDLE);
	SetFrames(6, 3);

	int WalkLeft[]		= {0,1,0,2};
	int WalkRight[]		= {5,3,5,4};
	int WalkUp[]		= {9,10,9,11};
	int WalkDown[]		= {6,7,6,8};
	int WalkLength[]	= {4,4,4,4};

	int StandLeft[]		= {0};
	int StandRight[]	= {5};
	int StandUp[]   	= {9};
	int StandDown[] 	= {6};
	int StandLength[]	= {1,1,1,1};

	int ChargeLeft[]	= {12, 13};
	int ChargeRight[]	= {15, 16};
	int ChargeUp[]		= {10, 11};
	int ChargeDown[]	= {7, 8};
	int ChargeLength[]  = {2,2,2,2};

	ANIMATION_RUNNING	= AddAnimation(WalkUp, WalkDown, WalkLeft, WalkRight, WalkLength);
	ANIMATION_STANDING	= AddAnimation(StandUp, StandDown, StandLeft, StandRight, StandLength);
	ANIMATION_WALKING	= AddAnimation(WalkUp, WalkDown, WalkLeft, WalkRight, WalkLength);
	ANIMATION_EATING	= AddAnimation(StandUp, StandDown, StandLeft, StandRight, StandLength);
	ANIMATION_CHARGING  = AddAnimation(ChargeUp, ChargeDown, ChargeLeft, ChargeRight, ChargeLength);

	SetAnimationSpeed(200);
	PlayAnimation(ANIMATION_WALKING);

	mWanderStep		= 50;
	mPauseTime		= 1000;
	mMoveSpeed		= 0.5;
	mSearchDistance = 200;
	mSearchAngle	= 60;
	mTargetType		= ET_GRASS;

	mHungerTimer = 1000;
}

Moose::~Moose()
{

}

void Moose::Update()
{
	Entity::Update();

	if (!mAlive)
		return;

	if (mAge > 40)
		SetAlive(false);

	mHungerTimer -= mApp->mTimeDelta;
	if (mHungerTimer < 0)
	{
		mHungerTimer = 1000;
		mHunger -= rand()%2 + 1;
		if (mHunger < 0)
			SetAlive(false);
	}
}

void Moose::Render()
{
	Entity::Render();
}

void Moose::FSM()
{
	switch (mCurState)
	{
	case ES_IDLE:
		Idle();
		break;
	case ES_HUNTING:
		Hunting();
		break;
	case ES_ATTACK:
		Attack();
		break;
	case ES_STALKING:
		Stalking();
		break;
	case ES_MATING:
		Mating();
		break;
	case ES_CHARGING:
		Charging();
		break;
	}
}

void Moose::Idle()
{
	Wander();		// Moves around based on the values set in the constructor.

	if (mHunger < 50)
	{
		mTargetType = ET_GRASS;
		SetPrey((Entity **)((BaseApp*)mApp)->mGameState->mGrass);
		mWanderStep = 300;
		SetState(ES_HUNTING);
	}

	if (mAge > 20 && mHunger > 50 && !mSterile)
	{
		//Trigger stalking..
		SetPrey((Entity **)((BaseApp *)mApp)->mGameState->mMoose);
		mTargetType = ET_MOOSE;
		mWanderStep = 300;
		SetState(ES_STALKING);
	}

	//collect message from the message board..
	MessageBoardMessages::iterator Pos;
	Message CurMessage = MessageBoard::GetBoard()->CollectMessage(Pos);
	
	if (CurMessage.TargetType == ET_MOOSE)
	{
		//dismiss the message if its a false alarm.
		if (CurMessage.Target->GetAlive() == false || CurMessage.Target->GetType() == ET_CHUCK)
		{
			MessageBoard::GetBoard()->DismissMessage(Pos);
			return;
		}

		//check the distance between this moose and the sender
		float dist = sqrt((GetX() - CurMessage.Poster->GetX()) * (GetX() - CurMessage.Poster->GetX()) + (GetY() - CurMessage.Poster->GetY()) * (GetY() - CurMessage.Poster->GetY()));

		if (dist < 300)
		{
			MessageBoard::GetBoard()->DismissMessage(Pos);
			mTargetType = ET_PREDATOR;
			mCurTarget = CurMessage.Target;
			mMoveSpeed = 2.5;								// Increase it's movement speed
			SetAnimationSpeed(100);						// Decreases the animation's time between frames (makes the animation play faster)
			PlayAnimation(ANIMATION_CHARGING, true);	
			SetState(ES_CHARGING);
		}
	}
}

void Moose::Hunting()
{
	Wander();
	Search();		// Call this to search for your prey (only works if SetPreyList has been called successfully).

	// Check if the entity found a suitable target.
	if (mCurTarget) 
	{
		SetState(ES_ATTACK);						// Switch its state to attack
		mMoveSpeed = 2.5;							// Increase it's movement speed
		SetAnimationSpeed(100);						// Decreases the animation's time between frames (makes the animation play faster)
		PlayAnimation(ANIMATION_RUNNING, true);		// Plays the running animation (must be set in constructor for this to work)
	}
}

void Moose::Charging()
{
	int ChaseResult = Chase();
	if (ChaseResult == 2)
	{
		//caught prey
		mCurTarget = 0;
		mMoveSpeed = 0.5;
		mWanderStep = 50;
		SetPrey(0);
		mCurrentStep = 0;
		mHunger = 100;
		mTargetType = ET_NONE;
		SetAnimationSpeed(200);
		SetState(ES_IDLE);
	}
	else if (ChaseResult == 0)
	{
		//caught prey
		mCurTarget = 0;
		mMoveSpeed = 0.5;
		mWanderStep = 50;
		SetPrey(0);
		mCurrentStep = 0;
		mHunger = 100;
		mTargetType = ET_NONE;
		SetAnimationSpeed(200);
		SetState(ES_IDLE);
	}
}

void Moose::Attack()
{
	// Calls chase every update loop
	// Chase() attempts to move after a target
	// Returns 2 if it caught the target
	// Returns 1 if it didnt catch the target but is still chasing it
	// Returns 0 if the target dies before it can catch it, or runs away.

	int ChaseResult = Chase();
	if (ChaseResult == 2)
	{
		//caught prey
		mCurTarget = 0;
		mMoveSpeed = 0.5;
		mWanderStep = 50;
		SetPrey(0);
		mCurrentStep = 0;
		mHunger = 100;
		mTargetType = ET_NONE;
		SetAnimationSpeed(200);
		SetState(ES_IDLE);
	}
	else if (ChaseResult == 0)
	{
		//prey got killed or ran away.
		mCurTarget = 0;
		mMoveSpeed = 0.5;
		mCurrentStep = 0;
		SetAnimationSpeed(200);
		SetState(ES_HUNTING);
	}
}

void Moose::Stalking()
{
	Wander();
	Search();

	if (mHunger < 50)
	{
		mTargetType = ET_GRASS;
		SetPrey((Entity **)((BaseApp*)mApp)->mGameState->mGrass);
		mWanderStep = 300;
		SetState(ES_HUNTING);
	}

	if (mCurTarget) 
	{
		SetState(ES_MATING);						// Switch its state to attack
		mMoveSpeed = 1.5;								// Increase it's movement speed
		SetAnimationSpeed(100);						// Decreases the animation's time between frames (makes the animation play faster)
		PlayAnimation(ANIMATION_WALKING);
	}
}

void Moose::Mating()
{
	if (mHunger < 50)
	{
		mTargetType = ET_GRASS;
		SetPrey((Entity **)((BaseApp*)mApp)->mGameState->mGrass);
		mWanderStep = 300;
		SetState(ES_HUNTING);
	}

	int ChaseResult = Chase();
	if (ChaseResult == 2)
	{
		mCurTarget = 0;
		mMoveSpeed = 0.5;
		mWanderStep = 50;
		SetPrey(0);
		mSterile = true;
		mTargetType = ET_NONE;
		mCurrentStep = 0;
		SetAnimationSpeed(200);
		SetState(ES_IDLE);
	}
	else if (ChaseResult == 0)
	{
		//mate got killed or ran away.
		mCurTarget = 0;
		mMoveSpeed = 0.5;
		mCurrentStep = 0;
		SetAnimationSpeed(200);
		SetState(ES_STALKING);
	}
}

void Moose::Reset()
{
	Entity::Reset();
	SetState(ES_IDLE);					// Next you have to set it's default state
	mWanderStep = 50;					// Set the wander steps, this is the distance of each stride (before the creature pauses) (you may modify this dynamically to increase the distance when hunting)
	mPauseTime = 1000;					// The time for the creature to pause. (after each stride creature will stop moving to recover strength)
	mMoveSpeed = 0.5;					// The movement speed. (how much to increment the X/Y in 1 step) (you can increase this dynamically to make creatures move faster e.g. when it spots prey).
	mSearchDistance = 200;				// How far ahead of the creature it can see.
	mSearchAngle = 60;					// How wide it's scope is (in degrees)
	mTargetType = ET_GRASS;				// What it's current target is (To change it's current target use the SetPreyList function)

	mHungerTimer = 1000;
	mSterile = false;
}

void Moose::Caught()
{
	// this function is abstract and must be overridden.
	// this is called when the entity successfully chases a prey.
	if (mTargetType == ET_GRASS)
	{
		mCurTarget->SetAlive(false);
		mCurTarget = 0;
		mHunger = 100;
	}
	else if (mTargetType == ET_PREDATOR)
	{
		mCurTarget->SetAlive(false);
		mCurTarget = 0;
	}
	else if (mTargetType == ET_MOOSE)
	{
		//predator starts mating..
		//produce 2 - 4 new Mooses
		int Spawn = rand()%2;
		Spawn += 1;

		// Create new Mooses
		for (int i = 0; i < Spawn; i ++)
			for (int j = 0; j < ENTITY_LISTSIZE; j ++)
			{
				if (((BaseApp*)mApp)->mGameState->mMoose[j]->mAlive == false)
				{
					// Reset the Mooses..
					((BaseApp*)mApp)->mGameState->mMoose[j]->Reset();
					int Sex = rand()%2;

					if (Sex)
						((BaseApp*)mApp)->mGameState->mMoose[j]->mGender = true;
					else
						((BaseApp*)mApp)->mGameState->mMoose[j]->mGender = false;

					((BaseApp*)mApp)->mGameState->mMoose[j]->SetPosition(mX, mY);
					break;
				}
			}
	}
}

bool Moose::SearchConditionCheck(Entity * curTarget)
{
	if (mTargetType == ET_GRASS)
		return true;
	else if (mTargetType == ET_MOOSE)
	{
		if (curTarget->GetAge() > 20 && curTarget->GetGender() != mGender)
			//Suitable target
			return true;
		else
			return false;
	}

	return true;
}