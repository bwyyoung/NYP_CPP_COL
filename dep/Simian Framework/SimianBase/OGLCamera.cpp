#include "OGLCamera.h"
#include "SimianBase.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLAux.h>
#include <gl/GLU.h>

OGLCamera::OGLCamera(float X /* = 0.0f */, float Y /* = 0.0f */, float Z /* = 0.0f */) : Camera(X, Y, Z)
{
	
}

OGLCamera::~OGLCamera()
{

}

void OGLCamera::Set3DMode()
{

}

void OGLCamera::Set2DMode()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, mSimian->GetWindowWidth(), 0, mSimian->GetWindowHeight());
	glScalef(1, -1, 1);
	glTranslatef(0, (float)-mSimian->GetWindowHeight(), 0);
	glMatrixMode(GL_MODELVIEW);
}