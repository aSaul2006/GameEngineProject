/************************************************************************
*  Program Name: Shaders.cpp                                             
*  Name: GSP420 Rendering Core - Mike Murrell                                                        *
*  Date: January 28th - February 1st
*  Description: A singleton design that declars a global directional light shader
*  Inspired by Frank Lunas design on his intro to directx11 book
*  Update: February 8, 2013
************************************************************************/

#include "Shaders.h"

/******************************************************
* Loads the .fx file and puts it in the ID3DXEffect
*******************************************************/

Shader::Shader(const std::wstring& filename): m_FX(0)
{
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(g_d3dDevice, filename.c_str(), 
		0, 0, D3DXSHADER_DEBUG, 0, &m_FX, &errors));
	if (errors)
		MessageBox(0, (wchar_t*)errors->GetBufferPointer(), 0, 0);
}


Shader::~Shader()
{
	ReleaseCOM(m_FX);
}

/******************************************************
* Sets the handles between the C++ and .fx variables
*******************************************************/
Basic::Basic(const std::wstring& filename) : Shader(filename)
{
	m_hTech           = m_FX->GetTechniqueByName("DirLightTech"); 
	m_hWorld          = m_FX->GetParameterByName(0, "gWorld");
	m_hWVP            = m_FX->GetParameterByName(0, "gWVP");
	m_hWorldInvTrans  = m_FX->GetParameterByName(0, "gWorldInvTrans");
	m_hEyePos         = m_FX->GetParameterByName(0, "gEyePosW");
	m_hMtrl			  = m_FX->GetParameterByName(0, "gMtrl");
	m_hTex			  = m_FX->GetParameterByName(0, "gTex");
	m_hDirLight		  = m_FX->GetParameterByName(0, "gDirLight");
}

VertexBlend::VertexBlend(const std::wstring& filename) : Shader(filename)
{
	m_hTech           = m_FX->GetTechniqueByName("VBlend2Tech"); 
	m_hWorld          = m_FX->GetParameterByName(0, "gWorld");
	m_hWVP            = m_FX->GetParameterByName(0, "gWVP");
	m_hWorldInvTrans  = m_FX->GetParameterByName(0, "gWorldInvTrans");
	m_hEyePos         = m_FX->GetParameterByName(0, "gEyePosW");
	m_hMtrl			  = m_FX->GetParameterByName(0, "gMtrl");
	m_hTex			  = m_FX->GetParameterByName(0, "gTex");
	m_hLight		  = m_FX->GetParameterByName(0, "gLight");
	m_hFinalXForms	  = m_FX->GetParameterByName(0, "gFinalXForms");
}

/******************************************************
* Destructors
*******************************************************/
Basic::~Basic(){}
VertexBlend::~VertexBlend(){}
/******************************************************
* Initializes the global shader you'll interface with
* Loads the fx file into it
*******************************************************/
Basic*		 Shaders::BasicFX = 0;
VertexBlend* Shaders::VBlendFX = 0;

void Shaders::InitAll()
{
	BasicFX  = new Basic(L"../Render/fx/lighting.fx");
	VBlendFX = new VertexBlend(L"../Render/fx/vblend2.fx");
}

void Shaders::DestroyAll()
{
	delete(BasicFX);
	delete(VBlendFX);
}