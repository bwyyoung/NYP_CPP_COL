#include "ToolbarButton.h"
#include "Sprite.h"
#include "BaseApp.h"
#include "GameState.h"

ToolbarButton::ToolbarButton(BaseApp * theApp, Sprite * Pic) : Button(theApp, theApp->mGameState->GetImageDatabase()["ButtonUp"], theApp->mGameState->GetImageDatabase()["ButtonDown"])
{
	mPic = Pic;
	SetSize(40, 40);
	mPic->PlayAnimation(0);
	mPic->SetSize(32, 32);
}

ToolbarButton::~ToolbarButton()
{

}

void ToolbarButton::Render(float x, float y)
{
	Button::Render(x, y);
	mPic->Render(x + 4, y + 4);
}

void ToolbarButton::Render()
{
	Render(mX, mY);
}

int ToolbarButton::Updateb()
{
	mPic->Update();
	return Button::Updateb();
}