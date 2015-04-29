#include "Disaster.h"
#include "BaseApp.h"
#include "Image.h"
#include "MouseInput.h"
#include "BitmappedFont.h"

using namespace std;

Disaster::Disaster(BaseApp * theApp) : Panel(theApp)
{
	mFont			= new BitmappedFont(theApp, mApp->CreateImage("../data/BitmapFont.png", "StatusPaneFont"));
	mBGImage		= mApp->CreateImage("../data/StatusPane.png", "StatusPaneBG");
}

Disaster::~Disaster()
{
	delete mFont;
}

void Disaster::Update()
{

}

bool Disaster::Updateb()
{
	return Panel::Updateb();
}

void Disaster::Render(float x, float y)
{
}

void Disaster::Render()
{
}