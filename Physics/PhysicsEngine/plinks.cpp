/***************************************************************************************
*  Program Name:    plinks.cpp                                                         *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Implementation file for the particle links                         *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Implementation file for particle links.
 *
 * Part of the Cyclone physics system.
 */

#include "physicsengine\plinks.h"

using namespace PhysicsEngine;

double ParticleLink::currentLength() const
{
    Vector3D relativePos = particle[0]->getPosition() -
                          particle[1]->getPosition();
    return relativePos.magnitude();
}

unsigned ParticleCable::addContact(ParticleContact *contact,
                                    unsigned limit) const
{
    // Find the length of the cable
    double length = currentLength();

    // Check if we're over-extended
    if (length < maxLength)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    // Calculate the normal
    Vector3D normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal.normalize();
    contact->contactNormal = normal;

    contact->penetration = length-maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned ParticleRod::addContact(ParticleContact *contact,
                                  unsigned limit) const
{
    // Find the length of the rod
    double currentLen = currentLength();

    // Check if we're over-extended
    if (currentLen == length)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    // Calculate the normal
    Vector3D normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal.normalize();

    // The contact normal depends on whether we're extending or compressing
    if (currentLen > length) {
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
    } else {
        contact->contactNormal = normal * -1;
        contact->penetration = length - currentLen;
    }

    // Always use zero restitution (no bounciness)
    contact->restitution = 0;

    return 1;
}

double ParticleConstraint::currentLength() const
{
    Vector3D relativePos = particle->getPosition() - anchor;
    return relativePos.magnitude();
}

unsigned ParticleCableConstraint::addContact(ParticleContact *contact,
                                   unsigned limit) const
{
    // Find the length of the cable
    double length = currentLength();

    // Check if we're over-extended
    if (length < maxLength)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->particle[0] = particle;
    contact->particle[1] = 0;

    // Calculate the normal
    Vector3D normal = anchor - particle->getPosition();
    normal.normalize();
    contact->contactNormal = normal;

    contact->penetration = length-maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned ParticleRodConstraint::addContact(ParticleContact *contact,
                                 unsigned limit) const
{
    // Find the length of the rod
    double currentLen = currentLength();

    // Check if we're over-extended
    if (currentLen == length)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->particle[0] = particle;
    contact->particle[1] = 0;

    // Calculate the normal
    Vector3D normal = anchor - particle->getPosition();
    normal.normalize();

    // The contact normal depends on whether we're extending or compressing
    if (currentLen > length) {
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
    } else {
        contact->contactNormal = normal * -1;
        contact->penetration = length - currentLen;
    }

    // Always use zero restitution (no bounciness)
    contact->restitution = 0;

    return 1;
}