/************************************************************************
*  Program Name: Model.h                                              
*  Name: GSP420 Rendering Core - Mike Murrell                                                        *
*  Date: January 28th - February 1st
*  Description: Allows you to import and translate/rotate a .X Model
*  Update: February 8, 2013
************************************************************************/
#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "d3dUtil.h"
#include "d3dApp.h"
#include "Shaders.h"


class Model
{
public:
	
	ID3DXMesh* m_Model;
	std::vector<Mtrl> m_ModelMtrl;
	std::vector<IDirect3DTexture9*> m_ModelTex;
	IDirect3DTexture9* defaultTex;

	Model(const std::wstring& modelFileName);
	Model(const std::wstring& modelFileName, const std::wstring& textureFileName);
	~Model(void);

	void DrawModel(D3DXMATRIX wVPM);
	void InitModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float deg);
	void Move(float x, float y);
	void Rotate(float theta);
	void Scale(float scale);
	D3DXVECTOR3 getPosition() {return position;}
	
protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	float scale;
	float theta; 
};

#endif