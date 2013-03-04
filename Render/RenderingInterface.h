#ifndef RENDERINGINTERFACE_H_
#define RENDERINGINTERFACE_H_

#include "EngineMain.h"

#pragma once

class RenderingInterface
{
public:
	RenderingInterface(void);
	~RenderingInterface(void);
/*******************************************************
* Position functions
*******************************************************/
	
	void		setPosition(Model model, D3DXVECTOR3 position);
	void		movePosition(Model model, D3DXVECTOR3 move);
	D3DXVECTOR3 getPosition(Model model);
	D3DXVECTOR3	convertVector(float x, float y);
	void		convertVector(D3DXVECTOR3 vec, double &x, double &y);
	void		convertMatrix(D3DXMATRIX mat);
	D3DXMATRIX	convertMatrix();

/*******************************************************
* Window functions
*******************************************************/

	void		setWindowSize(float width, float height);
	void		setFullScreen(bool enable);

/*******************************************************
* 
*******************************************************/

private:
	
	EngineMain* renderer;

};


#endif