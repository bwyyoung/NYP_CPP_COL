#pragma once
#include <Windows.h>
#include "Macros.h"

extern HINSTANCE hInstance;
extern HWND hWnd;

typedef bool(*ProcessMessageFunc)(UINT uMsg);

bool SIM_EXP InitWinAPI(int Width, int Height, char * Title, bool Fullscreen);
void SIM_EXP StartWinAPIMainLoop();
void SIM_EXP DestroyWinAPI();
void SIM_EXP ProcessMessage(ProcessMessageFunc a);

extern bool SIM_EXP mRunning;
