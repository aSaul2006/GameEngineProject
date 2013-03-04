#include "TextBox.h"


TextBox::TextBox(std::wstring n, int x, int y, DWORD co, std::wstring c )
{
	m_wsHOName = n;
	m_iHOLocX = x;
	m_iHOLocY = y;
	m_iHOLocZ = 0;
	m_dwTextColor = co;
	m_sContent = c;

	D3DXCreateFont(g_pD3DDevice,     // the D3D Device
		18,                          // font height
		0,                           // default font width
		FW_NORMAL,                   // font weight
		1,                           // not using MipLevels
		false,                       // italic font
		DEFAULT_CHARSET,             // default character set
		OUT_DEFAULT_PRECIS,          // default OutputPrecision,
		DEFAULT_QUALITY,             // default Quality
		DEFAULT_PITCH | FF_DONTCARE, // default pitch and family
		L"Arial",                    // use Facename Arial
		&m_Font
		);
}

TextBox::~TextBox() {}

void TextBox::drawElement()
{
	const wchar_t *a = m_sContent.c_str();

	FontPosition.top = m_iHOLocY;
	FontPosition.left = m_iHOLocX;
	FontPosition.right = m_iHOLocX+((int)wcslen(a)* 9);
	FontPosition.bottom = m_iHOLocY + 64;	

	m_Font->DrawTextW(NULL, a, (int)wcslen(a), &FontPosition, DT_CENTER, m_dwTextColor); //draw text
}

void TextBox::updateText(std::wstring c)
{
	m_sContent = c;
}

void TextBox::modifyColor(DWORD c)
{
	m_dwTextColor = c;
}

std::wstring TextBox::getName()
{
	return m_wsHOName;
}
