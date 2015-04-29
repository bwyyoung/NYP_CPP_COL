#pragma once
#include "Macros.h"
#include <string>

using namespace std;

class SIM_EXP ImageFormat
{
public:
	ImageFormat();
	ImageFormat(char * thePath);
	~ImageFormat();

	virtual bool Load(char * thePath) = 0;
	void Unload();

	//Gets
	string GetPath();
	int GetWidth();
	int GetHeight();
	int GetActualWidth();
	int GetActualHeight();
	int GetBitDepth();
	unsigned char * GetData();
protected:
	string mPath;
	int mWidth;
	int mHeight;
	int mActualWidth;
	int mActualHeight;
	int mBitDepth;
	unsigned char * mData;
};