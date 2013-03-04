/************************************************************************
*  Program Name:		Path.cpp										*
*  Name:				James B. Strickler II							*
*  Date:				January 25, 2013								*
*  Description:			Pathing class used to define, manage,  			*
*						and traverse a path  							*
*						(defined by a series of 2D vectors)				*
*  Update:																*
*																		*
************************************************************************/
#include "Path.h"
#include "misc/utils.h"
#include "misc/Cgdi.h"
#include "2d/transformations.h"

//adds a waypoint to the end of the path
void Path::AddWayPoint(Vector2D new_point) {
      m_WayPoints.push_back(new_point);
}


std::list<Vector2D> Path::CreateRandomPath(int   NumWaypoints,
                                           double MinX,
                                           double MinY,
                                           double MaxX,
                                           double MaxY)
{
    m_WayPoints.clear();

    double midX = (MaxX+MinX)/2.0;
    double midY = (MaxY+MinY)/2.0;

    double smaller = min(midX, midY);

    double spacing = TwoPi/(double)NumWaypoints;

    for (int i=0; i<NumWaypoints; ++i)
    {
      double RadialDist = RandInRange(smaller*0.2f, smaller);

      Vector2D temp(RadialDist, 0.0f);

      Vec2DRotateAroundOrigin(temp, i*spacing);

      temp.x += midX; temp.y += midY;
      
      m_WayPoints.push_back(temp);
                            
    }

    curWaypoint = m_WayPoints.begin();

    return m_WayPoints;
}


void Path::Render()const
{
  gdi->OrangePen();

  std::list<Vector2D>::const_iterator it = m_WayPoints.begin();

  Vector2D wp = *it++;

  while (it != m_WayPoints.end())
  {
    gdi->Line(wp, *it);

    wp = *it++;
  }

  if (m_bLooped) gdi->Line(*(--it), *m_WayPoints.begin());
}
