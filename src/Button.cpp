#include "Button.h"
#include "Image.h"
#include "MouseInput.h"
#include "BaseApp.h"

Button::Button(BaseApp * theApp, Image * UIPicture, Image * UIDown)
{
	mApp			= theApp;
	mUIPicture		= UIPicture;
	mUIDownPicture	= UIDown;
	mButtonDown		= false;
	mMouseDown		= false;
}

Button::~Button()
{

}

void Button::Update()
{

}

int Button::Updateb()
{
	//returns 1 for button down, 2 for button released, 0 for not pressed.
	int MouseX, MouseY;
	if (mApp->mMouseInput->GetButtonDown(LBUTTON))
	{
		mApp->mMouseInput->GetMousePosition(&MouseX, &MouseY);

		if (mMouseDown == false)
			if (MouseX > mX && MouseX < mX + mWidth)
				if (MouseY > mY && MouseY < mY + mHeight)
				{
					mButtonDown = true;
					return 1;
				}
				else
				{
					mButtonDown = false;
				}

		mMouseDown = true;
	}
	else
	{
		bool Ret = false;
		if (mButtonDown == true)
			Ret = true;
		mButtonDown = false;
		mMouseDown  = false;
		if (Ret) return 2;
	}

	return 0;
}

void Button::Render(float x, float y)
{
	//draw the button according to size.
	Image * CurPic = 0;

	if (mButtonDown)
		CurPic = mUIDownPicture;
	else
		CurPic = mUIPicture;

	// Top Border..
	CurPic->SetSize(10, 10);
	CurPic->Render(x, y, 0, 0, 10, 10);
	CurPic->SetSize(mWidth - 20, 10);
	CurPic->Render(x + 10, y, 10, 0, 10, 10);
	CurPic->SetSize(10, 10);
	CurPic->Render(x + mWidth - 10, y, 20, 0, 10, 10);

	// Bottom Border...
	CurPic->SetSize(10, 10);
	CurPic->Render(x, y + mHeight - 10, 0, 20, 10, 10);
	CurPic->SetSize(mWidth - 20, 10);
	CurPic->Render(x + 10, y + mHeight - 10, 10, 20, 10, 10);
	CurPic->SetSize(10, 10);
	CurPic->Render(x + mWidth - 10, y + mHeight - 10, 20, 20, 10, 10);

	// Left Border...
	CurPic->SetSize(10, mHeight - 20);
	CurPic->Render(x, y + 10, 0, 10, 10, 10);

	// Right Border...
	CurPic->SetSize(10, mHeight - 20);
	CurPic->Render(x + mWidth - 10, y + 10, 20, 10, 10, 10);

	// Background..
	CurPic->SetSize(mWidth - 20, mHeight - 20);
	CurPic->Render(x + 10, y + 10, 10, 10, 10, 10);
}

void Button::Render()
{
	Render(mX, mY);
}