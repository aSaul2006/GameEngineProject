#include "RenderingInterface.h"


RenderingInterface::RenderingInterface(void)
{
	
}


RenderingInterface::~RenderingInterface(void)
{
}

/*******************************************************
* Position functions
*******************************************************/
	
void RenderingInterface::setPosition(Model model, D3DXVECTOR3 position)
{
	//model.setPosition(position.x, position.y);
}
void RenderingInterface::movePosition(Model model, D3DXVECTOR3 move)
{
	//model.Move(move.x, move.y);
}
//D3DXVECTOR3 RenderingInterface::getPosition(Model model)
//{
	//return model.getPosition();
//}
D3DXVECTOR3	RenderingInterface::convertVector(float x, float y)
{
	return D3DXVECTOR3(x, y, 1.0f);
}

void RenderingInterface::convertVector(D3DXVECTOR3 vec, double &x, double &y)
{
	x = double(vec.x);
	y = double(vec.y);
}

D3DXMATRIX	RenderingInterface::convertMatrix()
{
	D3DXMATRIX x;
	return x;
}

void RenderingInterface::convertMatrix(D3DXMATRIX mat)
{
}

/*******************************************************
* Window functions
*******************************************************/

void RenderingInterface::setWindowSize(float width, float height)
{
	//renderer->ChangeWindow(width, height);
}
void setFullScreen(bool enable)
{
	//renderer->EnableFullScreen(enable);
}

