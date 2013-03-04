//////////////////////////////////////////////////
//
// class for displaying text boxes
//
//////////////////////////////////////////////////

//======================================
// include guard
#ifndef UI_TEXTBOX_H
#define UI_TEXTBOX_H

//======================================
// dependencies
#include "HUDObject.h"

//======================================
// the code

class TextBox :
	public HUDObject
{
	DWORD m_dwTextColor;
	std::wstring m_sContent; // string to be displayed

	RECT FontPosition; 
	LPD3DXFONT m_Font;
public:
	TextBox(std::wstring, int, int, DWORD, std::wstring);	//m_wsHOName, x, y, m_dwTextColor, m_sContent
	~TextBox(void);

	void drawElement();				//draws the textbox using m_Font
	void updateText(std::wstring);  //changes m_sContent to arguement
	void modifyColor(DWORD);		//changes the color of the text
	std::wstring getName();			//returns m_wsHOName
};

#endif // !UI_TEXTBOX_H
