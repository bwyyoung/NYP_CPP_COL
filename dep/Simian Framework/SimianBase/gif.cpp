#include "gif.h"
#include "gif/gif_lib.h"
#include <../include/math.h>

GIFFormat::GIFFormat() : ImageFormat()
{

}

GIFFormat::GIFFormat(char * thePath) : ImageFormat(thePath)
{
	Load(thePath);
}

GIFFormat::~GIFFormat()
{

}

#include <iostream>

using namespace std;

bool GIFFormat::Load(char * thePath)
{
	GifFileType * curImage = 0;
	curImage = DGifOpenFileName(thePath);

	if (!curImage)
		return false;

	if (DGifSlurp(curImage) == GIF_ERROR)
		return false;

	mWidth = curImage->Image.Width;
	mHeight = curImage->Image.Height;
	mBitDepth = curImage->SColorMap->BitsPerPixel;

	//for (int i = 0; i < mWidth * mHeight; i ++)
	//	cout << (int)curImage->SColorMap->Colors[curImage->SavedImages[0].RasterBits[i]].Red << " " << (int)curImage->SColorMap->Colors[curImage->SavedImages[0].RasterBits[i]].Green << " " << (int)curImage->SColorMap->Colors[curImage->SavedImages[0].RasterBits[i]].Blue << endl;

	mActualWidth = 0;
	mActualHeight = 0;
	int Power = 1;

	if (mWidth > mHeight)
	{
		while (mActualWidth < mWidth)
		{
			mActualWidth =(int) pow((double) 2, Power);
			Power ++;
		}
		mActualHeight = mActualWidth;
	}
	else
	{
		while (mActualHeight < mHeight)
		{	
			mActualHeight =(int) pow((double) 2, Power);
			Power ++;
		}
		mActualWidth = mActualHeight;
	}

	mData = new unsigned char[mActualWidth * mActualHeight * 4];

	for (int i = 0; i < mActualHeight; i ++)
	{
		for (int j = 0; j < mActualWidth; j ++)
		{
			if (i < mHeight && j < mWidth)
			{
				GifColorType & a = curImage->SColorMap->Colors[curImage->SavedImages[0].RasterBits[i * mWidth + j]];
				mData[(i * mActualWidth + j) * 4] = a.Red;
				mData[(i * mActualWidth + j) * 4 + 1] = a.Green;
				mData[(i * mActualWidth + j) * 4 + 2] = a.Blue;
				mData[(i * mActualWidth + j) * 4 + 3] = 255;
			}
			else
			{
				mData[(i * mActualWidth + j) * 4] = 0;
				mData[(i * mActualWidth + j) * 4 + 1] = 0;
				mData[(i * mActualWidth + j) * 4 + 2] = 0;
				mData[(i * mActualWidth + j) * 4 + 3] = 255;
			}
		}
	}

	mBitDepth = 32;

	DGifCloseFile(curImage);
	return true;
}