#include "SpriteStrip.h"


SpriteStrip::SpriteStrip(std::wstring fn, int NOSIS)
{
	D3DXGetImageInfoFromFile(fn.c_str(), &imageInfo);
	g_pD3DDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_surface, NULL);
	D3DXLoadSurfaceFromFile(m_surface, NULL, NULL, fn.c_str(), NULL, D3DX_DEFAULT, 0, NULL);

	m_iSpriteWidth = (imageInfo.Width/NOSIS);

	//Defaults m_desiredSprite to the first image in the strip
	m_desiredSprite.left = 0;
	m_desiredSprite.top = 0;
	m_desiredSprite.right = m_iSpriteWidth;
	m_desiredSprite.bottom = imageInfo.Height;
}

SpriteStrip::~SpriteStrip() {}

RECT SpriteStrip::getSourceRect(int index, float scale)
{
	m_desiredSprite.left = index * m_iSpriteWidth;
	m_desiredSprite.right = long((index * m_iSpriteWidth) + (scale * m_iSpriteWidth) );
	m_desiredSprite.top = 0;
	m_desiredSprite.bottom =  imageInfo.Height;

	return m_desiredSprite;
}

int SpriteStrip::SSgetImageHeight()
{
	return imageInfo.Height;
}

int SpriteStrip::SSgetImageWidth()
{
	return m_iSpriteWidth;
}
