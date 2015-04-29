#include "OGLDevice.h"
#include "SimianBase.h"
#include "WinAPI.h"
#include <Windows.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/GLAux.h>

HDC hDC;
HGLRC hRC;

OGLDevice::OGLDevice(SimianBase * theApp):RenderDevice(theApp)
{

}

void OGLDevice::Init()
{
	//Set up all of opengl's stuff...
	hDC = GetDC(hWnd);
	int PixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),   // size of structure.
		1,                               // always 1.
		PFD_DRAW_TO_WINDOW |             // support window
		PFD_SUPPORT_OPENGL |             // support OpenGl
		PFD_DOUBLEBUFFER,                // support double buffering
		PFD_TYPE_RGBA,                   // support RGBA
		16,                              // bit color mode
		0, 0, 0, 0, 0, 0,                // ignore color bits
		0,                               // no alpha buffer
		0,                               // ignore shift bit
		0,                               // no accumulation buffer
		0, 0, 0, 0,                      // ignore accumulation bits.
		16,                              // number of depth buffer bits.
		0,                               // number of stencil buffer bits.
		0,                               // 0 means no auxiliary buffer
		PFD_MAIN_PLANE,                  // The main drawing plane
		0,                               // this is reserved
		0, 0, 0 };                       // layer masks ignored.

	// this chooses the best pixel format and returns index.
	PixelFormat = ChoosePixelFormat(hDC, &pfd);

	// This set pixel format to device context.
	SetPixelFormat(hDC, PixelFormat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	glClearColor((float)mApp->mBackgroundColor.R/255.0f, (float)mApp->mBackgroundColor.G/255.0f, (float)mApp->mBackgroundColor.B/255.0f, 0.0f);
}

void OGLDevice::Cleanup()
{
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

void OGLDevice::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	mSimian->Render();
	SwapBuffers(hDC);
}

void OGLDevice::ChangeSize(int Width, int Height)
{

}