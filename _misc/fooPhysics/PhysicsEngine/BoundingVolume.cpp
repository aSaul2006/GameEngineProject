/***************************************************************************************
*  Program Name:    BoundingVolume.cpp                                                 *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the rough collision detection system.  It is    *
*                   used to return pairs of objects that may be in contact, which can  *
*                   then be tested using more detailed methods found in CollisionDetect.                        *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
// BoundingVolume.cpp
/*
 * Implementation file for the rough first step collision detector.
 */


#include "physicsengine\BoundingVolume.h"

using namespace PhysicsEngine;

BoundingSphere::BoundingSphere(const Vector3D &v3SphereCenter, double dSphereRadius)
{
    BoundingSphere::m_v3SphereCenter = v3SphereCenter;
    BoundingSphere::m_dSphereRadius = dSphereRadius;
}

BoundingSphere::BoundingSphere(const BoundingSphere &one,
                               const BoundingSphere &two)
{
    Vector3D v3SphereCenterOffset = two.m_v3SphereCenter - one.m_v3SphereCenter;
    double dDistance = v3SphereCenterOffset.squareOfTheMagnitude();
    double radiusDiff = two.m_dSphereRadius - one.m_dSphereRadius;

    // Check if the larger sphere encloses the small one
    if (radiusDiff*radiusDiff >= dDistance)
    {
        if (one.m_dSphereRadius > two.m_dSphereRadius)
        {
            m_v3SphereCenter = one.m_v3SphereCenter;
            m_dSphereRadius = one.m_dSphereRadius;
        }
        else
        {
            m_v3SphereCenter = two.m_v3SphereCenter;
            m_dSphereRadius = two.m_dSphereRadius;
        }
    }

    // Otherwise we need to work with partially
    // overlapping spheres
    else
    {
        dDistance = sqrt(dDistance);
        m_dSphereRadius = (dDistance + one.m_dSphereRadius + two.m_dSphereRadius) * 
			((double)0.5);

        // The new centre is based on one's centre, moved towards
        // two's centre by an ammount proportional to the spheres'
        // radius.
        m_v3SphereCenter = one.m_v3SphereCenter;
        if (dDistance > 0)
        {
            m_v3SphereCenter += v3SphereCenterOffset * 
				((m_dSphereRadius - one.m_dSphereRadius)/dDistance);
        }
    }

}

bool BoundingSphere::overlaps(const BoundingSphere *other) const
{
    double distanceSquared = (m_v3SphereCenter - other->m_v3SphereCenter).squareOfTheMagnitude();
    return distanceSquared < (m_dSphereRadius+other->m_dSphereRadius)*
		(m_dSphereRadius+other->m_dSphereRadius);
}

double BoundingSphere::getGrowth(const BoundingSphere &other) const
{
    BoundingSphere newSphere(*this, other);

    // We return a value proportional to the change in surface
    // area of the sphere.
    return newSphere.m_dSphereRadius*newSphere.m_dSphereRadius - m_dSphereRadius*m_dSphereRadius;
}