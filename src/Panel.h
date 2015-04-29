#pragma once
#include "RenderObject.h"

class BaseApp;

class Panel : public RenderObject
{
public:
	Panel(BaseApp * theApp);
	~Panel();

	bool			Updateb();
	virtual void	Update();
	virtual void	Render(float x, float y) = 0;
	virtual void	Render() = 0;
	void			SetHandle(int x, int y, int width, int height);
protected:
	int				HandleX, 
					HandleY, 
					HandleWidth, 
					HandleHeight;
	bool			daMouseDown;
	int				MouseIncX;
	int				MouseIncY;
	BaseApp *		mApp;
};