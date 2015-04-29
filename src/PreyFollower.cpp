#include "Prey.h"
#include "MSmath.h"
#include "Environment.h"
#include "BaseApp.h"
#include "GameState.h"

void Prey::SetLeader(Prey *Leader)
{
	MyLeader = Leader;
	LeaderAlive = true;
}
void Prey::SetFollowerState(FollowerState TheState)
{
	FSTATE = TheState;
}
Prey * Prey::GetLeader()
{
	return MyLeader;
}
void Prey::FollowLeader()
{
		if (LeaderInRange())
		{
			Wander();
		}
		else
		{
		//	cout<<"Leader is out of range"<<endl;
			SaveTarget();
			ChaseMode();
			SetFollowerState(FS_CHASELEADER);
		}
}
void Prey::ChaseLeader()
{
	try
	{
		mCurTarget = MyLeader;
		if (GetLeaderDistance()>(LeaderRadius - LeaderBuffer))
		{
			//cout<<"Chasing Leader"<<endl;
			Chase();
		}
		else
		{
			//cout<<"Within Normal range"<<endl;
			FSTATE  = FS_NORMAL;
			PatrolMode();
			ResetTarget();
		}
	}
	catch (...)
	{
			MessageBox(NULL, "ChaseLeader", "something happened", MB_OK);
	}
}
float Prey::GetLeaderDistance()
{
	float XDistance = MyLeader->GetX() - GetX();
	float YDistance = MyLeader->GetY() - GetY();
	float Magnitude = sqrt(XDistance*XDistance + YDistance*YDistance);
	return Magnitude;
}
bool Prey::LeaderInRange()
{
	if (GetLeaderDistance()>LeaderRadius)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool Prey::CheckNightTime()
{
	if (((BaseApp*)mApp)->mGameState->mEnvironment->GetEnvState() == esNight)
		return true;
	else
		return false;
}
void Prey::CheckLeaderAlive()
{
	
	if (MyLeader->GetAlive() == false)
	{
		MSTATE = MS_NORMAL;
		LeaderAlive = false;
		MyLeader = NULL;
	}
}
void Prey::SetMindState(MindState TheState)
{
	MSTATE = TheState;
}
void Prey::Sleep()
{
		PlayAnimation(ANIMATION_SLEEPING,true);

		if(CheckNightTime() == false)
		{
			SetFollowerState(FS_NORMAL);
			SetAnimationSpeed(200);
			PlayAnimation(ANIMATION_WALKING);
			SetState(ES_IDLE);
		}
};
void  Prey::SUBFSM()
{
	if (FSTATE == FS_SLEEPING)
	{
		Sleep();
	}
	else
	{
		if (MSTATE ==MS_ALERTED)
		{
			if (CheckNightTime()==false)
				MSTATE = MS_NORMAL;
		}
		if (AmLeader)
		{
			if ((mCurState!=ES_EVADING)&&(mCurState!=ES_MATING)&&(mCurState!=ES_ATTACK))
			{
				LeaderHunterCheck();
			}
			if (MSTATE == MS_NORMAL)
			{

				if(CheckNightTime())
				{
					SetLeaderState(LS_PATROL);
					SetAnimationSpeed(200);
					PlayAnimation(ANIMATION_STANDING);
					SetState(ES_IDLE);
				}
			}
			switch(LSTATE)
			{
			case LS_NORMAL:
				Wander();
				break;
			case LS_PATROL:
				Patrol();
				break;
			default:
				break;
			}
		}
		else
		{
			if (MSTATE == MS_NORMAL)
			{
				if(CheckNightTime())
				{

					SetFollowerState(FS_SLEEPING);
					SetAnimationSpeed(200);
					SetState(ES_IDLE);
				}
			}
			if (MyLeader)
				CheckLeaderAlive();
			if (LeaderAlive)
			{
				switch(FSTATE)
				{
				case FS_NORMAL:
					FollowLeader();
					break;
				case FS_CHASELEADER:
					ChaseLeader();
					break;
				default:
					break;
				}
			}
			else
			{
				Wander();
			}
		}
	}
	
}