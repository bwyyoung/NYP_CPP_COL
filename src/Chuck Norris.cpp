#include "Chuck Norris.h"
#include "Predator.h"
#include "Prey.h"
#include "Sprite.h"
#include "Image.h"
#include "BaseApp.h"
#include "Macs.h"
#include "Math.h"
#include <vector>
#include "GameState.h"

#include <iostream>
using namespace std;

Chuck_Norris::Chuck_Norris(SimianBase *theApp, char *theName) : Entity(theApp, theName)
{
	/* SET UP */
	SetType(ET_CHUCK);
	SetFrames(4, 6);
	mRageSprite = NULL;


	/* ANIMATION */

	// Standing
	int StandingLeft[]		= {5};
	int StandingRight[]		= {6};
	int StandingDown[]		= {12};
	int StandingUp[]		= {18};
	int StandingLengths[]	= {1, 1, 1, 1};

	// Walking
	int WalkingLeft[]		= {5, 4, 3};
	int WalkingRight[]		= {6, 7, 8};
	int WalkingDown[]		= {12, 13, 14};
	int WalkingUp[]			= {18, 19, 20};
	int WalkingLengths[]	= {3, 3, 3, 3};

	// Running
	int RunningLeft[]		= {2, 1};
	int RunningRight[]		= {9, 10};
	int RunningDown[]		= {15, 16};
	int RunningUp[]			= {21, 22};
	int RunningLengths[]	= {2, 2, 2, 2};

	// Jump-Kicking
	int JumpKickingLeft[]		= {0};
	int JumpKickingRight[]		= {11};
	int JumpKickingDown[]		= {17};
	int JumpKickingUp[]			= {23};
	int JumpKickingLengths[]	= {1, 1, 1, 1};

	// Add Animations
	ANIMATION_STANDING = AddAnimation(StandingUp, StandingDown, StandingLeft, StandingRight, StandingLengths);
	ANIMATION_WALKING = AddAnimation(WalkingUp, WalkingDown, WalkingLeft, WalkingRight, WalkingLengths);
	ANIMATION_RUNNING = AddAnimation(RunningUp, RunningDown, RunningLeft, RunningRight, RunningLengths);
	ANIMATION_JUMPKICKING = AddAnimation(JumpKickingUp, JumpKickingDown, JumpKickingLeft, JumpKickingRight, JumpKickingLengths);

	// Start Animation
	SetAnimationSpeed(200);
	PlayAnimation(ANIMATION_STANDING, false);


	/* CHUCK NORRIS' STATS */
	Reset();
}

Chuck_Norris::~Chuck_Norris()
{
	SAFE_DELETE(mRageSprite);
}

void Chuck_Norris::Update()
{
	Entity::Update();
	if (!mAlive) {
		/* A circular wave of death is produced when 2 Chucklets attack each other */
		if (bWaveOfDeath)
		{
			mWaveRadius += mApp->mTimeDelta;	// The wave radius increase with time.
			mWODSprite->Update();				// Update Wave Of Death Sprite.

			if (mWaveRadius > 500) // The wave stops after reaching a radius of 500.
			{
				bWaveOfDeath = false;
				mWaveRadius = 0;
				return;
			}

			// This loop finds the entities which are in the AOE of the wave and kills them.
			for (int i = 0; i < ENTITY_LISTSIZE * 4; i++)
			{
				if (((BaseApp *)mApp)->mGameState->mAllTargets[i]->GetAlive()) // Checks whether entity is alive
				{
					Vector2D V(((BaseApp *)mApp)->mGameState->mAllTargets[i]->GetX() - mX, ((BaseApp *)mApp)->mGameState->mAllTargets[i]->GetY() - mY);
					if (V.Magnitude() < mWaveRadius)
						((BaseApp *)mApp)->mGameState->mAllTargets[i]->SetAlive(false);
				}
			}
		}
		return;
	}

	mHungerTimer -= mApp->mTimeDelta;	// Update Hunger Timer
	if (mHungerTimer < 0)	// If Hunger Timer has passed 1 second
	{
		mHungerTimer = 1000;	// Reset Timer
		mHunger -= 5;			// Decrease Hunger by 5.

		// If Chuck Norris gets too hungry, he becomes Raged
		if (mHunger <= 0)
		{
			mHunger = 0;
			if (!bRage)
			{
				// Set Rage mode
				bRage = true;
				mSpeedFactor = 2;		// Speed x2
				mSearchDistance = 400;	// Increased Sight Range
				mSearchAngle = 360;		// 360 degrees Viewing Cone - completely aware of surroundings.
				mRageSprite->PlayAnimation(0, true);
			}
		}
	}

	if (bRage)
		mRageSprite->Update();
}

