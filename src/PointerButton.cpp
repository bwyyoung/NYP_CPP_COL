#include "PointerButton.h"
#include "BaseApp.h"
#include "Image.h"

PointerButton::PointerButton(BaseApp * theApp, Image * ButtonPic):Button(theApp, theApp->GetImageDatabase()["ButtonUp"], theApp->GetImageDatabase()["ButtonDown"])
{
	mPointerPic = ButtonPic;
	SetSize(50, 50);
}

PointerButton::~PointerButton()
{

}

void PointerButton::Render(float x, float y)
{
	Button::Render(x, y);
	//render the extra stuff.
	mPointerPic->SetPosition(x + 8, y + 8);
	mPointerPic->Render();
}

void PointerButton::Render()
{
	Render(mX, mY);
}