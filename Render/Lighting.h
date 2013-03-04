/************************************************************************
*  Program Name: Lighting.h                                             
*  Name: GSP420 Rendering Core - Mike Murrell                                                        *
*  Date: January 28th - February 1st
*  Description: A light class that holds the structs for the lights
*  Inspired by Frank Lunas design on his intro to directx11 book
*  Update: February 8, 2013
************************************************************************/

#ifndef LIGHTING_H
#define LIGHTING_H

#include <Windows.h>
#include <d3dx9math.h>

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	D3DXCOLOR Ambient;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Specular;
	D3DXVECTOR3 Direction;
};

struct PointLight
{
	PointLight() {ZeroMemory(this, sizeof(this)); }

	D3DXCOLOR Ambient;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Specular;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Attenuation;
	float Range;
};

struct SpotLight
{
	D3DXCOLOR Ambient;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Specular;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Attenuation;
	D3DXVECTOR3 Direction;
	float Range;
	float Spot;
};

class Lighting
{
public:
	Lighting(void);
	~Lighting(void);
};

#endif