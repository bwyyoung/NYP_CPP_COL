#include "D3D9Device.h"
#include "SimianBase.h"
#include "WinAPI.h"

#define _D3DFORMATCHECK(theformat) if (format == D3DFMT_UNKNOWN && SUCCEEDED(mD3D9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, theformat, theformat, false))) format = theformat;

LPDIRECT3DDEVICE9 D3DD;

D3D9Device::D3D9Device(SimianBase * theApp) : RenderDevice(theApp)
{
	mD3D9 = 0;
	mD3DD9 = 0;
}

void D3D9Device::Init()
{
	mD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!mD3D9)
		//display error message.
		return;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;
	if (mApp->GetFullscreen())
	{
		d3dpp.Windowed = FALSE;

		D3DFORMAT format = D3DFMT_UNKNOWN;

		//do a check to see which 16 bit format is available.
		_D3DFORMATCHECK(D3DFMT_R5G6B5);
		_D3DFORMATCHECK(D3DFMT_X1R5G5B5);
		_D3DFORMATCHECK(D3DFMT_A8B8G8R8);

		d3dpp.BackBufferFormat = format;

		d3dpp.BackBufferWidth = mApp->GetWindowWidth();
		d3dpp.BackBufferHeight = mApp->GetWindowHeight();
	}
	else 
	{
		d3dpp.Windowed = TRUE;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	}
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	
	if (FAILED(mD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &mD3DD9)))
	{
		//show error message.
		return;
	}

	D3DD = mD3DD9;
}

void D3D9Device::Render()
{
	HRESULT hr;

	hr = mD3DD9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(mApp->mBackgroundColor.R, mApp->mBackgroundColor.G, mApp->mBackgroundColor.B, 0), 1.0f, 0);
	if (SUCCEEDED(mD3DD9->BeginScene()))
	{
		mApp->Render();
		mD3DD9->EndScene();
	}
	hr = mD3DD9->Present(NULL, NULL, NULL, NULL);

	if (hr == D3DERR_DEVICELOST)
		return;
	if (hr == D3DERR_DRIVERINTERNALERROR)
		return;
	if (hr==D3DERR_INVALIDCALL)
		return;
	if (FAILED(hr))
		return;
}

void D3D9Device::Cleanup()
{
	if (mD3DD9)
		mD3DD9->Release();
	if (mD3D9)
		mD3D9->Release();

	mD3D9 = 0;
	mD3DD9 = 0;
}

void D3D9Device::ChangeSize(int Width, int Height)
{

}