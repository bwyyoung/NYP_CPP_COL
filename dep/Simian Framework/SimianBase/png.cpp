#include <Windows.h>
#include "png.h"
#include "png/png.h"
#include <iostream>
#include <../include/math.h>

using namespace std;

PNGFormat::PNGFormat() : ImageFormat()
{

}

PNGFormat::PNGFormat(char * thePath) : ImageFormat(thePath)
{
	Load(thePath);
}

PNGFormat::~PNGFormat()
{

}

bool PNGFormat::Load(char *thePath)
{
	mPath = thePath;

	// Setup filestream
	FILE *inFile = 0;

	fopen_s(&inFile, mPath.c_str(), "rb");
	if (!inFile)
		return false;

	unsigned char sig[8];

	fread(sig, 1, 8, inFile);
	if (!png_check_sig(sig, 8))
		return false;

	// Setup PNG
	png_structp png_ptr = 0;
	png_infop info_ptr = 0;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

	if (!png_ptr)
		return false;

	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, 0, 0);
		return false;
	}

	// Try loading from filestream..
	png_init_io(png_ptr, inFile);
	png_set_sig_bytes(png_ptr, 8);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);

	png_bytep * row_pointer = new png_bytep[png_ptr->height];
	row_pointer = png_get_rows(png_ptr, info_ptr);

	mWidth = png_ptr->width;
	mHeight = png_ptr->height;
	mBitDepth = png_ptr->pixel_depth;

	//get the next best size..
	mActualWidth = 0;
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

	// copy the info in our row pointer to our data buffer...
	// size of data buffer = width * height * pixel size
	mData = new unsigned char[mActualHeight * mActualWidth * mBitDepth/8];

	for (int i = 0; i < mActualHeight; i ++)
	{
		for (int j = 0; j < mActualWidth * mBitDepth/8; j ++)
			if (i < mHeight && j < mWidth * mBitDepth/8)
			{
				mData[i * (mActualWidth * mBitDepth/8) + j] = row_pointer[i][j];
			}
			else
			{
				mData[i * (mActualWidth * mBitDepth/8) + j] = 0;
			}
	}

	delete [] row_pointer;

	fclose(inFile);

	return true;
}