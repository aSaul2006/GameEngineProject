//////////////////////////////////////////////////
//
// class for status bars, e.g. health, xp, etc.
//
//////////////////////////////////////////////////

//======================================
// include guard
#ifndef UI_STATUSBAR_H
#define UI_STATUSBAR_H

//======================================
// dependencies
#include "HUDObject.h"
#include "SpriteStrip.h"

//======================================
// the code

class StatusBar :
	public HUDObject
{
	float m_fValue;								// size of the bar
	float m_fMaxValue;							//max amount of m_fValue
	int m_iImageID;								//for use when muliple bar sprites are in spritestrip. have to be same size

	RECT source;								//location to be drawn from
	RECT destination;							//location to be drawn
	LPDIRECT3DVERTEXBUFFER9 m_pVertexObject;    //vertex buffer for drawing
	CUSTOMVERTEX m_Vertices[4];					//go into the vertex buffer
	
public:
	StatusBar(std::wstring, int, int, int, float, float, SpriteStrip);	//m_wsHOName, x, y, m_iImageID, m_fValue, m_fMaxValue, SpriteStrip
	~StatusBar(void);

	void *pVertexBuffer;					//used for rendering
	void drawElement(SpriteStrip);			//draws element
	void modifyValue(float, SpriteStrip);	//increase and decrease the current bar width
	float getValue();						//returns m_fValue;
	std::wstring getName();					//returns m_wsHOName
	void changeImage(int);
};

#endif // !UI_STATUSBAR_H
