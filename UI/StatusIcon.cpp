#include "StatusIcon.h"


StatusIcon::StatusIcon(std::wstring n, int x, int y, int i, SpriteStrip ss)
{
	m_wsHOName = n;
	m_iHOLocX = x;
	m_iHOLocY = y;
	m_iHOLocZ = 0;
	m_iImageID = i;

	destination.left = m_iHOLocX;
	destination.right = long(m_iHOLocX + (ss.SSgetImageWidth()));
	destination.top = m_iHOLocY;
	destination.bottom = m_iHOLocY + ss.SSgetImageHeight();

	source = ss.getSourceRect( m_iImageID, 1.0);
	void *pVertexBuffer = NULL;
	m_pVertexObject = NULL;

	m_Vertices[0].x = m_iHOLocX+(float)destination.left;  m_Vertices[0].y = m_iHOLocX+(float)destination.top;
	m_Vertices[1].x = m_iHOLocX+(float)destination.right; m_Vertices[1].y = m_iHOLocX+(float)destination.top;
	m_Vertices[2].x = m_iHOLocX+(float)destination.right; m_Vertices[2].y = m_iHOLocX+(float)destination.bottom;
	m_Vertices[3].x = m_iHOLocX+(float)destination.left;  m_Vertices[3].y = m_iHOLocX+(float)destination.bottom;
	m_Vertices[0].z = m_Vertices[1].z = m_Vertices[2].z = m_Vertices[3].z = 0;
	m_Vertices[0].rhw   = m_Vertices[1].rhw   = m_Vertices[2].rhw   = m_Vertices[3].rhw = 1.0f;
	m_Vertices[0].color = m_Vertices[1].color = m_Vertices[2].color = m_Vertices[3].color = NULL;

	g_pD3DDevice->CreateVertexBuffer(
		4*sizeof(CUSTOMVERTEX),       // tells DirectX the required size of the vertex buffer (in bytes).
		0,                            // specifies how the vertex buffer will be used – "0" being the default.
		D3DFVF_XYZRHW|D3DFVF_DIFFUSE, // tells DirectX about the memory layout of each vertex (the FVF format).
		D3DPOOL_DEFAULT,              // says that you don't care where memory is allocated.
		&m_pVertexObject,             // the address of a pointer to be filled with the vertex buffer object location.
		NULL);

	// a memory buffer (pointed to by pVertexBuffer) that we must copy our vertex data into
	m_pVertexObject->Lock(0, 4*sizeof(CUSTOMVERTEX), &pVertexBuffer, 0);		
	memcpy(pVertexBuffer, m_Vertices, 4*sizeof(CUSTOMVERTEX)); // copy the vertex
	m_pVertexObject->Unlock();
}


StatusIcon::~StatusIcon()
{
	if( m_pVertexObject != NULL)
	{
		m_pVertexObject->Release();
		m_pVertexObject = NULL;
	}
}

void StatusIcon::drawElement(SpriteStrip ss)
{
	// get the backbuffer
	g_pD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &g_pBackBuffer);

	// render the scene
	g_pD3DDevice->SetStreamSource(0, m_pVertexObject, 0, sizeof(CUSTOMVERTEX));
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	g_pD3DDevice->StretchRect(ss.m_surface, &source, g_pBackBuffer, &destination, D3DTEXF_NONE) ;
}

void StatusIcon::changeImage(int i, SpriteStrip ss)
{
	m_iImageID = i;
	source = ss.getSourceRect( m_iImageID, 1.0);
}

int StatusIcon::getID()
{
	return m_iImageID;
}

std::wstring StatusIcon::getName()
{
	return m_wsHOName;
}
