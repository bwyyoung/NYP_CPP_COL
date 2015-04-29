#pragma once
#include "Entity.h"

class BaseApp;
class Image;

class PlaceHolder : public Entity
{
public:
	PlaceHolder(BaseApp * theApp, Image * theImage);
	~PlaceHolder();

	void SetAnimation(int AnimationID, Entity_Type theType);

	virtual void Update();
	virtual void Render();
	virtual void FSM();

	virtual void Caught();
	virtual bool SearchConditionCheck(Entity * curTarget);
	virtual void Reset();
protected:
};