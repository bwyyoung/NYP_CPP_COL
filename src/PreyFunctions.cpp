#include "Prey.h"
#include "Chuck Norris.h"
#include "Predator.h"
#include "BaseApp.h"
#include "Entity.h"
#include "GameState.h"
#include "Grass.h"
#include <iostream>
#include <stdlib.h>
#include <../include/math.h>

#include "Macs.h"
#include "MessageBoard.h"

void Prey::Caught()
{
	if (mTargetType==ET_GRASS)
	{
		Grass *thegrass;
		thegrass = (Grass*)mCurTarget;
		((Grass*)thegrass)->SetEating(true);
		if (mCurTarget->GetAlive()==false)
		{
			if (HungerCheck())
			{
				FoodTime();
				SetState(ES_HUNTING);
			}
			else
			{
				theHunger=0;
				PatrolMode();
				SetState(ES_IDLE);
				thetarget->SetEating(false);
				thetarget->SetOccupied(false);
			}
		}
	}
	else if (mTargetType==ET_PREY)
	{
		if (mCurTarget->GetGender()==false)
		{
			((Prey*)mCurTarget)->setsterile(true);
		}
		if (GetGender()==false)
		{
			setsterile(true);
		}
		int numofprey= rand()%2+2;
		for (int j=1; j<numofprey; j++)
		{
			for(int i = 0; i< ENTITY_LISTSIZE; i++)
			{
			
				if(((BaseApp*)mApp)->mGameState->mPrey[i]->mAlive==false)
				{
					((BaseApp*)mApp)->mGameState->mPrey[i]->Reset();
					((BaseApp*)mApp)->mGameState->mPrey[i]->SetPosition(mX,mY);
					break;
				}
		}
		}
		PatrolMode();
	}
	mCurTarget = 0;
	mCurrentStep = 0;
}
void Prey::Idle()
{
	if (LSTATE!= LS_PATROL)
	{
		if (FSTATE!= FS_SLEEPING)
		{
			if (HungerCheck())
			{
				FoodTime();
				SetState(ES_HUNTING);
			}
			else
			{

				if (IsMature()&&!mSterile)
				{
					SetPrey((Entity**)((BaseApp*)mApp)->mGameState->mPrey);
					mTargetType=ET_PREY;
					SetState(ES_STALKING);
				}
				else
					SUBFSM();
			}
		}
		else
			SUBFSM();
	}
	else
		SUBFSM();
}

