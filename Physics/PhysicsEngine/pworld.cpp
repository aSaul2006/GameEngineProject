/***************************************************************************************
*  Program Name:    pworld.cpp                                                         *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Implementation file for the particle world                         *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Implementation file for random number generation.
*/

#include <cstdlib>
#include "physicsengine\pworld.h"

using namespace PhysicsEngine;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
:
resolver(iterations),
maxContacts(maxContacts)
{
    contacts = new ParticleContact[maxContacts];
    calculateIterations = (iterations == 0);

}

ParticleWorld::~ParticleWorld()
{
    delete[] contacts;
}

void ParticleWorld::startFrame()
{
    for (Particles::iterator p = particles.begin();
        p != particles.end();
        p++)
    {
        // Remove all forces from the accumulator
        (*p)->clearAccumulator();
    }
}

unsigned ParticleWorld::generateContacts()
{
    unsigned limit = maxContacts;
    ParticleContact *nextContact = contacts;

    for (ContactGenerators::iterator g = contactGenerators.begin();
        g != contactGenerators.end();
        g++)
    {
        unsigned used =(*g)->addContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        // We've run out of contacts to fill. This means we're missing
        // contacts.
        if (limit <= 0) break;
    }

    // Return the number of contacts used.
    return maxContacts - limit;
}

void ParticleWorld::integrate(double dElapsedFrameTime)
{
    for (Particles::iterator p = particles.begin();
        p != particles.end();
        p++)
    {
        // Remove all forces from the accumulator
        (*p)->integrate(dElapsedFrameTime);
    }
}

void ParticleWorld::runPhysics(double dElapsedFrameTime)
{
    // First apply the force generators
    registry.updateForces(dElapsedFrameTime);

    // Then integrate the objects
    integrate(dElapsedFrameTime);

    // Generate contacts
    unsigned usedContacts = generateContacts();

    // And process them
    if (usedContacts)
    {
        if (calculateIterations) resolver.setIterations(usedContacts * 2);
        resolver.resolveContacts(contacts, usedContacts, dElapsedFrameTime);
    }
}

ParticleWorld::Particles& ParticleWorld::getParticles()
{
    return particles;
}

ParticleWorld::ContactGenerators& ParticleWorld::getContactGenerators()
{
    return contactGenerators;
}

ParticleForceRegistry& ParticleWorld::getForceRegistry()
{
    return registry;
}

void GroundContacts::init(PhysicsEngine::ParticleWorld::Particles *particles)
{
    GroundContacts::particles = particles;
}

unsigned GroundContacts::addContact(PhysicsEngine::ParticleContact *contact,
                                    unsigned limit) const
{
    unsigned count = 0;
    for (PhysicsEngine::ParticleWorld::Particles::iterator p = particles->begin();
        p != particles->end();
        p++)
    {
        double y = (*p)->getPosition().y;
        if (y < 0.0f)
        {
            contact->contactNormal = PhysicsEngine::Vector3D::UP;
            contact->particle[0] = *p;
            contact->particle[1] = NULL;
            contact->penetration = -y;
            contact->restitution = 0.2f;
            contact++;
            count++;
        }

        if (count >= limit) return count;
    }
    return count;
}