void Chuck_Norris::Render()
{
	if (bWaveOfDeath)
		mWODSprite->Render();

	if (!mAlive) return;
	Entity::Render();
	if (bRage)
	{
		mRageSprite->SetPosition(mX, mY - 20);
		mRageSprite->Render();
	}
}

void Chuck_Norris::Render(float x, float y)
{
	if (bWaveOfDeath)
	{
		mWODSprite->Render();
	}

	if (!mAlive) return;
	Entity::Render(x, y);
	if (bRage)
	{
		mRageSprite->SetPosition(x, y - 20);
		mRageSprite->Render();
	}
	if (mCurState == ES_IDLE)
	{
		mHighSprite->SetPosition(x, y - 20);
		mHighSprite->Render();
	}
}

void Chuck_Norris::FSM()
{
	switch (mCurState)
	{
	case -1:			// Initialize
		Init();
		break;
	case ES_IDLE:		// Stoned State
		Stone();
		break;
	case ES_HUNTING:	// Hunting State
		Hunt();
		break;
	case ES_ATTACK:		// Attacking State
		Attack();
		break;
	case ES_JUMPKICK:	// Jump-Kicking State
		JumpKick();
		break;
	}
}

void Chuck_Norris::Init()
{
	SetTargets();
	SetState(ES_HUNTING);

	/* RAGE */
	mRageSprite = new Sprite(mApp, ((BaseApp *)mApp)->GetImageDatabase()["Rage"]);
	int RageAnim[] = {0, 1, 2, 3, 2, 1};
	mRageSprite->SetFrames(1, 4);
	mRageSprite->AddAnimation(RageAnim, 6);
	mRageSprite->SetAnimationSpeed(167);
	mRageSprite->SetSize(32, 32);

	/* Wave Of Death */
	mWODSprite = new Sprite(mApp, ((BaseApp *)mApp)->GetImageDatabase()["WOD"]);
	int WODAnim[] = {0, 1, 2, 3, 4};
	mWODSprite->SetFrames(1, 5);
	mWODSprite->AddAnimation(WODAnim, 5);
	mWODSprite->SetAnimationSpeed(100);
	mWODSprite->SetSize(1024, 1024);

	/* HIGH */
	mHighSprite = new Sprite(mApp, ((BaseApp *)mApp)->GetImageDatabase()["High"]);
	mHighSprite->SetFrames(1, 4);
	mHighSprite->AddAnimation(RageAnim, 6);
	mHighSprite->SetAnimationSpeed(167);
	mHighSprite->SetSize(32, 32);
}

void Chuck_Norris::Stone()
{
	mStoneTimer += mApp->mTimeDelta;	// Update stoning timer.
	mHighSprite->Update();

	if (mStoneTimer > 5000)		// If Chuck has been stoning for 5 secs
	{
		mStoneTimer = 0;

		// Go back to hunting
		SetState(ES_HUNTING);
		mMoveSpeed = 1 * mSpeedFactor;
		SetAnimationSpeed(200);
		PlayAnimation(ANIMATION_WALKING, true);
	}
}

void Chuck_Norris::Hunt()
{
	Wander();
	Search();

	if (mCurTarget) // If target found
	{
		SetState(ES_ATTACK);
		mMoveSpeed = 3 * mSpeedFactor;
		SetAnimationSpeed(200);
		PlayAnimation(ANIMATION_RUNNING, true);
	}
	else			// If no target found
	{
	}
}

void Chuck_Norris::Attack()
{
	int result = Chase();
	if (result == 0)		// If target got away or died somehow
	{
		mCurTarget = 0;

		// Get back to HUNTING
		SetState(ES_HUNTING);
		mMoveSpeed = 1 * mSpeedFactor;
		SetAnimationSpeed(200);
		PlayAnimation(ANIMATION_WALKING, true);
	}
	else if (result == 2)	// If target is caught... whatever happens next is handled in the Caught() function
	{
	}
}

