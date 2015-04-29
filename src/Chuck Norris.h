#pragma once
#include "Entity.h"

//TODO: Rage State.
//TODO: Sound, OpenAL

class SimianBase;
class Sprite;
class Chuck_Norris : public Entity
{
protected:
	int				ANIMATION_JUMPKICKING;

	int				mJKickTimer;
	int				mStoneTimer;
	int				mHungerTimer;

	bool			bRage;
	float			mSpeedFactor;
	bool			bWaveOfDeath;
	int				mWaveRadius;

	Sprite *		mRageSprite;
	Sprite *		mWODSprite;
	Sprite *		mHighSprite;

public:
	Chuck_Norris(SimianBase * theApp, char * theName);
	~Chuck_Norris();

	virtual void	Update();
	void			Render();
	void			Render(float x, float y);

	/* FSM Functions */
	virtual void	FSM();
	void			Init();
	void			Stone();
	void			Hunt();
	void			Attack();
	void			JumpKick();

	/* Triggered Events Functions */
	virtual void	Caught();
	virtual void	Reset();
	virtual bool	SearchConditionCheck(Entity * curTarget);

	/* Get Functions */
	bool			GetIsWaveOfDeath();

	/* Set Functions */
	void			SetTargets();
	void 			SetIsWaveOfDeath(bool bTrue);
};