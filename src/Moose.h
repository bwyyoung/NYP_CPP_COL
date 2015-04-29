#pragma once
#include "Entity.h"

class Moose : public Entity
{
public:
	Moose(SimianBase * theApp, char * theName);
	~Moose();

	virtual void Update();
	void Render();
	virtual void FSM();
	void Idle();
	void Hunting();
	void Attack();
	void Stalking();
	void Mating();
	void Charging();

	virtual void Caught();
	virtual bool SearchConditionCheck(Entity * curTarget);
	virtual void Reset();

	int ANIMATION_CHARGING;
protected:
	int mHungerTimer;
};