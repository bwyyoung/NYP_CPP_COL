#include "Timer.h"
#include <time.h>
#include <windows.h>

Timer::Timer()
{
	bIsRenderUpdate = true;
	mFPS = mFrameCount = mSecondsPassed = mDeltaTime = mLastFrameTime = 0;
	mRenderFPS = mRenderFrameCount = 0;
}
Timer::~Timer()
{
}

void Timer::Update()
{
	mDeltaTime = (unsigned long) GetTickCount() - mLastFrameTime;
	if (GetTickCount() > (mSecondsPassed * 1000))	// If a second has passed
	{
		mFPS = mFrameCount;
		mFrameCount = 0;
		mSecondsPassed++;

		if (bIsRenderUpdate) {
			mRenderFPS = mRenderFrameCount;
			mRenderFrameCount = 0;
		}
	}
	mFrameCount++;
	mLastFrameTime = (unsigned long) GetTickCount();
}

void Timer::RenderUpdate()
{
	if (bIsRenderUpdate)
		mRenderFrameCount++;
}




/* GET FUNCTIONS */
bool Timer::GetIsRenderUpdate()
{
	return bIsRenderUpdate;
}

unsigned long Timer::GetAppTime()
{
	return GetTickCount();
}

unsigned int Timer::GetAppTimeSeconds()
{
	return mSecondsPassed;
}

unsigned long Timer::GetDeltaTime()
{
	return mDeltaTime;
}

unsigned int Timer::GetFPS()
{
	return mFPS;
}

unsigned long Timer::GetMSPF()
{
	return 1000 / mFPS;
}

unsigned int Timer::GetRenderFPS()
{
	return mRenderFPS;
}

unsigned long Timer::GetRenderMSPF()
{
	return 1000 / mRenderFPS;
}




/* SET FUNCTIONS */
void Timer::SetIsRenderUpdate(bool boolean)
{
	bIsRenderUpdate = boolean;
}