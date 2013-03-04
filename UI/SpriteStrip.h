//////////////////////////////////////////////////
//
// class for aiding with HUD image display
//
//////////////////////////////////////////////////

//======================================
// include guard
#ifndef UI_SPRITESTRIP_H
#define UI_SPRITESTRIP_H

//======================================
// dependencies
#include "stdafx.h"
#include <string>

//======================================
// the code

class SpriteStrip
{
	RECT m_desiredSprite;     // holds the location of this sprite in the source bitmap
	int m_iNumberOfSprites;
	int m_iSpriteWidth;
	D3DXIMAGE_INFO imageInfo; //used for offscreen surface
public:
	SpriteStrip(std::wstring, int);
	~SpriteStrip(void);

	RECT getSourceRect(int, float); //takes imageID and 1.0 for icon or a scaling value for a statusbar
	int SSgetImageHeight();
	int SSgetImageWidth();
	IDirect3DSurface9* m_surface;
};

#endif // !UI_SPRITESTRIP_H
