/************************************************************************
*  Program Name: Model.cpp                                             
*  Name: GSP420 Rendering Core - Mike Murrell                                                        *
*  Date: January 28th - February 1st
*  Description: Mainly for the rendering of the engine
*  Update: February 8, 2013
************************************************************************/

#include "Model.h"

/******************************************************
* Loads the .X model into the m_model variable
* stores the materials and textures in the 
* variables as well
*******************************************************/

Model::Model(const std::wstring& modelFileName)
{
	LoadXFile(modelFileName.c_str(), &m_Model, m_ModelMtrl, m_ModelTex);
	defaultTex = 0;
}
Model::Model(const std::wstring& modelFileName, const std::wstring& textureFileName)
{
	LoadXFile(modelFileName.c_str(), &m_Model, m_ModelMtrl, m_ModelTex);

	HR(D3DXCreateTextureFromFile(g_d3dDevice, textureFileName.c_str(), &defaultTex));
}


Model::~Model(void)
{
	ReleaseCOM(m_Model);
	for(unsigned int i = 0; i < m_ModelTex.size(); ++i)
		ReleaseCOM(m_ModelTex[i]);
	if (defaultTex != 0)
		ReleaseCOM(defaultTex);
}

/******************************************************
* Sets the position and rotation of the model
*******************************************************/
void Model::InitModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float deg)
{
	position = pos;
	rotation = rot;
	theta = deg;
}

/******************************************************
* Moves the model around in a 2d plane
*******************************************************/
void Model::Move(float x, float y)
{
	position.x += x;
	position.y += y;
}

/******************************************************
* Rotates the model
*******************************************************/
void Model::Rotate(float deg)
{
	theta	 += deg;
}

void Model::Scale(float scale)
{
	this->scale = scale;
}

/******************************************************
* Draws the Model
* Interfaces with the shaders

* Error: Doesn't load the textures
* I'm thinking it has something to do with pointers
* I had the same problem with materials
* But fixed it by sending the address instead
* Tried the same technique with textures but
* am still having problems with it 
*******************************************************/
void Model::DrawModel(D3DXMATRIX wVPM)
{
	D3DXMATRIX R, T, toWorld;
	D3DXMatrixScaling(&toWorld, scale, scale, scale);
	D3DXMatrixRotationAxis(&R, &rotation, theta);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	toWorld *= R*T;

	Shaders::BasicFX->SetWorldViewProj(toWorld*wVPM);
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &toWorld);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	Shaders::BasicFX->SetWorldInvTrans(worldInvTrans);
	Shaders::BasicFX->SetWorld(toWorld);

	// Begin passes.
	for(UINT j = 0; j < m_ModelMtrl.size(); ++j)
	{
		Shaders::BasicFX->SetMaterial(&m_ModelMtrl[j]);
		
		if(m_ModelTex[j] !=0)
		{
			Shaders::BasicFX->SetTexture(m_ModelTex[j]);
			//HR(Shaders::BasicFX->m_FX->SetTexture(Shaders::BasicFX->m_hTex, m_ModelTex[j]));
		}
		else
		{
			Shaders::BasicFX->SetTexture(defaultTex);
		}

		Shaders::BasicFX->m_FX->CommitChanges();
		HR(m_Model->DrawSubset(j));
	}
}