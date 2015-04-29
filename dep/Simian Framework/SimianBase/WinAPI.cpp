#include <windows.h>
#include "WinAPI.h"
#include "SimianBase.h"
#include "Timer.h"
#include "MouseInput.h"
#include "KeyboardInput.h"

#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN

HINSTANCE hInstance;
HWND hWnd;

bool _fullscreen;
bool mRunning = false;

int CurTime = 0;
int LastTime = 0;

ProcessMessageFunc WMChecker;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DestroyWinAPI();

void StartWinAPIMainLoop()
{
	MSG msg;

	while(mRunning)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				mRunning = false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (mSimian->GetUpdateMSPF() > 0)
			{
				int time = (int) GetTickCount();
				if (LastTime != 0)
					CurTime += time - LastTime;

				if (CurTime > mSimian->GetUpdateMSPF())
				{
					mSimian->mTimeDelta = CurTime;
					mSimian->mKeyboardInput->Update();
					mSimian->Update();
					mSimian->RendererRender();
					CurTime = 0;
				}

				LastTime = time;
			}
			else
			{
				mSimian->mKeyboardInput->Update();
				mSimian->Update();
				mSimian->RendererRender();
			}
			mSimian->mTimer->Update();
		}
	}

	DestroyWinAPI();
}

bool InitWinAPI(int Width, int Height, char * Title, bool Fullscreen)
{
	WNDCLASS wc;

	hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "SimianFramework";

	if (!RegisterClass(&wc))
		return false;

	RECT WindowRect;
	WindowRect.left = 0;
	WindowRect.top = 0;
	WindowRect.right = Width;
	WindowRect.bottom = Height;

	_fullscreen = Fullscreen;

	if (Fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = Width;
		dmScreenSettings.dmPelsHeight = Height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			return false;

		hWnd = CreateWindowEx(WS_EX_APPWINDOW, "SimianFramework", Title, WS_POPUP, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInstance, NULL);
	}
	else
	{
		AdjustWindowRectEx(&WindowRect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW|WS_EX_WINDOWEDGE);

		hWnd = CreateWindowEx(WS_EX_APPWINDOW|WS_EX_WINDOWEDGE, "SimianFramework", Title, WS_OVERLAPPEDWINDOW, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInstance, NULL);
	}

	if (!hWnd)
		return false;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	mRunning = true;

	return true;
}

void DestroyWinAPI()
{
	mRunning = false;

	mSimian->CleanupRenderer();

	if (hWnd)
		DestroyWindow(hWnd);

	if (_fullscreen)
		ChangeDisplaySettings(NULL, 0);

	UnregisterClass("SimianFramework", hInstance);
}

LRESULT CALLBACK WndProc(HWND hWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int height;
	int width;

	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		mSimian->mMouseInput->Update(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_CLOSE:
		mRunning = false;
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);

		if (height < 240)
			height = 240;
		if (width < 320)
			width = 320;

		mSimian->SetWindowSize(width, height);
		return 0;
	}

	if (WMChecker)
		if (WMChecker(uMsg))
			return 0;

	return DefWindowProc(hWND, uMsg, wParam, lParam);
}

void ProcessMessage(ProcessMessageFunc a)
{
	WMChecker = a;
}