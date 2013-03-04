/************************************************************************
*  Program Name:         Texture Resource Class                                               
*  Name:                 Juan Ramirez                                                                                        
*  Date:                 January 25, 2013  
*  Description:          Loads texture variables                                                                                                                                                                   *
*  Update:                                            
*                                                     
************************************************************************/

#pragma once
#include "resourcemanager.h"
#include <d3d9.h>
#include <d3dx9.h>

//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Texture Resource Interface class----------------------------
//===============================================================================
class cTextureResource :
	public cResource
{
private:
	IDirect3DTexture9* p_Texture;
	LPDIRECT3DDEVICE9  p_Device;	// This is a temp device used to create the texture

	/* 
	This function turns the string variable "m_FileName" into an
	wstring. Then it converts wstring into LPCWSTR, the variable type
	needed to successfully create the texture variable 
	*/
	LPCWSTR StringToLPCWSTR(std::string s_Variable);
public:
	inline cTextureResource(void)
	{
		m_ResourceID = m_Scope = 0;
		m_Type = RESOURCE_TEXTURE;

		p_Texture = NULL;
		p_Device = NULL;
	}

	// Overloaded functions
	~cTextureResource(void);
	void load();
	void unload();

	// Get texture variable
	LPDIRECT3DTEXTURE9 GetTexture(void) {return p_Texture;}

	// Set D3DX Device variable
	// This variable is a temp variable that is only used to create
	// the texture. Needs to be loaded in from the Rendering Core's 
	// D3DX Device
	void SetDevice(LPDIRECT3DDEVICE9 D3DXDevice) {p_Device = D3DXDevice;}
};

