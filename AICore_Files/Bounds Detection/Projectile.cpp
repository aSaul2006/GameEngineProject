/*****************************************************************
Program Name: Projectile.cpp
Name: Zach Fetters
Description: This file is used for implementing functions for the
			  basic projectile class
*****************************************************************/

#include "Projectile.h"
#include "Game.h" //include the main game class
#include "Geometry.h"
#include "Constants.h"
#include <list>

Entity* Projectile::GetClosestIntersectingEntity(Vector2D From, Vector2D To)const
{

	Entity* ClosestIntersectingEntity = 0;
	double ClosestSoFar = MaxDouble;

	//iterate through all game entities
	std::list<Entity*>::const_iterator curEntity;
	for(curEntity = m_pWorld->GetAllEntities().begin();
		curEntity != m_pWorld->GetAllEntities().end();
		++curEntity)
	{

		//don't check the entity who shot the projectile
		if(((*curEntity)->ID() != m_iShooterID))
		{

			//if the distasnce to the segment between From and To is less than the entities bounding
			//radius then thre is an intersection
			if(DistToLineSegment(From, To, (*curEntity)->Pos()) < (*curEntity)->BRadius())
			{

				//check if this the closest intersecting entity so far
				double Dist = Vec2DDistanceSq((*curEntity)->Pos(), m_vOrigin);

				//if so then set this to be the new closest entity
				if(Dist < ClosestSoFar)
				{

					Dist = ClosestSoFar;
					ClosestIntersectingEntity = *curEntity;

				}

			}

		}

	}

	return ClosestIntersectingEntity;

}


std::list<Entity*> Projectile::GetListOfIntersectingEntities(Vector2D From, Vector2D To)const
{

	//list to hold all entites who are intersecting with the given line segment
	std::list<Entity*> hits;

	//iterate through all game entites
	std::list<Entity*>::const_iterator curEntity;
	for(curEntity = m_pWorld->GetAllEntities.begin();curEntity != m_pWorld->GetAllEntities.end();++curEntity)
	{

		//don't check the entity who shot the projectile
		if(((*curEntity)->ID() != m_iShooterID))
		{

			//if the distasnce to the segment between From and To is less than the entities bounding
			//radius then add this entity the the hit list
			if(DistToLineSegment(From, To, (*curEntity)->Pos()) < (*curEntity)->BRadius())
			{

				hits.push_back(*curEntity);
			}

		}

	}

	return hits;

}