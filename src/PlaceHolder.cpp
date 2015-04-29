#include "PlaceHolder.h"

PlaceHolder::PlaceHolder(BaseApp * theApp, Image * theImage):Entity((SimianBase*)theApp, theImage)
{
	SetState(ES_IDLE);
	SetAlive(true);
}

PlaceHolder::~PlaceHolder()
{

}

void PlaceHolder::SetAnimation(int AnimationID, Entity_Type theType)
{
	SetType(theType);

	if (theType == ET_PREDATOR)
	{
		SetFrames(4, 6);

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
		mWanderStep = 50;
	}
	else if (theType == ET_PREY)
	{
		SetFrames(9,6);

		int RunningLeft[] = {8,7,6}; int RunningRight[] = {2,3,4}; int RunningUp[]={13,12,14}; int RunningDown[] = {16,15,17};
		int RunningLengths[]={3,3,3,3};

		int WalkingLeft[] = {10,8,10,9}; int WalkingRight[] = {0,2,0,1}; int WalkingUp[]={13,12,14,12}; int WalkingDown[] = {16,15,17,15}; 
		int WalkingLengths[] = {4,4,4,4};

		int StandingLeft[] = {10}; int StandingRight[]={0}; int StandingUp[] = {12}; int StandingDown[] = {15}; 
		int StandingLengths[] = {1,1,1,1};

		ANIMATION_RUNNING = AddAnimation(RunningUp, RunningDown, RunningLeft, RunningRight, RunningLengths);
		ANIMATION_WALKING = AddAnimation(WalkingUp, WalkingDown, WalkingLeft, WalkingRight, WalkingLengths);
		ANIMATION_STANDING = AddAnimation(StandingUp, StandingDown, StandingLeft, StandingRight, StandingLengths);
		SetAnimationSpeed(200);
		mWanderStep = 50;
	}
	else if (theType == ET_GRASS)
	{
		SetFrames(2, 3);

		int Twitch[] = {0, 1};
		int TwitchLengths[] = {2,2,2,2};

		ANIMATION_STANDING = AddAnimation(Twitch, Twitch, Twitch, Twitch, TwitchLengths);
		mWanderStep = 0;
	}

						// Set the wander steps, this is the distance of each stride (before the creature pauses) (you may modify this dynamically to increase the distance when hunting)
	mPauseTime = 1000;					// The time for the creature to pause. (after each stride creature will stop moving to recover strength)
	mMoveSpeed = 0.5;					// The movement speed. (how much to increment the X/Y in 1 step) (you can increase this dynamically to make creatures move faster e.g. when it spots prey).
	mSearchDistance = 200;				// How far ahead of the creature it can see.
	mSearchAngle = 60;					// How wide it's scope is (in degrees)
	mTargetType = ET_PREDATOR;			// What it's current target is (To change it's current target use the SetPreyList function)
}

void PlaceHolder::Update()
{
	Entity::Update();
	Wander();
}

void PlaceHolder::Render()
{
	Entity::Render();
}

void PlaceHolder::FSM()
{

}

void PlaceHolder::Caught()
{

}

bool PlaceHolder::SearchConditionCheck(Entity * curTarget)
{
	return true;
}

void PlaceHolder::Reset()
{

}