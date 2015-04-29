#include "ImageFormat.h"

ImageFormat::ImageFormat()
{

}

ImageFormat::ImageFormat(char * thePath)
{
	mPath = thePath;
}

ImageFormat::~ImageFormat()
{

}

void ImageFormat::Unload()
{
	delete [] mData;
}

string ImageFormat::GetPath()
{
	return mPath;
}

int ImageFormat::GetWidth()
{
	return mWidth;
}

int ImageFormat::GetHeight()
{
	return mHeight;
}

int ImageFormat::GetBitDepth()
{
	return mBitDepth;
}

unsigned char * ImageFormat::GetData()
{
	return mData;
}

int ImageFormat::GetActualWidth()
{
	return mActualWidth;
}

int ImageFormat::GetActualHeight()
{
	return mActualHeight;
}