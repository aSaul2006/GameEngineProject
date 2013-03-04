/************************************************************************
*  Program Name: Shaders.h                                              
*  Name: GSP420 Rendering Core - Mike Murrell                                                        *
*  Date: January 28th - February 1st
*  Description: A singleton design that declares a global directional light shader
*  Inspired by Frank Lunas design on his intro to directx11 book
*  Update: February 8, 2013
*
************************************************************************/


#ifndef Shaders_H
#define Shaders_H

#include "d3dUtil.h"
#include "Lighting.h"

#pragma once

#pragma region Shader
class Shader
{
public:
	Shader(const std::wstring& filename);
	virtual ~Shader();
	ID3DXEffect* m_FX;
private:
	Shader(const Shader& rhs);
	Shader& operator=(const Shader& rhs);
};
#pragma endregion

class Basic : public Shader
{
public:
	Basic(const std::wstring& filename);
	 ~Basic(void);
	
	/******************************************************
	* Sets up the functions that will let you interface 
	* with the shader features
	*******************************************************/

	void SetWorld(D3DXMATRIX M)							{HR(m_FX->SetMatrix(m_hWorld, &M));}
	void SetWorldInvTrans(D3DXMATRIX M)					{HR(m_FX->SetMatrix(m_hWorldInvTrans, &M));}
	void SetWorldViewProj(D3DXMATRIX M)					{HR(m_FX->SetMatrix(m_hWVP, &M)); }
	void SetEyePosition(const D3DXVECTOR3 ep)			{HR(m_FX->SetValue(m_hEyePos, &ep, sizeof(D3DXVECTOR3)));}
	void SetMaterial(Mtrl *mm)							{HR(m_FX->SetValue(m_hMtrl, &mm, sizeof(Mtrl)));}
	void SetTexture(IDirect3DTexture9* mt)				{HR(m_FX->SetTexture(m_hTex, mt));}
	void SetDirectionalLight(DirectionalLight* dl)		{HR(m_FX->SetValue(m_hDirLight, &dl, sizeof(DirectionalLight)));}

	D3DXHANDLE   m_hTech;					//Technique
	D3DXHANDLE   m_hWorld;					//World Matrix
	D3DXHANDLE   m_hWorldInvTrans;			//World Inverse Matrix
	D3DXHANDLE   m_hWVP;					//World View Projection
	D3DXHANDLE   m_hEyePos;					//Eye Projection
	D3DXHANDLE	 m_hMtrl;					//Material Handle
	D3DXHANDLE	 m_hTex;					//Texture Handle
	D3DXHANDLE	 m_hDirLight;				//Directional Light Handle
//	D3DXHANDLE	 m_hPointLight;				//Point Light Handle
//	D3DXHANDLE	 m_hSpotLight;				//Spot Light Handle
};

class VertexBlend : public Shader
{
public:

	VertexBlend(const std::wstring& filename);
	~VertexBlend(void);

	/******************************************************
	* Sets up the functions that will let you interface 
	* with the shader features
	*******************************************************/

	D3DXHANDLE   m_hTech;
	D3DXHANDLE   m_hWorld;
	D3DXHANDLE   m_hWorldInvTrans;
	D3DXHANDLE   m_hWVP;
	D3DXHANDLE   m_hEyePos;	
	D3DXHANDLE   m_hTex;
	D3DXHANDLE   m_hMtrl;
	D3DXHANDLE   m_hLight;
	D3DXHANDLE   m_hFinalXForms;

	void SetWorld(D3DXMATRIX M)							{HR(m_FX->SetMatrix(m_hWorld, &M));}
	void SetWorldInvTrans(D3DXMATRIX M)					{HR(m_FX->SetMatrix(m_hWorldInvTrans, &M));}
	void SetWorldViewProj(D3DXMATRIX M)					{HR(m_FX->SetMatrix(m_hWVP, &M)); }
	void SetEyePosition(const D3DXVECTOR3 ep)			{HR(m_FX->SetValue(m_hEyePos, &ep, sizeof(D3DXVECTOR3)));}
	void SetMaterial(Mtrl mm)							{HR(m_FX->SetValue(m_hMtrl, &mm, sizeof(Mtrl)));}
	void SetTexture(IDirect3DTexture9* mt)				{HR(m_FX->SetTexture(m_hTex, mt));}
	void SetDirectionalLight(DirectionalLight* dl)		{HR(m_FX->SetValue(m_hLight, &dl, sizeof(DirectionalLight)));}
	//void SetFinalXForms(AnimatedModel m_SkinnedMesh)	{HR(m_FX->SetValue(m_hFinalXForms, m_SkinnedMesh->getFinalXFormArray(), m_SkinnedMesh->numBones()));

};

/******************************************************
* Class for declaring global shaders
*******************************************************/

#pragma region Shaders
class Shaders
{
public:
	static void InitAll();
	static void DestroyAll();

	static Basic* BasicFX;
	static VertexBlend* VBlendFX;
};
#pragma endregion


#endif