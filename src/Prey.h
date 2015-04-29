#pragma once
#include "Entity.h"
#include "grass.h"
#include "BaseApp.h"
#include <iostream>
#include <map>
#include <WinAPI.h>
#define DTOR(x) x/180.0f*3.14159265f
#define RTOD(x) x/3.14159265f*180.0f

using namespace std;

static const int _VERTI[2] = {1,0};
static const int _HORIZ[2] = {0,1};

struct FlyBox
{
	float LocalToGlobal[2];
	float VectorCoordinates[2];
	float MyDirection;
	float Amplitude;
	float DashSpeed;
	float Ratio;
	float MinSpeed;
	float Angle;
	float OldX, OldY;
};

class SimianBase;
class BaseApp;
class Image;

enum FollowerState
{
		FS_NORMAL,FS_CHASELEADER,FS_SLEEPING
};
enum LeaderState
{
	LS_NORMAL, LS_PATROL
};
enum MindState
{
	MS_NORMAL,MS_ALERTED
};
class Prey : public Entity
{
private:
	int ID;
	int TheBound;
	FlyBox Radar;
	int FaceUpdate;
	int Maturity;
	int Death;
	int TimeOut;

	int theHunger;
	int TimerHunger;
	int TimerHunger2;
	int HungerLimit;
	int HungerThreshold;
	int HungerDeath;
	bool BoundaryCheck;



	bool AmLeader;
	bool LeaderAlive;
	float LeaderRadius;
	float LeaderBuffer;

	FollowerState FSTATE;
	MindState MSTATE;
	LeaderState LSTATE;

	int NumTargets;
	Grass*thetarget;
	map<int, Prey*> MyBrethren;
	Prey *MyLeader;
	

	Entity **OldPrey;
	Entity_Type OldType;
	int OLDLIST;

public:

	void SUBFSM();
////Functions for Followers//////
	void FollowLeader();
	void Sleep();
	bool CheckNightTime();
	bool LeaderInRange();
	float GetLeaderDistance();
	void CheckLeaderAlive();
	void SetFollowerState(FollowerState TheState);
	void SetMindState (MindState TheState);
	void ChaseLeader();
	Prey* GetLeader();


////Functions for Leaders///////
	void Patrol();
	void FollowerCheckUp();
	void GetFollowers();
	void LeaderHunterCheck();
	void SetLeaderState(LeaderState TheState);
	bool CheckLeader();


	Prey(SimianBase * theApp, char * theName) ;
	~Prey();
	virtual void Caught();
	virtual void Update();
	virtual void FSM();
	virtual void Reset();
	virtual bool SearchConditionCheck(Entity * curTarget);
	
	void BoundCheck();
	void SexyTime();
	void FoodTime();
	void ResetTarget();
	void SaveTarget();

	bool EscapeEvade();
	
	void UpdateView();
	void Render();
	void DirectionCheck();
	void HunterCheck();
	void HungerBar();
	
	void SetFacingDirection();
	void SetID(int TheID);
	int GetID();
	void MakeLeader(bool Lead);
	void SetLeader(Prey *Leader);



	bool IsMature();

	bool HungerCheck();
	bool Replenish();

	void Idle();
	void Run();
	void NewVector();
	void IdleMode();
	void setsterile(bool thevalue);
	void StalkMode();
	void ChaseMode();
	void PatrolMode();
	bool GetSterile();
	void Hunting();
	void Attack();
	void Stalking();
	void Mating();
	void Evading();
};