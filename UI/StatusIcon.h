//////////////////////////////////////////////////
//
// class for icon display
//
//////////////////////////////////////////////////

//======================================
// include guard
#ifndef UI_STATUSICON_H
#define UI_STATUSICON_H

//======================================
// dependencies
#include "HUDObject.h"
#include "SpriteStrip.h"

//======================================
// the code

class StatusIcon :
	public HUDObject
{
	//images need to be the same size in file for this to function properly
	int m_iImageID;								// index to image so it can be linked with an object
	RECT source;								//location to be drawn from
	RECT destination;							//location to be drawn
	LPDIRECT3DVERTEXBUFFER9 m_pVertexObject;    //vertex buffer for drawing
	CUSTOMVERTEX m_Vertices[4];					//go into the vertex buffer

public:
	StatusIcon(std::wstring, int, int, int, SpriteStrip); //m_wsHOName, x, y, m_iImageID, SpriteStrip
	~StatusIcon(void);

	void *pVertexBuffer;                  //used for rendering
	void drawElement(SpriteStrip);        //draws the StatusIcon pass the spritestrip
	void changeImage(int, SpriteStrip);   //changes the image. first arguement is the index, second is the SpriteStrip
	int getID();                          //returns m_iImageID
	std::wstring getName();               //returns m_wsHOName
};

#endif // !UI_STATUSICON_H
