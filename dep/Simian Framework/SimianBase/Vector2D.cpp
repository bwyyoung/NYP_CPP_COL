#include "Vector2D.h"
#include <Math.h>

Vector2D::Vector2D(float X, float Y)
{
	mX = X;
	mY = Y;
}

Vector2D::~Vector2D()
{

}

float Vector2D::Magnitude()
{
	return (float)sqrt((double)(mX * mX + mY * mY));
}

float Vector2D::Angle()
{
	float x;
	x = mX;
	float Mag = Magnitude();

	return (float)asin(double(x/Mag))/3.142f * 180.0f;
}

Vector2D Vector2D::operator +(Vector2D a)
{
	return Vector2D(mX + a.mX, mY + a.mY);
}

Vector2D Vector2D::operator -(Vector2D a)
{
	return Vector2D(mX - a.mX, mY - a.mY);
}

Vector2D Vector2D::operator+(float a)
{
	return Vector2D(mX + a, mY + a);
}

Vector2D Vector2D::operator-(float a)
{
	return Vector2D(mX - a, mY - a);
}