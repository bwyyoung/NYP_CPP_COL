#include "BitmappedFont.h"
#include "BaseApp.h"
#include "Sprite.h"
#include "Image.h"
#include <string>

using namespace std;

BitmappedFont::BitmappedFont(BaseApp * theApp, Image * theTexture)
{
	mFontSprite = theTexture;
}

BitmappedFont::~BitmappedFont()
{
	delete mFontSprite;
}

#include <iostream>

using namespace std;

void BitmappedFont::DrawString(float X, float Y, char *theText, int Size)
{
	string a = theText;
	for (int i = 0; i < (int) a.length(); i ++)
	{
		int CharInd = -1;
		if (a[i] > 64 && a[i] < 91)
			CharInd = (int)a[i] - 65;
		else if (a[i] > 96 && a[i] < 123)
			CharInd = (int)a[i] - 97;
		else if (a[i] > 47 && a[i] < 58)
		{
			//cout << (int)a[i] << endl;
			CharInd = (int)a[i] - 48 + 26;
		}
		else if (a[i] == ' ')
			CharInd = -1;

		if (CharInd == -1)
			continue;

		int Height = 0;
		if (CharInd > 25)
		{
			CharInd -= 26;
			Height ++;
			//cout << CharInd << endl;
		}

		mFontSprite->SetSize(Size,Size);
		mFontSprite->Render(X + i * (Size + 1), Y, CharInd * 13, Height * 13, 13, 13);
	}
}