void Prey::BoundCheck()
{
	float x,y;
	float TurnAngle = atan(Radar.VectorCoordinates[0]/Radar.VectorCoordinates[1]);
	if (mCurState == ES_EVADING)
	{
		if ((mX-TheBound>0)||(mX+TheBound>mApp->GetWindowWidth()))
		{
					if (mY<Radar.OldY)
					{
						TurnAngle= -TurnAngle;
					}
					else if (mY==Radar.OldY)
					{
						TurnAngle=90;
					}
					float blah = (float)RTOD(2.0f*(float)TurnAngle);
					x= (float)Radar.VectorCoordinates[0]*cosf(blah) + (float)Radar.VectorCoordinates[1]*sinf(blah);
					y= -(float)Radar.VectorCoordinates[0]*sinf(blah) +  (float)Radar.VectorCoordinates[1]*cosf(blah);
		}
		else if ((mY-TheBound>0)||(mY+TheBound>mApp->GetWindowHeight()))
		{
				TurnAngle = TurnAngle*2;
				if (mX<Radar.OldX)
				{
					TurnAngle= -TurnAngle-180;
				}
				else if (mX==Radar.OldX)
				{
					TurnAngle=180;
				}
				x= Radar.VectorCoordinates[0]*cosf(RTOD(TurnAngle)) + Radar.VectorCoordinates[1]*sinf(RTOD(TurnAngle));
				y= -Radar.VectorCoordinates[0]*sinf(RTOD(TurnAngle)) +  Radar.VectorCoordinates[1]*cosf(RTOD(TurnAngle));
		}
		Radar.VectorCoordinates[0]=x;
		Radar.VectorCoordinates[1]=y;
		Radar.MyDirection = RTOD(atanf(Radar.VectorCoordinates[1]/Radar.VectorCoordinates[0]))+90;
	}
	BoundaryCheck=true;
}
void Prey::Attack()
{
	if (thetarget != NULL)
	{
		if (thetarget->GetMature() == true)
		{
			if (thetarget->GetAlive()==false)
			{
				thetarget->SetEating(false);
				thetarget->SetOccupied(false);
				if (HungerCheck())
				{
					FoodTime();
					mCurTarget = 0;
					SetState(ES_HUNTING);
				}
				else
				{
					theHunger=0;
					mCurTarget = 0;
					PatrolMode();
					SetState(ES_IDLE);
				}
			}
			else
			{
				int result = Chase();
				if ((result==2)||(result==0))
				{			
					if (Replenish())
					{
						theHunger=0;
						PatrolMode();
						SetState(ES_IDLE);
						thetarget->SetEating(false);
						thetarget->SetOccupied(false);
					}
				}
				else if (result==1)
				{
					mMoveSpeed = 2;							
					SetAnimationSpeed(100);
					thetarget = (Grass*)mCurTarget;
				}	
			}

		}
		else
		{
			PatrolMode();
			SetState(ES_IDLE);
		}
	}
	else
	{
		PatrolMode();
		SetState(ES_IDLE);
	}


		


}
void Prey::Evading()
{
	Run();
	//int chance = rand()%100;
	//if (chance<1)
	//	NewVector();
	if (EscapeEvade())
	{
		PatrolMode();
		PlayAnimation(ANIMATION_WALKING);
		SetState(ES_IDLE);
	}
}
bool Prey::EscapeEvade()
{
	TimeOut+=mApp->mTimeDelta;

	if (TimeOut>2000)
	{  	
		float a = sqrt(( abs(mX-mCurTarget->GetX())) * (abs(mX-mCurTarget->GetX()))+(abs(mY-mCurTarget->GetY()))*(abs(mY-mCurTarget->GetY())));
		if (a>mSearchDistance)
		{
			TimeOut=0;
			return true;
		}
		//else if (TimeOut>=5000)
		//{
		//	return true;
		//}
		else
		{
			return false;
		}
	}
	return false;
}
void Prey::Hunting()
{
	mNumPrey=ENTITY_LISTSIZE;
	SUBFSM();
	Search();
	if (mCurTarget)
	{
		if (mCurTarget->GetType()==ET_GRASS)
		{
			if (mCurTarget->GetAlive()==true)
			{
				if (((Grass*)mCurTarget)->GetEaten()==false)
				{
					if (((Grass*)mCurTarget)->GetMature()==true)
					{
						thetarget=(Grass*)mCurTarget;
						PlayAnimation(ANIMATION_RUNNING,true);
						SetState(ES_ATTACK);
					}
				}
			}
		}
	}
}
void Prey::HunterCheck()
{
	SaveTarget();
	//search for predators

	NumTargets=mNumPrey=ENTITY_LISTSIZE*2;

	mTargetType= ET_CHUCK;
	SetPrey(((BaseApp*)mApp)->mGameState->NewPrey);

	Search();

	//TODO: Set Check for Chuck Norris
	if (mCurTarget)
	{
		if ((mCurTarget->GetType()==ET_PREDATOR||mCurTarget->GetType()==ET_CHUCK) )
		{
			if (mCurTarget->GetState() == ES_HUNTING)
			{
				ChaseMode();
				NewVector();
				MessageBoard::GetBoard()->PostMMessage(Message(this, ET_MOOSE, mCurTarget));
				SetState(ES_EVADING);
			}
			else
			{
				NumTargets=mNumPrey=ENTITY_LISTSIZE;
				SetPrey(OldPrey);
				mTargetType = OldType;
				mCurTarget=0;
			}
		}
		else
		{
			NumTargets=mNumPrey=ENTITY_LISTSIZE;
			SetPrey(OldPrey);
			mTargetType = OldType;
			mCurTarget=0;
		}
	}
	else
	{
		ResetTarget();
		mCurTarget=0;
	}
}
void Prey::NewVector()
{

	Radar.VectorCoordinates[0] = mX-mCurTarget->GetX();
	Radar.VectorCoordinates[1] = mY-mCurTarget->GetY();
	float Length = sqrt(Radar.VectorCoordinates[0]*Radar.VectorCoordinates[0] + Radar.VectorCoordinates[1]*Radar.VectorCoordinates[1] );
	Radar.VectorCoordinates[0] = Radar.VectorCoordinates[0]/Length;
	Radar.VectorCoordinates[1] = Radar.VectorCoordinates[1]/Length;
	Radar.MyDirection = RTOD(atan(Radar.VectorCoordinates[1]/Radar.VectorCoordinates[0]))+90;
}
void Prey::SetFacingDirection()
{
		
		FaceUpdate+=mApp->mTimeDelta;

		if (FaceUpdate>5000)
		{
			PlayAnimation(ANIMATION_RUNNING);
		}
		if (FaceUpdate>10)
		{
			FaceUpdate=0;
			float dir[2];
			float angle=0;
			dir[0]=mX-Radar.OldX;
			dir[1]=mY-Radar.OldY;
	
			if ((dir[0]<0)&&(dir[1]<0))
			{
				angle = RTOD(atan(-dir[1]/-dir[0]))+270;
			}
			else if  ((dir[0]>0)&&(dir[1]>0))
			{
				angle = RTOD(atan(dir[1]/dir[0]))+90;
			}
			else if  ((dir[0]<0)&&(dir[1]>0))
			{
				angle = 270 - RTOD(atan(dir[1]/-dir[0]));
			}
			else if ((dir[0]>0)&&(dir[1]<0))
			{
				angle = 90 - RTOD(atan(-dir[1]/dir[0]));
			}
			else if (dir[0]==0)
			{
				if (dir[1]>0)
				{
					angle = 0;
				}
				else if (dir[1]<0)
				{
					angle = 180;
				}
			}
			else if (dir[1]==0)
			{
				if (dir[0]>0)
				{
					angle = 90;
				}
				else if (dir[0]<0)
				{
					angle = 270;
				}
			}

			if (angle >= 315 || angle < 45 )
			{
				mWanderAngle = float(rand()%360)+315;
				if (mWanderAngle>360)
				{
					mWanderAngle-=360;
				}
				if (GetCurrentDirection()!=ED_UP)
				{
					SetCurrentDirection(ED_UP);
					PlayAnimation(mCurrentSeq, true);
				}

			}
			else if (angle >= 45 && angle < 135)
			{
				mWanderAngle = float(rand()%135)+45;
				if (GetCurrentDirection()!=ED_RIGHT)
				{
					SetCurrentDirection(ED_RIGHT);
					PlayAnimation(mCurrentSeq, true);
				}
			}
			else if (angle >= 135 && angle < 225)
			{
				mWanderAngle = float(rand()%225)+135;
				if (GetCurrentDirection()!=ED_DOWN)
				{
					SetCurrentDirection(ED_DOWN);
					PlayAnimation(mCurrentSeq, true);
				}
			}
			else if (angle >= 225 && angle < 315)
			{
				mWanderAngle = float(rand()%315)+225;
				if (GetCurrentDirection()!=ED_LEFT)
				{
					SetCurrentDirection(ED_LEFT);
					PlayAnimation(mCurrentSeq, true);
				}
			}
			Radar.OldX=mX;
			Radar.OldY=mY;
		}
}
void Prey::Run()
{
	SetFacingDirection();
	Radar.Angle+=10;
	if (Radar.Angle>=360)
	{
		Radar.Angle = 0;
	}
	float y = sinf(DTOR(Radar.Angle));
	float thebearing=Radar.MyDirection-90;
	Radar.LocalToGlobal[0]=-sinf(DTOR(thebearing))*y;
	Radar.LocalToGlobal[1]=cosf(DTOR(thebearing))*y;
	if(((Radar.Angle>90)&&(Radar.Angle<180))||((Radar.Angle>270)&&(Radar.Angle<360)))
	{
		Radar.LocalToGlobal[0]=-Radar.LocalToGlobal[0];
		Radar.LocalToGlobal[1]=-Radar.LocalToGlobal[1];
		if ((Radar.LocalToGlobal[0]>0)||(Radar.LocalToGlobal[1]>0))
		{
			if (Radar.LocalToGlobal[0]<Radar.MinSpeed)
				Radar.LocalToGlobal[0]=Radar.LocalToGlobal[0]*Radar.MinSpeed;
			if (Radar.LocalToGlobal[1]<Radar.MinSpeed)
				Radar.LocalToGlobal[1]=Radar.LocalToGlobal[1]*Radar.MinSpeed;
		}
		else if ((Radar.LocalToGlobal[0]<0)||(Radar.LocalToGlobal[1]<0))
		{
			if (Radar.LocalToGlobal[0]>(-Radar.MinSpeed))
				Radar.LocalToGlobal[0]=Radar.LocalToGlobal[0]*Radar.MinSpeed;
			if (Radar.LocalToGlobal[1]>(-Radar.MinSpeed))
				Radar.LocalToGlobal[1]=Radar.LocalToGlobal[1]*Radar.MinSpeed;
		}		
	}
	mX+=Radar.Ratio*Radar.DashSpeed*Radar.VectorCoordinates[0] + Radar.LocalToGlobal[0]*Radar.Amplitude;
	mY+=Radar.Ratio*Radar.DashSpeed*Radar.VectorCoordinates[1] + Radar.LocalToGlobal[1]*Radar.Amplitude;
}
void Prey::Mating()
{
	if (((Prey*)mCurTarget)->GetSterile()==false)
	{
		int result  = Chase();
		if (result ==2)
		{
			
			mCurTarget = 0;
			mMoveSpeed = 0.5;
			mWanderStep = 50;
			SetPrey(0);
			mCurrentStep = 0;
			mHunger = 100;
			mTargetType = ET_NONE;
			SetAnimationSpeed(200);

			PatrolMode();
			SetState(ES_IDLE);
		}
		else if (result ==0)
		{
			SetState(ES_STALKING);
		}	
	}
	else
	{
		PatrolMode();
		SetState(ES_IDLE);
	}
}
void Prey::SexyTime()
{
	NumTargets=mNumPrey=ENTITY_LISTSIZE;
	SetPrey((Entity**)((BaseApp*)mApp)->mGameState->mPrey);
	mTargetType=ET_PREY;
}
void Prey::FoodTime()
{
	SetPrey((Entity**)((BaseApp*)mApp)->mGameState->mGrass);
	//mCurTarget->SetType(ET_GRASS);
	mTargetType=ET_GRASS;

}
void Prey::Stalking()
{

	if (HungerCheck())
	{
		FoodTime();
		SetState(ES_HUNTING);
	}
	else
	{
		SUBFSM();
		SexyTime();
		Search();
		if(mCurTarget)
				if (((Prey*)mCurTarget)->IsMature())
					if (((Prey*)mCurTarget)->GetGender()!=mGender)
						if(((Prey*)mCurTarget)->GetSterile()==false)
						{
							ChaseMode();
							SetState(ES_MATING);
						}
	}
}
