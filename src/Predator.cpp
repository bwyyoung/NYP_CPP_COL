#include "Predator.h"
#include "BaseApp.h"
#include "Macs.h"
#include "GameState.h"
#include "Image.h"

Predator::Predator(SimianBase * theApp, char * theName) : Entity(theApp, theName)
{
	SetType(ET_PREDATOR);	// First, you have to set the object type so that other objects can recognise it..
	SetState(ES_IDLE);		// Next you have to set it's default state
	SetFrames(4, 6);		// Next set the number of frames in the sprite sheet (rows and columns like popcap)

	//Animation information according to sprite sheet (where 0 is the first frame)
	int RunningLeft[] = {2, 1, 0};int RunningRight[] = {9, 10, 11};int RunningUp[] = {21, 22, 23};int RunningDown[] = {15, 16, 17};
	int RunningLengths[] = {3,3,3,3};

	int WalkingLeft[] = {5, 4, 5, 3}; int WalkingRight[] = {6, 7, 6, 8}; int WalkingUp[] = { 18, 19, 18, 20 }; int WalkingDown[] = { 12, 13, 12, 14 };
	int WalkingLengths[] = {4,4,4,4};

	int StandingLeft[] = { 5 }; int StandingRight[] = { 6 }; int StandingUp[] = { 18 }; int StandingDown[] = { 12 };
	int StandingLengths[] = {1,1,1,1};

	// Create the animations with the previously defined arrays.
	ANIMATION_RUNNING = AddAnimation(RunningUp, RunningDown, RunningLeft, RunningRight, RunningLengths);
	ANIMATION_WALKING = AddAnimation(WalkingUp, WalkingDown, WalkingLeft, WalkingRight, WalkingLengths);
	ANIMATION_STANDING = AddAnimation(StandingUp, StandingDown, StandingLeft, StandingRight, StandingLengths);
	SetAnimationSpeed(200);				// Set the animation speed (time in milliseconds between frames)
	PlayAnimation(ANIMATION_WALKING);	// Play the default animation.

	mWanderStep = 50;					// Set the wander steps, this is the distance of each stride (before the creature pauses) (you may modify this dynamically to increase the distance when hunting)
	mPauseTime = 1000;					// The time for the creature to pause. (after each stride creature will stop moving to recover strength)
	mMoveSpeed = 0.5;					// The movement speed. (how much to increment the X/Y in 1 step) (you can increase this dynamically to make creatures move faster e.g. when it spots prey).
	mSearchDistance = 200;				// How far ahead of the creature it can see.
	mSearchAngle = 60;					// How wide it's scope is (in degrees)
	mTargetType = ET_PREDATOR;			// What it's current target is (To change it's current target use the SetPreyList function)
	

	mHungerTimer = 1000;
}

Predator::~Predator()
{

}

void Predator::Update()
{
	Entity::Update(); // you must call this if you want your entity to age.

	if (!mAlive)
		return;

	//Check if predator is going to die
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

void Predator::Render()
{
	Entity::Render(); // you must call this if you want your entity to render correctly.
}

void Predator::FSM()
{
	// Create your own FSM in this function. You must inherit it as it is abstract (automatically calls in the update loop, so don't call it manually).
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
	}
}
void Predator::Idle()
{
	Wander();		// Moves around based on the values set in the constructor.

	if (mHunger < 50)
	{
		mTargetType = ET_PREY;
		SetPrey((Entity **)((BaseApp*)mApp)->mGameState->mPrey);
		mWanderStep = 300;
		SetState(ES_HUNTING);
	}

	if (mAge > 20 && mHunger > 50 && !mSterile)
	{
		//Trigger stalking..
		SetPrey((Entity **)((BaseApp *)mApp)->mGameState->mPredators);
		mTargetType = ET_PREDATOR;
		mWanderStep = 300;
		SetState(ES_STALKING);
	}
}

void Predator::Hunting()
{
	Wander();
	Search();		// Call this to search for your prey (only works if SetPreyList has been called successfully).

	// Check if the entity found a suitable target.
	if (mCurTarget) 
	{
		SetState(ES_ATTACK);						// Switch its state to attack
		mMoveSpeed = 2.5;								// Increase it's movement speed
		SetAnimationSpeed(100);						// Decreases the animation's time between frames (makes the animation play faster)
		PlayAnimation(ANIMATION_RUNNING, true);		// Plays the running animation (must be set in constructor for this to work)
	}
}

void Predator::Attack()
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

void Predator::Stalking()
{
	Wander();
	Search();

	if (mHunger < 50)
	{
		mTargetType = ET_PREY;
		SetPrey((Entity **)((BaseApp*)mApp)->mGameState->mPrey);
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

void Predator::Mating()
{
	if (mHunger < 50)
	{
		mTargetType = ET_PREY;
		SetPrey((Entity **)((BaseApp*)mApp)->mGameState->mPrey);
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

void Predator::Reset()
{
	Entity::Reset();
	SetState(ES_IDLE);					// Next you have to set it's default state
	mWanderStep = 50;					// Set the wander steps, this is the distance of each stride (before the creature pauses) (you may modify this dynamically to increase the distance when hunting)
	mPauseTime = 1000;					// The time for the creature to pause. (after each stride creature will stop moving to recover strength)
	mMoveSpeed = 0.5;						// The movement speed. (how much to increment the X/Y in 1 step) (you can increase this dynamically to make creatures move faster e.g. when it spots prey).
	mSearchDistance = 200;				// How far ahead of the creature it can see.
	mSearchAngle = 60;					// How wide it's scope is (in degrees)
	mTargetType = ET_PREDATOR;			// What it's current target is (To change it's current target use the SetPreyList function)

	mHungerTimer = 1000;
	mSterile = false;
}

void Predator::Caught()
{
	// this function is abstract and must be overridden.
	// this is called when the entity successfully chases a prey.
	if (mTargetType == ET_PREY)
	{
		mCurTarget->SetAlive(false);
		mCurTarget = 0;
		mHunger = 100;
	}
	else if (mTargetType == ET_PREDATOR)
	{
		//predator starts mating..
		//produce 2 - 4 new predators
		int Spawn = rand()%2;
		Spawn += 1;

		// Create new predators..
		for (int i = 0; i < Spawn; i ++)
			for (int j = 0; j < ENTITY_LISTSIZE; j ++)
			{
				if (((BaseApp*)mApp)->mGameState->mPredators[j]->mAlive == false)
				{
					// Reset the predator..
					((BaseApp*)mApp)->mGameState->mPredators[j]->Reset();
					int Sex = rand()%2;

					if (Sex)
						((BaseApp*)mApp)->mGameState->mPredators[j]->mGender = true;
					else
						((BaseApp*)mApp)->mGameState->mPredators[j]->mGender = false;

					((BaseApp*)mApp)->mGameState->mPredators[j]->SetPosition(mX, mY);
					break;
				}
			}
	}
}

bool Predator::SearchConditionCheck(Entity * curTarget)
{
	if (mTargetType == ET_PREY)
		return true;
	else if (mTargetType == ET_PREDATOR)
	{
		if (curTarget->GetAge() > 20 && curTarget->GetGender() != mGender)
			//Suitable target
			return true;
		else
			return false;
	}

	return true;
}
