#include "Background.h"

Background::Background(void)
{
	background = NULL;
}

Background::~Background(void)
{
	if( background != NULL) 
	{
		background->Release();
		background = NULL;
	}
}

bool Background::load(LPDIRECT3DDEVICE9 Device, std::wstring filename)
{
	imageScale = 100;

	HRESULT hr;
	D3DXIMAGE_INFO imageInfo;
	hr = D3DXGetImageInfoFromFile(filename.c_str(), &imageInfo);
	if(FAILED(hr))
	{
		return false;
	}

	height = imageInfo.Height;
	width = imageInfo.Width;

	hr = Device->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &background, NULL);
	if(FAILED(hr))
	{
		return false;
	}

	//load from file
	hr = D3DXLoadSurfaceFromFile(background, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	if(FAILED(hr))
	{
		return false;
	}

	//Set Rects
	scrRect.left = 0;
	scrRect.top = 0;
	scrRect.bottom = 0;
	scrRect.top = 0;

	destRect.left = 0;
	destRect.top = 0;
	destRect.bottom = 0;
	destRect.right = 0;

	return true;
}

void Background::render(LPDIRECT3DDEVICE9 Device)
{
	destRect.bottom = destRect.top + (int)(height * (imageScale / 100));
	destRect.right = destRect.left + (int)(width * (imageScale / 100));

	IDirect3DSurface9	*backBuffer = NULL;
	Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	Device->StretchRect(background, &scrRect, backBuffer, &destRect, D3DTEXF_NONE);

}

void Background::setPosition(int x, int y)
{
	destRect.left = x;
	destRect.top = y;
	destRect.bottom = destRect.top + height;
	destRect.right = destRect.left + width;
}

void Background::setSize(int percent)
{
	imageScale = percent;
}

void Background::setScrRect(int left, int top, int height, int width)
{
	scrRect.left = left;
	scrRect.top = top;
	scrRect.bottom = scrRect.top + height;
	scrRect.right = scrRect.left + width;
}

void Background::setDestRect(int left, int top, int height, int width)
{
	destRect.left = left;
	destRect.top = top;
	destRect.bottom = destRect.top + height;
	destRect.right = destRect.left + width;
}
