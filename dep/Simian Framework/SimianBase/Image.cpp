#include "Image.h"

using namespace std;

Image::Image() : RenderObject()
{
	mColor = Color(255,255,255,255);
}

Image::~Image()
{

}

bool Image::Load(char * thePath)
{
	mPath = thePath;

	if (mPath.substr(mPath.length() - 3) == "png")
	{
		mType = IMAGE_PNG;
	}
	else if (mPath.substr(mPath.length() - 3) == "gif")
	{
		mType = IMAGE_GIF;
	}
	else if (mPath.substr(mPath.length() - 3) == "gif")
	{
		mType = IMAGE_JPG;
	}

	return true;
}

int Image::GetActualHeight()
{
	return mActualHeight;
}

int Image::GetActualWidth()
{
	return mActualWidth;
}

void Image::SetColor(Color theColor)
{
	mColor = theColor;
}

void Image::SetColor(unsigned char R /* = 255 */, unsigned char G /* = 255 */, unsigned char B /* = 255 */, unsigned char A /* = 255 */)
{
	mColor = Color(R,G,B,A);
}

Color Image::GetColor()
{
	return mColor;
}