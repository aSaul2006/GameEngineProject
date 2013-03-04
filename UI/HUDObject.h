//////////////////////////////////////////////////
//
// Parent class for all HUD elements
//
//////////////////////////////////////////////////

//======================================
// include guard
#ifndef UI_HUDOBJECT_H
#define UI_HUDOBJECT_H

//======================================
// dependencies
#include "stdafx.h"
#include <string>

//======================================
// the code

class HUDObject
{
protected:
	int m_iHOLocX;           // x-coordinate on screen for the object
	int m_iHOLocY;           // y-coordinate on the screen for the object
	int m_iHOLocZ;           // z-index of the object for drawing
	std::wstring m_wsHOName; // the object's name to be displayed in textboxes

	// structure for vertices
	struct CUSTOMVERTEX
	{
		float x, y, z, rhw;
		DWORD color;
	};
public:
	HUDObject(void) {}
	virtual ~HUDObject(void){}
};

#endif // !UI_HUDOBJECT
