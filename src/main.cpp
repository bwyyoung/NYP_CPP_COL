#include <Windows.h>
#include "BaseApp.h"
#include "Console.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int CmdShow)
{
	srand(GetTickCount());
#ifdef _DEBUG
	RedirectIOToConsole();
#endif
	RedirectIOToConsole();
	BaseApp theApp;
	theApp.Setup("Project COL", 800, 600, false, Color(255,255,255));
	theApp.Start();	
	return 0;
}