void Chuck_Norris::JumpKick()
{
	mJKickTimer += mApp->mTimeDelta;// Update Jump Kick Timer
	mX += mXInc;	// Update Chuck's X-Position
	mY += mYInc;	// Update Chuck's Y-Position

	if (mJKickTimer > 500)	// If Chuck has been Jump-Kicking for 500ms
	{
		mJKickTimer = 0;

		// Set HUNTING state and animation
		SetState(ES_HUNTING);
		mMoveSpeed = 1 * mSpeedFactor;
		SetAnimationSpeed(200);
		PlayAnimation(ANIMATION_WALKING, true);
	}
}

void Chuck_Norris::Caught()
{
	mHunger += 25;	// +25 Hunger for killing target

	if (mCurTarget->GetType() == ET_GRASS)	// If target is a Grass Entity
	{
		mHunger -= 10; // Grass adds less hunger

		if (!bRage)
		{
			// Set STONE state and animation
			SetState(ES_IDLE);
			mMoveSpeed = 0;
			PlayAnimation(ANIMATION_STANDING, false);
			mHighSprite->PlayAnimation(0, true);
		}
		else 
		{
			// Set direction of the Jump Kick that Chuck Norris is going to perform.
			float factor = 0.1f * mSpeedFactor;
			mXInc = (mCurTarget->GetX() - mX) * factor;
			mYInc = (mCurTarget->GetY() - mY) * factor;

			// Set JUMPKICK state and animation
			SetState(ES_JUMPKICK);
			//mMoveSpeed = 5 * mSpeedFactor;
			PlayAnimation(ANIMATION_JUMPKICKING, false);
		}
	}
	else									// If target is not a Grass Entity
	{
		// If target is a Chuck_Norris and it is Attacking Me
		if (mCurTarget->GetType() == ET_CHUCK && mCurTarget->GetTarget() == this)
		{
			bWaveOfDeath = true;	// Create a Wave Of Death
			mWODSprite->PlayAnimation(0, false);
			mWODSprite->SetPosition(mX - 512, mY - 512);
			SetAlive(false);
		}
		else 
		{
			// Set direction of the Jump Kick that Chuck Norris is going to perform.
			float factor = 0.1f * mSpeedFactor;
			mXInc = (mCurTarget->GetX() - mX) * factor;
			mYInc = (mCurTarget->GetY() - mY) * factor;

			// Set JUMPKICK state and animation
			SetState(ES_JUMPKICK);
			//mMoveSpeed = 5 * mSpeedFactor;
			PlayAnimation(ANIMATION_JUMPKICKING, false);
		}
	}

	if (bRage)
	{
		if (mHunger >= 75)	// When Chuck Norris has killed enough
		{
			bRage = false;	// He becomes unraged
			// Stats return to normal
			mSpeedFactor = 1;
			mSearchDistance = 200;
			mSearchAngle = 60;
		}
	}

	if (mHunger > 100)	// Hunger cannot go above 200
		mHunger = 100;

	
	mCurTarget->SetAlive(false);	// Kill target
	mCurTarget = 0;					// Chuck Norris has no target now
	mCurrentStep = 0;
}

void Chuck_Norris::Reset()
{
	Entity::Reset();
	SetState((Enitity_State)-1);
	mWanderStep = 150;
	mPauseTime = 1000;
	mMoveSpeed = 1;
	mSearchDistance = 200;
	mSearchAngle = 60;
	mTargetType = ET_PREDATOR;

	mAge = 20;
	mStoneTimer = 0;
	mJKickTimer = 0;
	mHungerTimer = 1000;
	mHunger = 0;
	bRage = false;
	mSpeedFactor = 1;
	bWaveOfDeath = false;
	mWaveRadius = 0;
}

bool Chuck_Norris::SearchConditionCheck(Entity *curTarget)
{
	return true;
}



/* GET FUNCTIONS */
bool Chuck_Norris::GetIsWaveOfDeath()
{
	return bWaveOfDeath;
}



/* SET FUNCTIONS */
void Chuck_Norris::SetTargets()
{
	SetPrey(((BaseApp *)mApp)->mGameState->mAllTargets, ENTITY_LISTSIZE * 4);
}

void Chuck_Norris::SetIsWaveOfDeath(bool bTrue)
{
	bWaveOfDeath = bTrue;
}