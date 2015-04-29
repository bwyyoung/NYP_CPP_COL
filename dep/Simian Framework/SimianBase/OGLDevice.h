#include "RenderDevice.h"

class OGLDevice : public RenderDevice
{
public:
	OGLDevice(SimianBase * theApp);
	virtual void Init();
	virtual void Render();
	virtual void Cleanup();
	virtual void ChangeSize(int Width, int Height);
};