#include "Entity.h"
#include "BaseApp.h"
#include <../include/math.h>
#include <string>
#include <iostream>
#include "Macs.h"
#include "Math.h"
#include "Image.h"

using namespace std;

Entity::Entity(SimianBase * theApp, char * thePath, char * theName) : Sprite(theApp, ((BaseApp*)theApp)->CreateImage(thePath, theName))
{
	Init();
}

Entity::Entity(SimianBase * theApp, char * theName) : Sprite(theApp, ((BaseApp*)theApp)->GetImageDatabase()[string(theName)])
{
	Init();
}

Entity::Entity(SimianBase * theApp, Image * theImage) : Sprite(theApp, theImage)
{
	Init();
}

void Entity::Init()
{
	mCurTarget = 0;
	mCurDirection = ED_DOWN;
	mTotalAnimationData = 0;
	mCurState = ES_IDLE;
	mSearchType = ET_NONE;
	mWanderStep = 0;
	mWanderAngle = 0;
	mMoveSpeed = 0;
	mPauseTime = 0;
	mCurPauseTime = 0;
	mCurrentStep = 0;
	mTargetType = ET_NONE;
	mNumPrey = ENTITY_LISTSIZE;
	mPreyList = 0;
	mPredatorList = 0;
	mAlive = true;
	mAgeTimer = 5000;
	mAge = 0;
	mHunger = 100;
	mGender = GENDER_MALE;
	mSterile = false;
	mSelected = false;
	if (((BaseApp*)mApp)->mGameState)
	{
		mSelectionRing = ((BaseApp*)mApp)->GetImageDatabase()["SelectionRing"];
		mSelectionRing->SetSize(32, 32);
	}
}

Entity::~Entity()
{

}

void Entity::Select()
{
	mSelected = true;
}

void Entity::Deselect()
{
	mSelected = false;
}

void Entity::SetType(Entity_Type theType)
{
	mType = theType;
}

void Entity::SetState(Enitity_State theState)
{
	mCurState = theState;
}

void Entity::SetTarget(Entity * theTarget)
{
	mCurTarget = theTarget;
}

Entity_Type Entity::GetType()
{
	return mType;
}

Enitity_State Entity::GetState()
{
	return mCurState;
}

Entity * Entity::GetTarget()
{
	return mCurTarget;
}

void Entity::SetCurrentDirection(Entity_Direction theDirection)
{
	mCurDirection = theDirection;
}
int Entity::AddAnimation(int ** theSequence, int * Length)
{
	DirectionAnimation a;
	for (int i = 0; i < 4; i ++)
		a.AID[i] = Sprite::AddAnimation(theSequence[i], Length[i]);

	mAnimationData.resize(mTotalAnimationData + 1);
	mAnimationData[mTotalAnimationData] = a;
	mTotalAnimationData ++;
	return mTotalAnimationData - 1;
}

int Entity::AddAnimation(int * UPAnim, int * DOWNAnim, int * LEFTAnim, int * RIGHTAnim, int * Length)
{
	DirectionAnimation a;
	a.AID[0] = Sprite::AddAnimation(UPAnim, Length[0]);
	a.AID[1] = Sprite::AddAnimation(DOWNAnim, Length[1]);
	a.AID[2] = Sprite::AddAnimation(LEFTAnim, Length[2]);
	a.AID[3] = Sprite::AddAnimation(RIGHTAnim, Length[3]);

	mAnimationData.resize(mTotalAnimationData + 1);
	mAnimationData[mTotalAnimationData] = a;
	mTotalAnimationData ++;
	return mTotalAnimationData - 1;
}

void Entity::PlayAnimation(int theSequenceId, bool Loop)
{
	Sprite::PlayAnimation(mAnimationData[theSequenceId].AID[(int)mCurDirection], Loop);
	mCurrentSeq = theSequenceId;
}

