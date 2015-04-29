#pragma once
#include "Macros.h"

class SIM_EXP Timer
{
private:
	bool			bIsRenderUpdate;

	unsigned int	mFPS;
	unsigned int	mFrameCount;
	unsigned int	mSecondsPassed;
	unsigned long	mDeltaTime;
	unsigned long	mLastFrameTime;

	unsigned int	mRenderFPS;
	unsigned int	mRenderFrameCount;

public:
	Timer();
	~Timer();
	void			Update();			// Timer Update for Update loop
	void			RenderUpdate();		// Timer Update for Render loop


	/* Get Functions */
	bool			GetIsRenderUpdate();	// Get bIsRenderUpdate

	unsigned long	GetAppTime();			// Get time passed since the application has started in milliseconds.
	unsigned int	GetAppTimeSeconds();	// Get time passed since the application has started in seconds. 
	unsigned long	GetDeltaTime();			// Get time passed since last frame.
	unsigned int	GetFPS();				// Get Frames Per Second
	unsigned long	GetMSPF();				// Get Milliseconds Per Frame

	unsigned int	GetRenderFPS();		// Get Frames Per Second for Render loop
	unsigned long	GetRenderMSPF();	// Get Milliseconds Per Frame for Render loop



	/* Set Functions */
	void			SetIsRenderUpdate(bool IsRenderUpdate);	// Set bIsRenderUpdate
};