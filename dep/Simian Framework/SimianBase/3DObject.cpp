#include "3DObject.h"

Object3D::Object3D()
{
	mX = mY = mZ = 0.0f;
}

Object3D::Object3D(float theX /* = 0 */, float theY /* = 0 */, float theZ /* = 0 */)
{
	mX = theX;
	mY = theY;
	mZ = theZ;
}

Object3D::~Object3D()
{

}

void Object3D::SetX(float theX)
{
	mX = theX;
}

void Object3D::SetY(float theY)
{
	mY = theY;
}

void Object3D::SetZ(float theZ)
{
	mZ = theZ;
}

void Object3D::SetPosition(float theX, float theY, float theZ)
{
	mX = theX;
	mY = theY;
	mZ = theZ;
}

float Object3D::GetX()
{
	return mX;
}

float Object3D::GetY()
{
	return mY;
}

float Object3D::GetZ()
{
	return mZ;
}