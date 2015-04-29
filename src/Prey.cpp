#include "Prey.h"
#include "BaseApp.h"
#include "Entity.h"
#include "Grass.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Macs.h"
using namespace std;

Prey::Prey(SimianBase * theApp, char * theName) : Entity(theApp, theName)
{
	AmLeader = false;
	MyLeader = NULL;
	SetFollowerState(FS_NORMAL);
	SetLeaderState(LS_NORMAL);
	SetMindState(MS_NORMAL);

	NumTargets=mNumPrey=ENTITY_LISTSIZE;
	SetType(ET_PREY);
	SetState(ES_IDLE);
	SetFrames(9, 6);

	LeaderRadius = 150;
	LeaderBuffer = 70;

	mWanderStep = 50;
	mPauseTime = 1000;
	mMoveSpeed = 1;
	mSearchDistance =150;
	mSearchAngle = 90;

	TimeOut=0;
	TheBound = 100;
	FaceUpdate = 0;
	BoundaryCheck = false;

	Radar.Ratio = 0.4f;
	Radar.Angle=0.0f;
	theHunger = 0;
	Radar.DashSpeed = 5.0f;
	Radar.Amplitude =1.5f;
	Radar.MinSpeed= 3.0f;
	mSterile=false;
	LeaderAlive = false;
	Radar.OldX = 0;
	Radar.OldY = 0;
	mAge = 0;
	mHunger = 55;
	TimerHunger=0;
	TimerHunger2=0;
	HungerThreshold = 50;
	HungerLimit = 100;
	HungerDeath=0;
	Maturity = 10;
	Death = 100;

}
Prey::~Prey()
{
}
int Prey::GetID()
{
	return ID;
}
void Prey::SetID(int  theID)
{
	ID = theID;
	if (AmLeader)
	{
		SetGender(true);
	}
	else
	{
		if (ID%2==0)
		{
			SetGender(false);
		}
		else
		{
			SetGender(true);
		}
	}

	
}
void Prey::MakeLeader(bool Lead)
{
	AmLeader = Lead;

	int SleepingLeft[] = {24,25};
	int SleepingRight[]={26,27};
	int SleepingUp[] = {28};
	int SleepingDown[] = {29};
	int SleepingLengths[] = {2,2,1,1};

	if (Lead)
	{
		int RunningLeft[] = {38,37,36}; int RunningRight[] = {32,33,34}; int RunningUp[]={43,42,44}; int RunningDown[] = {46,45,47};
		int RunningLengths[]={3,3,3,3};

		int WalkingLeft[] = {40,38,40,39}; int WalkingRight[] = {30,32,30,31}; int WalkingUp[]={43,42,44,42}; int WalkingDown[] = {46,45,47,45}; 
		int WalkingLengths[] = {4,4,4,4};

		int EatingLeft[]={48,49,48,49};
		int EatingRight[]={50,51,50,51};
		int EatingUp[]={52,53,52,53};
		int EatingDown[]={35,41,35,41};
		int EatingLengths[]={4,4,4,4};

		int StandingLeft[] = {40};
		int StandingRight[]={30};
		int StandingUp[] = {42};
		int StandingDown[] = {45};
		int StandingLengths[] = {1,1,1,1};
		ANIMATION_RUNNING = AddAnimation(RunningUp, RunningDown, RunningLeft, RunningRight, RunningLengths);
		ANIMATION_WALKING = AddAnimation(WalkingUp, WalkingDown, WalkingLeft, WalkingRight, WalkingLengths);
		ANIMATION_STANDING = AddAnimation(StandingUp, StandingDown, StandingLeft, StandingRight, StandingLengths);
		ANIMATION_EATING = AddAnimation(EatingUp,EatingDown,EatingLeft,EatingRight,EatingLengths);
		ANIMATION_SLEEPING = AddAnimation(SleepingUp, SleepingDown, SleepingLeft, SleepingRight, SleepingLengths);
	}
	else
	{
		int RunningLeft[] = {8,7,6}; int RunningRight[] = {2,3,4}; int RunningUp[]={13,12,14}; int RunningDown[] = {16,15,17};
		int RunningLengths[]={3,3,3,3};

		int WalkingLeft[] = {10,8,10,9}; int WalkingRight[] = {0,2,0,1}; int WalkingUp[]={13,12,14,12}; int WalkingDown[] = {16,15,17,15}; 
		int WalkingLengths[] = {4,4,4,4};

		int EatingLeft[]={18,19,18,19};
		int EatingRight[]={20,21,20,21};
		int EatingUp[]={22,23,22,23};
		int EatingDown[]={5,11,5,11};
		int EatingLengths[]={4,4,4,4};

		int StandingLeft[] = {10};
		int StandingRight[]={0};
		int StandingUp[] = {12};
		int StandingDown[] = {15};
		int StandingLengths[] = {1,1,1,1};
		ANIMATION_RUNNING = AddAnimation(RunningUp, RunningDown, RunningLeft, RunningRight, RunningLengths);
		ANIMATION_WALKING = AddAnimation(WalkingUp, WalkingDown, WalkingLeft, WalkingRight, WalkingLengths);
		ANIMATION_STANDING = AddAnimation(StandingUp, StandingDown, StandingLeft, StandingRight, StandingLengths);
		ANIMATION_EATING = AddAnimation(EatingUp,EatingDown,EatingLeft,EatingRight,EatingLengths);
		ANIMATION_SLEEPING = AddAnimation(SleepingUp, SleepingDown, SleepingLeft, SleepingRight, SleepingLengths);
	}
	
	SetAnimationSpeed(200);

	PlayAnimation(ANIMATION_WALKING);
}
bool Prey::CheckLeader()
{
	return AmLeader;
}
void Prey::Reset()
{

	SetFollowerState(FS_NORMAL);
	SetLeaderState(LS_NORMAL);
	SetMindState(MS_NORMAL);
	MyBrethren.clear();
	MyLeader = NULL;
	mAlive=true;
	Entity::Reset();
	SetState(ES_IDLE);
	Radar.OldX = 0.0;
	Radar.OldY = 0.0;
	mAge = 0;
	mHunger=100;
	TimerHunger=0;
	TimerHunger2=0;

	Maturity = 1;
	Death = 100;
	mWanderStep = 50;
	mPauseTime = 1000;
	mMoveSpeed = 1;
	mTargetType = ET_GRASS;
	LeaderAlive = false;

	if (AmLeader)
	{
		SetGender(true);
	}
	else
	{
		if (ID%2==0)
		{
			SetGender(false);
		}
		else
		{
			SetGender(true);
		}
	}
	setsterile(false);
}
void Prey::setsterile(bool thevalue)
{
	mSterile=thevalue;
}
bool Prey::Replenish()
{
	thetarget->SetOccupied(true);
	theHunger+=mApp->mTimeDelta;
	if (theHunger>=500)
	{
		thetarget->SetEating(true);
		PlayAnimation(ANIMATION_EATING,true);
		theHunger=0;
		mHunger+=2;
	}
	
	if (mHunger>=HungerLimit)
	{
		mHunger=HungerLimit;
		return true;
	}
	HunterCheck();
	return false;
}
void Prey::ResetTarget()
{
	NumTargets=mNumPrey=OLDLIST;
	mPreyList = OldPrey;
	mTargetType = OldType;
}
void Prey::SaveTarget()
{
	OLDLIST = mNumPrey;
	OldPrey = mPreyList;
	OldType = mTargetType;
}

