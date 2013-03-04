/***************************************************************************************
*  Program Name:    joints.cpp                                                         *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Interface file for joints implementation between Physics bodies.   *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Implementation file for joints.
 * 
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */
#include "physicsengine\PhysicsEngine.h"

using namespace PhysicsEngine;

unsigned Joint::addContact(Contact *contact, unsigned limit) const
{
    // Calculate the position of each connection point in world coordinates
    Vector3D a_pos_world = body[0]->getPointInWorldSpace(position[0]);
    Vector3D b_pos_world = body[1]->getPointInWorldSpace(position[1]);

    // Calculate the length of the joint
    Vector3D a_to_b = b_pos_world - a_pos_world;
    Vector3D normal = a_to_b;
    normal.normalize();
    double length = a_to_b.magnitude();

    // Check if it is violated
    if (fabs(length) > error)
    {
        contact->body[0] = body[0];
        contact->body[1] = body[1];
        contact->m_v3NormalofContactWorld = normal;
        contact->m_v3PointOfContactWorld = (a_pos_world + b_pos_world) * 0.5f;
        contact->m_dDepthofContact = length-error;
        contact->m_dFriction = 1.0f;
        contact->m_dRestitution = 0;
        return 1;
    }

    return 0;
}

void Joint::set(PhysBody *a, const Vector3D& a_pos,
                PhysBody *b, const Vector3D& b_pos,
                double error)
{
    body[0] = a;
    body[1] = b;

    position[0] = a_pos;
    position[1] = b_pos;

    Joint::error = error;
}