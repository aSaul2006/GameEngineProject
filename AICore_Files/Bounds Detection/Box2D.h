/********************************************************
Program Name: Box2D.h
Name: Zach Fetters
Description: This file declares the variables and functions
			 to create a bounding box for a trigger
********************************************************/

#include "Vector2D.h"

class Box2D
{

private:

	//vector for the top left corner of the box
	Vector2D m_vTopLeft;

	//vector for the bottom right corner of the box
	Vector2D m_vBottomRight;

	//vector for the center of the box
	Vector2D m_vCenter;

public:

	Box2D(Vector2D TopLeft,
		  Vector2D BottomRight):m_vTopLeft(TopLeft),
								m_vBottomRight(BottomRight),
								m_vCenter((TopLeft + BottomRight) / 2.0)
	{

	}

	//return true if the given bounding box intersects with this one
	bool isOverlapping(const Box2D& CheckBox)const
	{

		return !((CheckBox.Top() > this->Bottom()) || 
				(CheckBox.Bottom() < this->Top()) ||
				(CheckBox.Left() > this->Right()) ||
				(CheckBox.Right() < this->Left()));

	}

	//return the top left point of the box
	Vector2D TopLeft()const
	{

		return m_vTopLeft;

	}

	//return the bottom right point of the box
	Vector2D BottomRight()const
	{

		return m_vBottomRight;

	}

	//return y value of top left point
	double Top()const
	{

		return m_vTopLeft.y;

	}

	//return x value of top left point
	double Left()const
	{

		return m_vTopLeft.x;

	}

	//return y value of bottom right point
	double Bottom()const
	{

		return m_vBottomRight.y;

	}

	//return x value of bottom right point
	double Right()const
	{

		return m_vBottomRight.x;

	}

	//return the center point of the box
	Vector2D Center()const
	{

		return m_vCenter;

	}


};