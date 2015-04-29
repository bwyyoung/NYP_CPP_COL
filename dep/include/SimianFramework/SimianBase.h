#include <string>
#include "Macros.h"
#include "Color.h"

using namespace std;

class Image;
class Camera;
class RenderDevice;
class Timer;
class MouseInput;
class KeyboardInput;

enum RENDERER
{
	RENDERER_OPENGL, RENDERER_DIRECTX
};

class SIM_EXP SimianBase
{
public:
	SimianBase(RENDERER theRenderer);
	~SimianBase();
	void Setup(char * Title, int Width, int Height, bool Fullscreen=false, Color bgColor = Color()); // Initializes all needed variables
	void Start(); // Starts the main loop

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Cleanup() = 0;

	void CleanupRenderer();
	void RendererRender();

	Image * CreateImage(char * thePath);
	Camera * CreateCamera();

	//Sets
	void SetWindowSize(int Width, int Height);
	void SetTitle(char * theTitle);
	void SetFullscreen(bool Fullscreen);
	void SetUpdateRate(int CyclesPerSecond);

	//Gets
	int GetWindowWidth();
	int GetWindowHeight();
	string GetTitle();
	bool GetFullscreen();
	int GetUpdateRate();
	int GetUpdateMSPF();

	void MainLoop();

	Timer * mTimer;

	int mTimeDelta;

	//input devices..
	MouseInput * mMouseInput;
	KeyboardInput * mKeyboardInput;
private:
	int mUpdateMSPF;
	RENDERER mRenderer;
	RenderDevice *mDevice;
	int mWidth;
	int mHeight;
	string mTitle;
	bool mFullscreen;
	Color mBackgroundColor;

	friend class OGLDevice;
	friend class D3D9Device;
};

extern SimianBase * mSimian;