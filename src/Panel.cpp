#include "Panel.h"
#include "MouseInput.h"
#include "BaseApp.h"

Panel::Panel(BaseApp * theApp)
{
	mApp			= theApp;
	HandleX			= HandleY = HandleWidth = HandleHeight = 0;
	MouseIncX		= -1;
	MouseIncY		= -1;
	daMouseDown		= false;
}

Panel::~Panel()
{

}

void Panel::SetHandle(int x, int y, int width, int height)
{
	HandleX			= x;
	HandleY			= y;
	HandleWidth		= width;
	HandleHeight	= height;
}

bool Panel::Updateb()
{
	//check if mouse clicked..
	if (mApp->mMouseInput->GetButtonDown(LBUTTON))
	{
		int MouseX, MouseY;
		mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);
		if (daMouseDown && MouseIncX > -1 && MouseIncY > -1)
		{
			mX = float(MouseX - MouseIncX);
			mY = float(MouseY - MouseIncY);
		}
		else if (!daMouseDown && MouseX > mX + HandleX && MouseX < mX + HandleX + HandleWidth && MouseY > mY + HandleY && MouseY < mY + HandleY + HandleHeight)
		{
			MouseIncX = MouseX - int(mX);
			MouseIncY = MouseY - int(mY);
		}
		daMouseDown = true;

		if (MouseX > mX && MouseX < mX + mWidth)
			if (MouseY > mY && MouseY < mY + mHeight)
				return true;

		return false;
	}
	else
	{
		daMouseDown = false;
		MouseIncX = -1;
		MouseIncY = -1;
		return false;
	}
}

void Panel::Update()
{

}