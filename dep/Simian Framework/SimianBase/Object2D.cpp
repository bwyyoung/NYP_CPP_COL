#include "Object2D.h"

Object2D::Object2D(float X, float Y, int Width, int Height)
{
	mX = X;
	mY = Y;
	mWidth = Width;
	mHeight = Height;
}

Object2D::~Object2D()
{

}

void Object2D::SetPosition(float X, float Y)
{
	mX = X;
	mY = Y;
}

void Object2D::SetX(float X)
{
	mX = X;
}

void Object2D::SetY(float Y)
{
	mY = Y;
}

void Object2D::SetSize(int Width, int Height)
{
	mWidth = Width;
	mHeight = Height;
}

void Object2D::SetWidth(int Width)
{
	mWidth = Width;
}

void Object2D::SetHeight(int Height)
{
	mHeight = Height;
}

float Object2D::GetX()
{
	return mX;
}

float Object2D::GetY()
{
	return mY;
}

int Object2D::GetWidth()
{
	return mWidth;
}

int Object2D::GetHeight()
{
	return mHeight;
}