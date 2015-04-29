#pragma once
#include "Entity.h"

class SimianBase;
class Image;

class Predator : public Entity
{
public:
	Predator(SimianBase * theApp, char * theName);
	~Predator();
	virtual void Update();
	void Render();
	virtual void FSM();
	void Idle();
	void Hunting();
	void Attack();
	void Stalking();
	void Mating();

	virtual void Caught();
	virtual bool SearchConditionCheck(Entity * curTarget);
	virtual void Reset();
protected:
	int mHungerTimer;
};