void Entity::Update()
{
	Sprite::Update();

	if (!mAlive)
		return;
	mAgeTimer -= mApp->mTimeDelta;
	if (mAgeTimer < 0)
	{
		mAgeTimer = 5000;
		mAge ++;
	}

	FSM();
}

void Entity::Wander()
{
	if (mCurrentStep <= 0)
	{
		PlayAnimation(ANIMATION_STANDING);
		Scan();
	}
	else
	{
		// increment X and Y vals according to angle and speed.
		mCurrentStep -= mMoveSpeed;
		mX += mXInc;
		mY += mYInc;
	}
}

void Entity::Scan()
{
	if (mSearchType == ET_NONE)
	{
		mCurPauseTime -= mApp->mTimeDelta;
		if (mCurPauseTime < 0)
		{
			//reset values for walking..
			mCurrentStep = (float)mWanderStep;
			//reset value for angle
			mWanderAngle = float(rand()%360);

			mXInc = (float)mMoveSpeed * sinf(mWanderAngle/180.0f * 3.142f);
			mYInc = (float)mMoveSpeed * cosf((180 - mWanderAngle)/180.0f * 3.142f);

			if (mWanderAngle >= 315 || mWanderAngle < 45)
			{
				SetCurrentDirection(ED_UP);
			}
			else if (mWanderAngle >= 45 && mWanderAngle < 135)
				SetCurrentDirection(ED_RIGHT);
			else if (mWanderAngle >= 135 && mWanderAngle < 225)
			{
				SetCurrentDirection(ED_DOWN);
			}
			else if (mWanderAngle >= 225 && mWanderAngle < 315)
				SetCurrentDirection(ED_LEFT);
			//cout << mWanderAngle << endl;
			SetAnimationSpeed(300);
			PlayAnimation(ANIMATION_WALKING);
			mCurPauseTime = rand()%mPauseTime;
		}
	}
	else
	{
		mCurPauseTime = 1;
	}
}

void Entity::SetPredator(Entity ** Predator)
{
	mPredatorList = Predator;
}

void Entity::SetPrey(Entity ** Prey)
{
	mPreyList = Prey;
}

void Entity::SetPrey(Entity ** Prey, int NumPrey)
{
	mPreyList = Prey;
	mNumPrey = NumPrey;
}

void Entity::Render()
{
	if (mAlive)
	{
		if (mSelected)
		{
			mSelectionRing->SetPosition(mX, mY + 8);
			mSelectionRing->Render();
		}

		Sprite::Render();
	}
}

void Entity::Render(float x, float y)
{
	if (mAlive)
	{
		if (mSelected)
		{
			mSelectionRing->SetPosition(x, y + 8);
			mSelectionRing->Render();
		}
		Sprite::Render(x, y);
	}
}

void Entity::Search()
{
	// Searches for a creature..
	Entity * NearestPrey = 0;
	float curDistance = 0;

	if (mTargetType != ET_NONE)
	{
		//search in a cone in front of creature..
		if (mPreyList)
		{
			for (int i = 0; i < mNumPrey; i ++)
			{
				if (mPreyList[i]->GetAlive() && SearchConditionCheck(mPreyList[i]))
				{
					// check whether the prey is in sight.
					float meX, meY;
					meX = mX + (float)mWidth/2.0f;
					meY = mY + (float)mHeight/2.0f;
					Vector2D Dist(mPreyList[i]->GetX() + mPreyList[i]->GetWidth()/2 - meX, mPreyList[i]->GetY() + mPreyList[i]->GetHeight()/2 - meY);
					float Angle = Dist.Angle();
					
					float pX = mPreyList[i]->GetX() + mPreyList[i]->GetWidth()/2;
					float pY = mPreyList[i]->GetY() + mPreyList[i]->GetHeight()/2;

					if (pY > meY && pX > meX)
						Angle = 180 - Angle;
					else if (pY > meY && pX < meX)
						Angle = 180 - Angle;
					else if (pY < meY && pX < meX)
						Angle += 360;

					float SearchAngle = mWanderAngle - mSearchAngle/2;
					float SearchAngle2 = mWanderAngle + mSearchAngle/2;
					bool Found = false;

					if (SearchAngle < 0)
						SearchAngle += 360;
					if (SearchAngle2 > 360)
						SearchAngle2 -= 360;

					if (SearchAngle + mSearchAngle > 360)
					{
						if (Angle > SearchAngle || Angle < SearchAngle2)
							Found = true;
					}
					else 
						if (Angle > SearchAngle && Angle < SearchAngle2)
							Found = true;

					if (Found)
					{
						float distance = Dist.Magnitude();
						if (distance < mSearchDistance)
						{
							if (NearestPrey)
							{
								if (distance < curDistance)
								{
									curDistance = distance;
									mCurTarget = mPreyList[i];
									NearestPrey = mCurTarget;
								}
							}
							else
							{
								mCurTarget = mPreyList[i];
								NearestPrey = mCurTarget;
							}
						}
					}
					else
					{

					}
				}
			}
		}
	}
}

