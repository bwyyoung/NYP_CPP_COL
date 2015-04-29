#pragma once
#include "Sprite.h"
#include <vector>

#define GENDER_MALE 1
#define GENDER_FEMALE 0

using namespace std;

enum Enitity_State
{
	ES_IDLE, ES_HUNTING, ES_ATTACK, ES_STALKING, ES_MATING, ES_EVADING, ES_REPRODUCTION, ES_EATEN, ES_JUMPKICK, ES_CHARGING
};

enum Entity_Type
{
	ET_PREDATOR, ET_PREY, ET_CHUCK, ET_GRASS, ET_NONE, ET_MOOSE
};

enum Entity_Direction
{
	ED_UP = 0, ED_DOWN, ED_LEFT, ED_RIGHT
};

struct DirectionAnimation
{
	int AID[4];
};

class SimianBase;

class Entity : public Sprite
{
public:
	Entity(SimianBase * theApp, char * thePath, char *theName);
	Entity(SimianBase * theApp, char * theName);
	Entity(SimianBase * theApp, Image * theImage);
	~Entity();

	int ANIMATION_RUNNING;
	int ANIMATION_WALKING;
	int ANIMATION_STANDING;
	int ANIMATION_EATING;
	int ANIMATION_SLEEPING;



	void SetType(Entity_Type theType);
	void SetState(Enitity_State theState);
	void SetTarget(Entity * theTarget);

	Entity_Type GetType();
	Enitity_State GetState();
	Entity * GetTarget();

	virtual void Update();
	
	Entity_Direction GetCurrentDirection();
	void SetCurrentDirection(Entity_Direction theDirection);
	int AddAnimation(int ** theSequence, int * Length);
	int AddAnimation(int * UPAnim, int * DOWNAnim, int * LEFTAnim, int * RIGHTAnim, int * Length);
	void PlayAnimation(int theSequenceId, bool Loop = true);

	virtual void FSM() = 0;

	virtual void Reset();

	void Wander();
	void Scan();
	void Search();

	virtual void Render();
	virtual void Render(float x, float y);
	int Chase();

	virtual void Caught() = 0;
	virtual bool SearchConditionCheck(Entity * curTarget) = 0;

	bool GetAlive();
	void SetAlive(bool a);

	void SetPrey(Entity **);
	void SetPrey(Entity **, int);
	void SetPredator(Entity **);

	void SetGender(bool Gender);

	int GetAge();
	int GetHunger();
	bool GetGender();

	void Select();
	void Deselect();
protected:
	int mWanderStep;
	float mCurrentStep;
	float mWanderAngle;
	float mMoveSpeed;
	bool mSterile;
	Entity_Type mSearchType;
	int mPauseTime;
	int mCurPauseTime;
	float mXInc;
	float mYInc;

	bool mAlive;
	bool mGender;
	

	int mNumPrey;
	Entity ** mPreyList;
	Entity ** mPredatorList;

	void Init();

	int mAge;
	int mAgeTimer;
	int mHunger;

	Enitity_State mCurState;
	Entity_Type mType;
	Entity * mCurTarget;
	Entity_Type mTargetType;
	int mSearchDistance;
	int mSearchAngle;
	Entity_Direction mCurDirection;
	vector<DirectionAnimation> mAnimationData;
	int mCurrentSeq;
	int mTotalAnimationData;
	bool mSelected;
	Image * mSelectionRing;
};
