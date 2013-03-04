/*****************************************************************
Program Name: Geometry.h
Name: Zach Fetters
Description: This file is used to hold various geometry functions
			 that may need to be performed in various places
*****************************************************************/

#include "Vector2D.h"

inline double DistToLineSegment(Vector2D v1, Vector2D v2, Vector2D vPoint)
{

	//calculate and add the angles between vPoint & v1, and vPoint & v2
	double m_dV1 = (vPoint.x - v1.x) * (v2.x - v1.x) + (vPoint.y - v1.y) * (v2.y - v1.y);

	//if both angles are obtuse then v1 is closest to vPoint
	if(m_dV1 <= 0)
	{

		return Vec2DDistance(v1, vPoint);

	}

	//calculate and add the angles between vPoint & v2, and vPoint & v2
	double m_dV2 = (vPoint.x - v2.x) * (v1.x - v2.x) + (vPoint.y - v2.y) * (v1.y - v2.y);

	//if both angles are obtuse then v2 is closest to vPoint
	if(m_dV2 <= 0)
	{

		return Vec2DDistance(v2, vPoint);

	}

	//calculate the point on the line between v1 and v2 which is closest to vPoint
	Vector2D ClosestPoint = v1 + ((v2 - v1) * m_dV1) / (m_dV1 + m_dV2);

	return Vec2DDistance(vPoint, ClosestPoint);

}