int Entity::Chase()
{
	//Chases its current target
	if (mCurTarget && mCurTarget->mAlive)
	{
		float meX, meY;
		meX = mX + (float)mWidth/2.0f;
		meY = mY + (float)mHeight/2.0f;
		Vector2D Dist(mCurTarget->GetX() + mCurTarget->GetWidth()/2 - meX, mCurTarget->GetY() + mCurTarget->GetHeight()/2 - meY);
		float Angle = Dist.Angle();

		float pX = mCurTarget->GetX() + mCurTarget->GetWidth()/2;
		float pY = mCurTarget->GetY() + mCurTarget->GetHeight()/2;

		if (pY > meY && pX > meX)
		{
			Angle = 180 - Angle;
		}
		else if (pY > meY && pX < meX)
		{
			Angle = 180 - Angle;
		}
		else if (pY < meY && pX < meX)
		{
			Angle += 360;
		}

		mXInc = (float)mMoveSpeed * sinf(Angle/180.0f * 3.142f);
		mYInc = (float)mMoveSpeed * cosf((180 - Angle)/180.0f * 3.142f);

		if (Angle >= 315 || Angle < 45)
		{
			if (GetCurrentDirection() != ED_UP)
			{
				SetCurrentDirection(ED_UP);
				PlayAnimation(mCurrentSeq, true);
			}
		}
		else if (Angle >= 45 && Angle < 135)
		{
			if (GetCurrentDirection() != ED_RIGHT)
			{
				SetCurrentDirection(ED_RIGHT);
				PlayAnimation(mCurrentSeq, true);
			}
		}
		else if (Angle >= 135 && Angle < 225)
		{
			if (GetCurrentDirection() != ED_DOWN)
			{
				SetCurrentDirection(ED_DOWN);
				PlayAnimation(mCurrentSeq, true);
			}
		}
		else if (Angle >= 225 && Angle < 315)
		{
			if (GetCurrentDirection() != ED_LEFT)
			{
				SetCurrentDirection(ED_LEFT);
				PlayAnimation(mCurrentSeq, true);
			}
		}

		mX += mXInc;
		mY += mYInc;
		if (Dist.Magnitude() < mWidth)
		{
			//Caught..
			Caught();
			return 2;
		}
		else
			return 1;
	}

	mCurTarget = 0;

	return 0;
}

void Entity::SetAlive(bool Alive)
{
	mAlive = Alive;
}

bool Entity::GetAlive()
{
	return mAlive;
}

Entity_Direction Entity::GetCurrentDirection()
{
	return mCurDirection;
}

int Entity::GetAge()
{
	return mAge;
}

int Entity::GetHunger()
{
	return mHunger;
}

bool Entity::GetGender()
{
	return mGender;
}

void Entity::Reset()
{
	Init();
}

void Entity::SetGender(bool Gender)
{
	mGender = Gender;
}