#include "GLImage.h"
#include "png.h"
#include "gif.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLAux.h>
#include <gl/GLU.h>
#include <math.h>

GLImage::GLImage() : Image()
{

}

GLImage::~GLImage()
{

}

bool GLImage::Load(char * thePath)
{
	Image::Load(thePath);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	ImageFormat * format = 0;
	if (mType == IMAGE_PNG)
	{
		format = new PNGFormat();
		if (!format->Load(thePath))
			return false;
	}
	else if (mType == IMAGE_GIF)
	{
		format = new GIFFormat();
		if (!format->Load(thePath))
			return false;
	}

	if (!format)
		return false;

	GLenum theFormat;
	if (format->GetBitDepth() == 32)
		theFormat = GL_RGBA;
	else if (format->GetBitDepth() == 24)
		theFormat = GL_RGB;
	else
		theFormat = GL_RGB;

	mWidth = format->GetWidth();
	mHeight = format->GetHeight();

	mOriginalWidth = format->GetActualWidth();
	mOriginalHeight = format->GetActualHeight();

	gluBuild2DMipmaps(GL_TEXTURE_2D, format->GetBitDepth()/8, format->GetActualWidth(), format->GetActualHeight(), theFormat, GL_UNSIGNED_BYTE, format->GetData());
	format->Unload();
	delete format;

	mActualHeight = mHeight;
	mActualWidth = mWidth;

	return true;
}

#include <iostream>
using namespace std;

bool GLImage::LoadUnfiltered(char * thePath)
{
	Image::Load(thePath);

	// Set properties.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);

	ImageFormat * format = 0;
	if (mType == IMAGE_PNG)
	{
		format = new PNGFormat();
		if (!format->Load(thePath))
			return false;
	}
	else if (mType == IMAGE_GIF)
	{
		format = new GIFFormat();
		if (!format->Load(thePath))
			return false;
	}

	if (!format)
		return false;

	GLenum theFormat;
	if (format->GetBitDepth() == 32)
		theFormat = GL_RGBA;
	else if (format->GetBitDepth() == 24)
		theFormat = GL_RGB;
	else
		theFormat = GL_RGB;

	mHeight = format->GetHeight();
	mWidth = format->GetWidth();

	mOriginalWidth = format->GetActualWidth();
	mOriginalHeight = format->GetActualHeight();

	// Create the texture.
	glTexImage2D(GL_TEXTURE_2D, 0, format->GetBitDepth()/8, format->GetActualWidth(), format->GetActualHeight(), 0, theFormat, GL_UNSIGNED_BYTE, format->GetData());

	format->Unload();
	delete format;

	mActualHeight = mHeight;
	mActualWidth = mWidth;

	return true;
}

void GLImage::Render()
{
	glMatrixMode(GL_TEXTURE);
	glScalef(1.0f/(float)mOriginalWidth,1.0f/(float)mOriginalHeight,1.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, mId);
	glColor4ub(mColor.R, mColor.G, mColor.B, mColor.A);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(mX, mY);
	glTexCoord2f(0, float(mActualHeight));
	glVertex2f(mX, mY + mHeight);
	glTexCoord2f((float)mActualWidth, (float)mActualHeight);
	glVertex2f(mX + mWidth, mY + mHeight);
	glTexCoord2f((float)mActualWidth, 0);
	glVertex2f(mX + mWidth, mY);
	glEnd();
	glColor4ub(255,255,255,255);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void GLImage::Render(float x, float y)
{
	glMatrixMode(GL_TEXTURE);
	glScalef(1.0f/(float)mOriginalWidth,1.0f/(float)mOriginalHeight,1.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, mId);
	glColor4ub(mColor.R, mColor.G, mColor.B, mColor.A);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(0, float(mActualHeight));
	glVertex2f(x, y + mHeight);
	glTexCoord2f((float)mActualWidth, (float)mActualHeight);
	glVertex2f(x + mWidth, y + mHeight);
	glTexCoord2f((float)mActualWidth, 0);
	glVertex2f(x + mWidth, y);
	glEnd();
	glColor4ub(255,255,255,255);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void GLImage::Update()
{

}

void GLImage::Render(float x, float y, int SrcX, int SrcY, int SrcWidth, int SrcHeight)
{
	glMatrixMode(GL_TEXTURE);
	glScalef(1.0f/(float)mOriginalWidth,1.0f/(float)mOriginalHeight,1.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, mId);
	glBegin(GL_QUADS);
	glTexCoord2f((float)SrcX, (float)SrcY);
	glVertex2f(x, y);
	glTexCoord2f((float)SrcX, (float)SrcY + (float)SrcHeight);
	glVertex2f(x, y + mHeight);
	glTexCoord2f((float)SrcX + (float)SrcWidth, (float)SrcY + (float)SrcHeight);
	glVertex2f(x + mWidth, y + mHeight);
	glTexCoord2f((float)SrcX + (float)SrcWidth, (float)SrcY);
	glVertex2f(x + mWidth, y);
	glEnd();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void GLImage::Render(int SrcX, int SrcY, int SrcWidth, int SrcHeight)
{
	Render(mX, mY, SrcX, SrcY, SrcWidth, SrcHeight);
}