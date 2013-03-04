/************************************************************************
*  Program Name:         Texture Resource Class                                               
*  Name:                 Juan Ramirez                                                                                        
*  Date:                 January 25, 2013  
*  Description:          Loads texture variables                                                                                                                                                                   *
*  Update:                                            
*                                                     
************************************************************************/
#include "cTextureResource.h"

cTextureResource::~cTextureResource(void)
{
	if(p_Texture)
	{
		p_Texture->Release();
		p_Texture = NULL;
	}
}

void cTextureResource::load()
{
	D3DXCreateTextureFromFileA(p_Device,m_FileName.c_str(),
		&p_Texture);
}
	
void cTextureResource::unload()
{
	if(p_Texture)
	{
		p_Texture->Release();
		p_Texture = NULL;
	}
}

LPCWSTR cTextureResource::StringToLPCWSTR(std::string s_Variable)
{
	std::wstring ws_Temp = std::wstring(s_Variable.begin(), s_Variable.end());
	return ws_Temp.c_str();
}