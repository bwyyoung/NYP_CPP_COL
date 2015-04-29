#include "DXImage.h"
#include "D3D9Device.h"
#include <d3dx9.h>

struct textured_vertex
{
	float x, y, z, rhw;
	DWORD diffuse;
	float tu, tv;
};

#define tri_fvf (D3DFVF_XYZRHW|D3DFVF_TEX1|D3DFVF_DIFFUSE)

DXImage::DXImage()
{

}

DXImage::~DXImage()
{

}

bool DXImage::Load(char * thePath)
{
	Image::Load(thePath);

	if (FAILED(D3DXCreateTextureFromFileEx(D3DD, thePath, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_POINT, D3DX_DEFAULT, 0, NULL, NULL,  &mTex)))
		return false;

	D3DSURFACE_DESC Desc;
	mTex->GetLevelDesc(0, &Desc);
	mActualHeight = Desc.Height;
	mActualWidth = Desc.Width;

	SetSize(mActualWidth, mActualHeight);

	return true;
}

void DXImage::Render(float tx, float ty, int SrcX, int SrcY, int SrcWidth, int SrcHeight)
{
	int x = (int)tx;
	int y = (int)ty;
	textured_vertex Vertices[6];
	Vertices[0].x = float(x) - 0.5f;
	Vertices[0].y = float(y) - 0.5f;
	Vertices[0].z = 0.0f;
	Vertices[0].rhw = 1.0f;
	Vertices[0].tu = ((float)(SrcX))/(float)mActualWidth;
	Vertices[0].tv = ((float)(SrcY))/(float)mActualHeight;
	Vertices[1].x = float(x + mWidth) - 0.5f;
	Vertices[1].y = float(y) - 0.5f;
	Vertices[1].z = 0.0f;
	Vertices[1].rhw = 1.0f;
	Vertices[1].tu = ((float)(SrcX + SrcWidth))/(float)mActualWidth;
	Vertices[1].tv = ((float)SrcY)/(float)mActualHeight;
	Vertices[2].x = float(x + mWidth) - 0.5f;
	Vertices[2].y = float(y + mHeight) - 0.5f;
	Vertices[2].z = 0.0f;
	Vertices[2].rhw = 1.0f;
	Vertices[2].tu = ((float)(SrcX + SrcWidth))/(float)mActualWidth;
	Vertices[2].tv = ((float)(SrcY + SrcHeight))/(float)mActualHeight;
	Vertices[3].x = float(x + mWidth) - 0.5f;
	Vertices[3].y = float(y + mHeight) - 0.5f;
	Vertices[3].z = 0.0f;
	Vertices[3].rhw = 1.0f;
	Vertices[3].tu = ((float)(SrcX + SrcWidth))/(float)mActualWidth;
	Vertices[3].tv = ((float)(SrcY + SrcHeight))/(float)mActualHeight;
	Vertices[4].x = float(x) - 0.5f;
	Vertices[4].y = float(y + mHeight) - 0.5f;
	Vertices[4].z = 0.0f;
	Vertices[4].rhw = 1.0f;
	Vertices[4].tu = ((float)SrcX)/(float)mActualWidth;
	Vertices[4].tv = ((float)(SrcY + SrcHeight))/(float)mActualHeight;
	Vertices[5].x = float(x) - 0.5f;
	Vertices[5].y = float(y) - 0.5f;
	Vertices[5].z = 0.0f;
	Vertices[5].rhw = 1.0f;
	Vertices[5].tu = ((float)SrcX)/(float)mActualWidth;
	Vertices[5].tv = ((float)SrcY)/(float)mActualHeight;

	for (int i = 0; i < 6; i ++)
	{
		Vertices[i].diffuse = D3DCOLOR_RGBA(mColor.R, mColor.G, mColor.B, mColor.A);
	}

	D3DD->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	D3DD->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	D3DD->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);   //Ignored

	D3DD->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	D3DD->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	D3DD->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TEXTURE);   //Ignored

	D3DD->SetRenderState(D3DRS_ALPHABLENDENABLE,true);		//alpha blending enabled
	D3DD->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);	//source blend factor
	D3DD->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	//destination blend factor
	
	D3DD->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	D3DD->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	D3DD->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);

	D3DD->SetTexture(0,mTex);

	//D3DD->SetStreamSource(0, mBuffer, 0, sizeof(textured_vertex));
	D3DD->SetFVF(tri_fvf);
	//D3DD->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	D3DD->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, Vertices, sizeof(textured_vertex));
}

void DXImage::Render(float x, float y)
{
	Render(x, y, 0, 0, mActualWidth, mActualHeight);
}

void DXImage::Render(int SrcX, int SrcY, int SrcWidth, int SrcHeight)
{
	Render(mX, mY, SrcX, SrcY, SrcWidth, SrcHeight);
}

void DXImage::Render()
{
	Render(0, 0, mActualWidth, mActualHeight);
}

void DXImage::Update()
{

}