/************************************************************************
*  Program Name: Lighting.h                                             
*  Name: GSP420 Rendering Core - Jesse Goldan, Mike Murrell                                                        *
*  Date: January 28th - February 1st
*  Description: A class that lets you do all functions that a camera would do
*  Created by Jesse and slightly modified by Mike
*  Update: February 8, 2013
************************************************************************/
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <d3dx9.h>

#pragma once
//Camera class
class Camera
{
public:
	Camera();
	void SetPosition(D3DXVECTOR3& position);
	void SetRotation(D3DXVECTOR3& rotation);
	void SetTarget (D3DXVECTOR3& target);
	const D3DXVECTOR3& GetPosition() const;
	const D3DXVECTOR3& GetRotation() const;
	const D3DXVECTOR3& GetTarget() const;
	D3DXMATRIX& GetProjection();
	const D3DXMATRIX& GetView() const;
	void Move (D3DXVECTOR3& move);
	void Rotate (D3DXVECTOR3& rotate);
	void SetFOV(float fov);
	float GetFOV() const;
	void SetAspectRatio(float aspectRatio);
	float GetAspectRatio() const;
	void SetNearValue(float nearValue);
	float GetNearValue() const;
	void SetFarValue(float farValue);
	float GetFarValue() const;
	virtual void Render();

private:
	float m_FOV;
	float m_NearValue;
	float m_FarValue;
	float m_AspectRatio;
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation; 
	D3DXVECTOR3 m_Target;
	D3DXMATRIX m_m4Proj;
	D3DXMATRIX m_m4View;
};
#endif 