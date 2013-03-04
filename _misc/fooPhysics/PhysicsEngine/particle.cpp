/***************************************************************************************
*  Program Name:    particle.cpp                                                       *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Implementation file for the particle system.                       *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Implementation file for the particle class.
 *
 */

#include <assert.h>
#include "physicsengine\particle.h"

using namespace PhysicsEngine;


/*
 * --------------------------------------------------------------------------
 * FUNCTIONS DECLARED IN HEADER:
 * --------------------------------------------------------------------------
 */

void Particle::integrate(double dElapsedFrameTime)
{
    // We don't integrate things with zero mass.
    if (inverseMass <= 0.0f) return;

    assert(dElapsedFrameTime > 0.0);

    // Update linear position.
    position.addScaledVector(velocity, dElapsedFrameTime);

    // Work out the acceleration from the force
    Vector3D resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    // Update linear velocity from the acceleration.
    velocity.addScaledVector(resultingAcc, dElapsedFrameTime);

    // Impose drag.
    velocity *= pow(damping, dElapsedFrameTime);

    // Clear the forces.
    clearAccumulator();
}



void Particle::setMass(const double mass)
{
    assert(mass != 0);
    Particle::inverseMass = ((double)1.0)/mass;
}

double Particle::getMass() const
{
    if (inverseMass == 0) {
        return DBL_MAX;
    } else {
        return ((double)1.0)/inverseMass;
    }
}

void Particle::setInverseMass(const double inverseMass)
{
    Particle::inverseMass = inverseMass;
}

double Particle::getInverseMass() const
{
    return inverseMass;
}

bool Particle::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

void Particle::setDamping(const double damping)
{
    Particle::damping = damping;
}

double Particle::getDamping() const
{
    return damping;
}

void Particle::setPosition(const Vector3D &position)
{
    Particle::position = position;
}

void Particle::setPosition(const double x, const double y, const double z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void Particle::getPosition(Vector3D *position) const
{
    *position = Particle::position;
}

Vector3D Particle::getPosition() const
{
    return position;
}

void Particle::setVelocity(const Vector3D &velocity)
{
    Particle::velocity = velocity;
}

void Particle::setVelocity(const double x, const double y, const double z)
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void Particle::getVelocity(Vector3D *velocity) const
{
    *velocity = Particle::velocity;
}

Vector3D Particle::getVelocity() const
{
    return velocity;
}

void Particle::setAcceleration(const Vector3D &acceleration)
{
    Particle::acceleration = acceleration;
}

void Particle::setAcceleration(const double x, const double y, const double z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void Particle::getAcceleration(Vector3D *acceleration) const
{
    *acceleration = Particle::acceleration;
}

Vector3D Particle::getAcceleration() const
{
    return acceleration;
}

void Particle::clearAccumulator()
{
    forceAccum.clear();
}

void Particle::addForce(const Vector3D &force)
{
    forceAccum += force;
}