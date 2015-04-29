#include "SimianBase.h"
#include "WinAPI.h"
#include "Image.h"
#include "OGLDevice.h"
#include "GLImage.h"
#include "OGLCamera.h"
#include "Timer.h"
#include "D3D9Device.h"
#include "DXImage.h"
#include "DXCamera.h"
#include "MouseInput.h"
#include "KeyboardInput.h"

SimianBase * mSimian;

SimianBase::SimianBase(RENDERER theRenderer)
{
	mTitle = "";
	mWidth = mHeight = 0;
	mRenderer = theRenderer;
	mDevice = 0;

	mUpdateMSPF = 0;
	mTimer = new Timer();
	mMouseInput = new MouseInput();
	mKeyboardInput = new KeyboardInput();
}

SimianBase::~SimianBase()
{
	SAFE_DELETE(mDevice);
	SAFE_DELETE(mTimer);
	SAFE_DELETE(mMouseInput);
	SAFE_DELETE(mKeyboardInput);
}

void SimianBase::RendererRender()
{
	mTimer->RenderUpdate();
	mDevice->Render();
}

void SimianBase::CleanupRenderer()
{
	mDevice->Cleanup();
}

void SimianBase::Setup(char *Title, int Width, int Height, bool Fullscreen, Color bgColor)
{
	mTitle = Title;
	mWidth= Width;
	mHeight = Height;

	mSimian = this;
	mFullscreen = Fullscreen;
	mBackgroundColor = bgColor;

	InitWinAPI(Width, Height, Title, Fullscreen);

	//set up according to renderer.
	if (mRenderer == RENDERER_OPENGL)
	{
		mDevice = new OGLDevice(this);
	}
	if (mRenderer == RENDERER_DIRECTX)
	{
		mDevice = new D3D9Device(this);
	}

	mDevice->Init();
}

void SimianBase::Start()
{
	StartWinAPIMainLoop();
}

void SimianBase::SetWindowSize(int Width, int Height)
{
	mWidth = Width;
	mHeight = Height;

	if (mDevice)
		mDevice->ChangeSize(Width, Height);
}

Image * SimianBase::CreateImage(char * thePath)
{
	if (mRenderer == RENDERER_OPENGL)
	{
		GLImage * newImage = new GLImage();
		if (newImage->Load(thePath))
			return (Image *)newImage;
		else
			return 0;
	}
	if (mRenderer == RENDERER_DIRECTX)
	{
		DXImage * newImage = new DXImage();
		if (newImage->Load(thePath))
			return (Image *)newImage;
		else
			return 0;
	}

	return 0;
}

Camera * SimianBase::CreateCamera()
{
	if (mRenderer == RENDERER_OPENGL)
		return (Camera *) new OGLCamera();
	if (mRenderer == RENDERER_DIRECTX)
		return (Camera *) new DXCamera();

	return 0;
}

int SimianBase::GetWindowHeight()
{
	return mHeight;
}

int SimianBase::GetWindowWidth()
{
	return mWidth;
}

string SimianBase::GetTitle()
{
	return mTitle;
}

void SimianBase::SetUpdateRate(int CyclesPerSecond)
{
	mUpdateMSPF = 1000/CyclesPerSecond;
}

int SimianBase::GetUpdateRate()
{
	return 1000/mUpdateMSPF;
}

int SimianBase::GetUpdateMSPF()
{
	return mUpdateMSPF;
}

bool SimianBase::GetFullscreen()
{
	return mFullscreen;
}