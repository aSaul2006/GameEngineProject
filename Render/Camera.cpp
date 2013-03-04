/************************************************************************
*  Program Name: Lighting.h                                             
*  Name: GSP420 Rendering Core - Jesse Goldan, Mike Murrell                                                        *
*  Date: January 28th - February 1st
*  Description: A class that lets you do all functions that a camera would do
*  Created by Jesse and slightly modified by Mike
*  Update: February 8, 2013
************************************************************************/

#include "Camera.h"

Camera::Camera(): m_FOV(70), m_NearValue(0.1f), m_FarValue(1000), m_AspectRatio(4.0f/3.0f), m_Target()
{
}

void Camera::SetPosition (D3DXVECTOR3& position)
{
	m_Position = position;
}

const D3DXVECTOR3& Camera::GetPosition() const
{
	return m_Position;
}

void Camera::Move (D3DXVECTOR3& move)
{
	m_Position += move;
	m_Target += move;
}

void Camera::SetRotation (D3DXVECTOR3& rotation)
{
	m_Rotation = rotation;
}

const D3DXVECTOR3& Camera::GetRotation() const
{
	return m_Rotation;
}

D3DXMATRIX& Camera::GetProjection()
{
	return m_m4Proj;
}
const D3DXMATRIX& Camera::GetView() const
{
	return m_m4View;
}

void Camera::Rotate (D3DXVECTOR3& rotate)
{
	m_Rotation += rotate;
}

void Camera::SetFOV (float fov)
{
	m_FOV = fov;
}

float Camera::GetFOV() const
{
	return m_FOV;
}

void Camera::SetNearValue (float nearValue)
{
	m_NearValue = nearValue;
}

float Camera::GetNearValue() const
{
	return m_NearValue;
}

void Camera::SetFarValue (float farValue)
{
	m_FarValue = farValue;
}

float Camera::GetFarValue() const
{
	return m_FarValue;
}

void Camera::SetAspectRatio (float aspectRatio)
{
	m_AspectRatio = aspectRatio;
}

float Camera::GetAspectRatio() const
{
	return m_AspectRatio;
}

void Camera::SetTarget (D3DXVECTOR3& target)
{
	m_Target = target;
}

const D3DXVECTOR3& Camera::GetTarget() const 
{
	return m_Target;
}

void Camera::Render()
{
	Camera();
	D3DXMatrixLookAtLH(&m_m4View, &m_Position, &m_Target, &m_Rotation);
	m_Rotation.x, 1, 0, 0;
	m_Rotation.y, 0, 1, 0; 
	m_Rotation.z, 0, 0, 1;
}