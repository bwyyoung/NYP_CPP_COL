#pragma once

class BaseApp;
class Image;
class Sprite;

class BitmappedFont
{
public:
	BitmappedFont(BaseApp * theApp, Image * theTexture);
	~BitmappedFont();

	void DrawString(float X, float Y, char * theText, int Size = 13);

	int weed;
protected:
	Image * mFontSprite;
	int CharacterIndex[36];
};