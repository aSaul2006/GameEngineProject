#pragma once
#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>



class Background
{
private:
	IDirect3DSurface9	*background;

	RECT scrRect;
	RECT destRect;
	int					width;
	int					height;
	int					imageScale;

public:

	Background(void);
	virtual ~Background(void);

	bool load(LPDIRECT3DDEVICE9 Device, std::wstring filename);
	void render(LPDIRECT3DDEVICE9 Device);
	void setPosition(int x, int y);
	void setSize(int percent);
	void setScrRect(int left, int top, int height, int width);
	void setDestRect(int left, int top, int height, int width);



};
