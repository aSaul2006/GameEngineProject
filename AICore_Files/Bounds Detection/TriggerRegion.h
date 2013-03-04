/*****************************************************************
Program Name: TriggerRegion.h
Name: Zach Fetters
Description: This file defines the basic trigger region along with
			 the circular and rectangular square trigger regions
			 to allow them to be used when creating various triggers
*****************************************************************/

#include "Vector2D.h"
#include "Box2D.h"


class TriggerRegion
{

public:

	virtual ~TriggerRegion()
	{

	}

	//return true if the entity is overlapping the trigger region
	virtual bool IsTouching(Vector2D EntityPosition, double EntityRadius)const = 0;

};


class TriggerRegion_Circle : public TriggerRegion
{

private:

	//the center point for the trigger region
	Vector2D m_vPosition;

	//the radius of the trigger region
	double m_dRadius;

public:

	TriggerRegion_Circle(Vector2D Position, double Radius):m_dRadius(Radius), m_vPosition(Position)
	{

	}

	//return true if the entity is overlapping the trigger region
	bool IsTouching(Vector2D Position, double EntityRadius)const
	{

		return Vec2DDistanceSq(m_vPosition, Position) < (EntityRadius + m_dRadius)*(EntityRadius + m_dRadius);

	}


};


class TriggerRegion_Rectangle : public TriggerRegion
{

private:

	Box2D* m_pTrigger;

public:

	TriggerRegion_Rectangle(Vector2D TopLeft, Vector2D BottomRight)
	{

		//create a bounding box for the tirgger region
		m_pTrigger = new Box2D(TopLeft, BottomRight);

	}

	~TriggerRegion_Rectangle()
	{

		delete m_pTrigger;

	}

	//return true if the entity is overlapping the trigger region
	bool IsTouching(Vector2D Position, double EntityRadius)const
	{

		Box2D Box(Vector2D(Position.x - EntityRadius, Position.y - EntityRadius),
				  Vector2D(Position.x + EntityRadius, Position.y + EntityRadius));

		return Box.IsOverlapping(*m_pTrigger);

	}

};