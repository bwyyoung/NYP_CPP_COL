#include "Prey.h"
#include "Chuck Norris.h"
#include "Predator.h"
#include "BaseApp.h"
#include "Entity.h"
#include "Macs.h"
#include "GameState.h"
#include <stdlib.h>
#include "MessageBoard.h"

void Prey::GetFollowers()
{

	SaveTarget();
	SexyTime();
	Search();
	
	if (mCurTarget)
	{
		if (mCurTarget->GetType() == ET_PREY)
		{
			if (((Prey*)mCurTarget)->CheckLeader()== false)
			{
				try
				{
					if (((Prey*)mCurTarget)->GetLeader() ==  NULL)
					{
						//cout<<"Found Follower!"<<endl;
						((Prey*)mCurTarget)->SetLeader(this);
						MyBrethren[((Prey*)mCurTarget)->GetID()] = ((Prey*)mCurTarget);
						if (CheckNightTime()&&MSTATE==MS_ALERTED)
						{
								((Prey*)mCurTarget)->SetMindState(MS_ALERTED);
								((Prey*)mCurTarget)->SetFollowerState(FS_NORMAL);
						}
					}
				}
				catch (...)
				{
					MessageBox(NULL, "GetFollowers", "error",MB_OK);
				}
			}
		}
	}
	ResetTarget();
}
void Prey::LeaderHunterCheck()
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
		if (CheckNightTime())
		{
			if (mCurTarget->GetType()==ET_PREDATOR||mCurTarget->GetType()==ET_CHUCK)
			{

				ChaseMode();
				NewVector();
				//post a message on the message board.
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
			if (mCurTarget->GetType()==ET_PREDATOR||mCurTarget->GetType()==ET_CHUCK)
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
					ResetTarget();
					mCurTarget=0;
				}
			}
			else
			{
				ResetTarget();
				mCurTarget=0;
			}
		}

	}
	else
	{
		ResetTarget();
		mCurTarget=0;
	}
}
void Prey::FollowerCheckUp()
{
	std::map<int, Prey*>::iterator iter;
	for (iter = MyBrethren.begin(); iter !=MyBrethren.end(); iter++)
	{
		if((*iter).second->GetAlive()==false)
		{
			MyBrethren.erase(iter);
		}
	}
}
void Prey::SetLeaderState(LeaderState TheState)
{
	LSTATE = TheState;
}
void Prey::Patrol()
{
	if(CheckNightTime()== false)
	{
		SetLeaderState(LS_NORMAL);
		SetAnimationSpeed(200);
		PlayAnimation(ANIMATION_WALKING);
		SetState(ES_IDLE);
	}
	else
	{
	
		SaveTarget();
		NumTargets=mNumPrey=ENTITY_LISTSIZE*2;
		mTargetType= ET_PREDATOR;
		SetPrey(((BaseApp*)mApp)->mGameState->NewPrey);
		Scan();
		Search();
		if (mCurTarget)
		{	//cout<<"Checking"<<endl;
			if ((mCurTarget->GetType()==ET_PREDATOR||mCurTarget->GetType()==ET_CHUCK))
			{
				//cout<<"Predator Detected"<<endl;
				std::map<int, Prey*>::iterator iter;
				for (iter = MyBrethren.begin(); iter !=MyBrethren.end(); iter++)
				{
					(*iter).second->SetMindState(MS_ALERTED);
					(*iter).second->SetFollowerState(FS_NORMAL);
					(*iter).second->SetTarget(mCurTarget);
					(*iter).second->SetState(ES_IDLE);
					(*iter).second->ChaseMode();
				}

				ChaseMode();
				NewVector();
				SetState(ES_EVADING);
				SetMindState(MS_ALERTED);
				SetLeaderState(LS_NORMAL);
			}
			else
			{
				ResetTarget();
				mCurTarget=0;
			}
		}
		else
		{
			ResetTarget();
			mCurTarget=0;
		}
	}

}