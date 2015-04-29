#include "TextButton.h"
#include "BaseApp.h"
#include "BitmappedFont.h"
#include "Image.h"

TextButton::TextButton(BaseApp * theApp, char * ButtonText, int FontSize):Button(theApp, theApp->CreateImage("../data/ButtonNormal.png"), theApp->CreateImage("../data/ButtonDown.png"))
{
	mButtonText = ButtonText;
	//estimate button size..
	SetSize(static_cast<int>(mButtonText.length()) * FontSize + 15, FontSize + 10);
	mFontSize = FontSize;
	mFontTexture = theApp->CreateImage("../data/BitmapFont.png");
	mFont = new BitmappedFont(mApp, mFontTexture);
}

TextButton::~TextButton()
{
	delete mFont;
	delete mFontTexture;
}

void TextButton::Render(float x, float y)
{
	Button::Render(x, y);
	mFont->DrawString(x + 5, y + 5, (char *)mButtonText.c_str(), mFontSize);
}

void TextButton::Render()
{
	Button::Render();
	Render(mX, mY);
}