void Prey::FSM()
{					

	if (mAlive)
	{
		if (FSTATE ==FS_SLEEPING)
			SetState(ES_IDLE);
		if ((mCurState!=ES_EVADING)&&(mCurState!=ES_MATING)&&(mCurState!=ES_ATTACK))
		{
			if (FSTATE != FS_SLEEPING)
				if (LSTATE != LS_PATROL)
					if ((MyLeader==NULL)&&(AmLeader == false))
						HunterCheck();
		}
		if (mAge>Death)
		{
			mAlive=false;
		}
		if (mCurState!=ES_ATTACK)
		{
			if (FSTATE != FS_SLEEPING)
				if (LSTATE != LS_PATROL)
					HungerBar();
		}

		//if you are leader, check for followers
		if ((mCurState == ES_IDLE)||(mCurState==ES_HUNTING)||(mCurState==ES_STALKING))
		{
			if (AmLeader)
			{
				if (MyBrethren.size()<5)
					GetFollowers();
			}
		}
		switch(mCurState)
		{
		case ES_IDLE:
			Idle();
			break;
		case ES_HUNTING:
			Hunting();
			break;
		case ES_STALKING:
			Stalking();
			break;
		case ES_MATING:
			Mating();
			break;
		case ES_ATTACK:
			Attack();
			break;
		case ES_EVADING:
			Evading();
		default:
			break;
		}
	}
}
void Prey::HungerBar()
{
	TimerHunger2+=mApp->mTimeDelta;
	if (TimerHunger2>=1000)
	{
		mHunger--;
		TimerHunger2=0;
		//cout<<"Hunger: "<<mHunger<<endl;
	}
	if (mHunger<=HungerDeath)
	{
		mAlive=false;
	}
}
bool Prey::HungerCheck()
{
	if (mHunger>HungerThreshold)
	{
		return false;
	}
	else if (mHunger<HungerThreshold)
	{
		return true;
	}
	return false;
}
void Prey::IdleMode()
{
	mWanderStep = 50;
	mPauseTime = 1000;
	mMoveSpeed = 1;
	mSearchDistance = 200;
	mSearchAngle = 60;
	mTargetType = ET_GRASS;
}
void Prey::ChaseMode()
{
	mMoveSpeed = 2;							
	SetAnimationSpeed(100);						
	PlayAnimation(ANIMATION_RUNNING, true);		
}
void Prey::PatrolMode()
{
	mCurTarget = 0;
	mMoveSpeed = 1;
	mWanderStep = 50;
	mCurrentStep = 0;
	SetAnimationSpeed(200);
	PlayAnimation(ANIMATION_WALKING);
}
void Prey::StalkMode()
{
	mCurTarget = 0;
	mMoveSpeed = 1;
	mCurrentStep = 0;
	SetAnimationSpeed(200);
}
bool Prey::IsMature()
{
	if (mAge>=Maturity)
		return true;
	else
		return false;
}
bool Prey::GetSterile()
{
	return mSterile;
}
bool Prey::SearchConditionCheck(Entity * curTarget)
{
	return true;
	//if (mTargetType==ET_PREY)
	//{
	//	if (curTarget!=0)
	//	{
	//		return true;
	//	}
	//	else
	//		return false;
	//}
	//else if ((mTargetType == ET_PREDATOR)||(mTargetType == ET_CHUCK))
	//	return true;
	//else
	//	return false;
	
}
void Prey::Render()
{
	if(mAlive)
		Sprite::Render();
}
void Prey::Update()
{
	Entity::Update();
}
