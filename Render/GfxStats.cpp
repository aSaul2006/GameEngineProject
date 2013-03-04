//=============================================================================
// GfxStats.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "d3dUtil.h"
#include "d3dApp.h"
#include "GfxStats.h"
#include <tchar.h>

GfxStats::GfxStats()
: m_Font(0), m_FPS(0.0f), m_MilliSecPerFrame(0.0f), m_NumTris(0), m_NumVertices(0)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 18;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    _tcscpy_s(fontDesc.FaceName, _T("Times New Roman"));

	HR(D3DXCreateFontIndirect(g_d3dDevice, &fontDesc, &m_Font));
}

GfxStats::~GfxStats()
{
	ReleaseCOM(m_Font);
}

void GfxStats::onLostDevice()
{
	HR(m_Font->OnLostDevice());
}

void GfxStats::onResetDevice()
{
	HR(m_Font->OnResetDevice());
}

void GfxStats::addVertices(DWORD n)   {	m_NumVertices += n;}
void GfxStats::subVertices(DWORD n)   {	m_NumVertices -= n;}
void GfxStats::addTriangles(DWORD n)  {	m_NumTris += n;    }
void GfxStats::subTriangles(DWORD n)  {	m_NumTris -= n;    }
void GfxStats::setTriCount(DWORD n)   {	m_NumTris = n;     }
void GfxStats::setVertexCount(DWORD n){	m_NumVertices = n; }

void GfxStats::update(float dt)
{
	// Make static so that their values persist accross function calls.
	static float numFrames   = 0.0f;
	static float timeElapsed = 0.0f;

	// Increment the frame count.
	numFrames += 1.0f;

	// Accumulate how much time has passed.
	timeElapsed += dt;

	// Has one second passed?--we compute the frame statistics once 
	// per second.  Note that the time between frames can vary so 
	// these stats are averages over a second.
	if( timeElapsed >= 1.0f )
	{
		// Frames Per Second = numFrames / timeElapsed,
		// but timeElapsed approx. equals 1.0, so 
		// frames per second = numFrames.

		m_FPS = numFrames;

		// Average time, in miliseconds, it took to render a single frame.
		m_MilliSecPerFrame = 1000.0f / m_FPS;

		// Reset time counter and frame count to prepare for computing
		// the average stats over the next second.
		timeElapsed = 0.0f;
		numFrames   = 0.0f;
	}
}

void GfxStats::display()
{
	// Make static so memory is not allocated every frame.
	static wchar_t buffer[256];

	swprintf_s(buffer, L"Frames Per Second = %.2f\n"
		L"Milliseconds Per Frame = %.4f\n"
		L"Triangle Count = %d\n"
		L"Vertex Count = %d", m_FPS, m_MilliSecPerFrame, m_NumTris, m_NumVertices);
	
	RECT R = {5, 5, 0, 0};
	HR(m_Font->DrawText(0, buffer, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(0,0,0)));
}