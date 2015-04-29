#pragma once
#include "RenderObject.h"

#include "Image.h"

class BaseApp;

struct rainDrop
{
	int x;
	int y;
	bool render;
};

enum EnvironmentState
{
	esClear, esRainLight, esRainMedium, esRainHeavy, esNight , esEveningTime //Note the Evening time state is not to be used by the player at all only the computer can control it.
};

class Entity;

class Environment : RenderObject
{
public:
	Environment(BaseApp * theApp);
	virtual ~Environment();

	void Init();
	virtual void Update();
	virtual void Render();
	virtual void Render(float x, float y);
	bool WaterPlants;
	void SetEnvState(EnvironmentState envState);
	EnvironmentState GetEnvState();
protected:
	BaseApp * mApp;
	Image * mRainDroplet;
	Image * mBlackMask;
	Image * mWhiteMask;
	Image * mLightning;

	rainDrop * rain;
	EnvironmentState mState;

	int mTimePassed;
	int mDayTimer;

	int mSpawnDropTime;

	int mRainSpeed;
	int mDropsPerVolley;
	int mLightningTime;
	bool mLightningDeadly;
	int mCurLightningTime;
	int mLightningAppearTime;
	Entity * mLightningTarget;
	bool mLightningUpdating;
	float mLightningAlpha;

	float curNightAlphaVal;

	void CreateEnvironmentEffect(EnvironmentState envState);
	void SetDarkness(float perc);
	void Lightning(bool KillCreatures, int LightningTime);
	void Lightning();
	void LightningUpdate();
};