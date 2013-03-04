/***************************************************************************************
*  Program Name:    pfgen.cpp                                                          *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Impementation file for teh particle force generators               *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Implementation file for the particle force generators.
 *
 */

#include "physicsengine\pfgen.h"

using namespace PhysicsEngine;


void ParticleForceRegistry::updateForces(double dElapsedFrameTime)
{
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++)
    {
        i->fg->updateForce(i->particle, dElapsedFrameTime);
    }
}

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator *fg)
{
    ParticleForceRegistry::ParticleForceRegistration registration;
    registration.particle = particle;
    registration.fg = fg;
    registrations.push_back(registration);
}

ParticleGravity::ParticleGravity(const Vector3D& gravity)
: gravity(gravity)
{
}

void ParticleGravity::updateForce(Particle* particle, double dElapsedFrameTime)
{
    // Check that we do not have infinite mass
    if (!particle->hasFiniteMass()) return;

    // Apply the mass-scaled force to the particle
    particle->addForce(gravity * particle->getMass());
}

ParticleDrag::ParticleDrag(double k1, double k2)
: k1(k1), k2(k2)
{
}

void ParticleDrag::updateForce(Particle* particle, double dElapsedFrameTime)
{
    Vector3D force;
    particle->getVelocity(&force);

    // Calculate the total drag coefficient
    double dragCoeff = force.magnitude();
    dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

    // Calculate the final force and apply it
    force.normalize();
    force *= -dragCoeff;
    particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle *other, double sc, double rl)
: other(other), springConstant(sc), restLength(rl)
{
}

void ParticleSpring::updateForce(Particle* particle, double dElapsedFrameTime)
{
    // Calculate the vector of the spring
    Vector3D force;
    particle->getPosition(&force);
    force -= other->getPosition();

    // Calculate the magnitude of the force
    double magnitude = force.magnitude();
    magnitude = abs(magnitude - restLength);
    magnitude *= springConstant;

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}

ParticleBuoyancy::ParticleBuoyancy(double maxDepth,
                                 double volume,
                                 double waterHeight,
                                 double liquidDensity)
:
maxDepth(maxDepth), volume(volume),
waterHeight(waterHeight), liquidDensity(liquidDensity)
{
}

void ParticleBuoyancy::updateForce(Particle* particle, double dElapsedFrameTime)
{
    // Calculate the submersion depth
    double depth = particle->getPosition().y;

    // Check if we're out of the water
    if (depth >= waterHeight + maxDepth) return;
    Vector3D force(0,0,0);

    // Check if we're at maximum depth
    if (depth <= waterHeight - maxDepth)
    {
        force.y = liquidDensity * volume;
        particle->addForce(force);
        return;
    }

    // Otherwise we are partly submerged
    force.y = liquidDensity * volume *
        (depth - maxDepth - waterHeight) / 2 * maxDepth;
    particle->addForce(force);
}

ParticleBungee::ParticleBungee(Particle *other, double sc, double rl)
: other(other), springConstant(sc), restLength(rl)
{
}

void ParticleBungee::updateForce(Particle* particle, double dElapsedFrameTime)
{
    // Calculate the vector of the spring
    Vector3D force;
    particle->getPosition(&force);
    force -= other->getPosition();

    // Check if the bungee is compressed
    double magnitude = force.magnitude();
    if (magnitude <= restLength) return;

    // Calculate the magnitude of the force
    magnitude = springConstant * (restLength - magnitude);

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}

ParticleFakeSpring::ParticleFakeSpring(Vector3D *anchor, double sc, double d)
: anchor(anchor), springConstant(sc), damping(d)
{
}

void ParticleFakeSpring::updateForce(Particle* particle, double dElapsedFrameTime)
{
    // Check that we do not have infinite mass
    if (!particle->hasFiniteMass()) return;

    // Calculate the relative position of the particle to the anchor
    Vector3D position;
    particle->getPosition(&position);
    position -= *anchor;

    // Calculate the constants and check they are in bounds.
    double gamma = 0.5f * sqrt(4 * springConstant - damping*damping);
    if (gamma == 0.0f) return;
    Vector3D c = position * (damping / (2.0f * gamma)) +
        particle->getVelocity() * (1.0f / gamma);

    // Calculate the target position
    Vector3D target = position * cos(gamma * dElapsedFrameTime) +
        c * sin(gamma * dElapsedFrameTime);
    target *= exp(-0.5f * dElapsedFrameTime * damping);

    // Calculate the resulting acceleration and therefore the force
    Vector3D accel = (target - position) * (1.0f / dElapsedFrameTime*dElapsedFrameTime) -
        particle->getVelocity() * dElapsedFrameTime;
    particle->addForce(accel * particle->getMass());
}

ParticleAnchoredSpring::ParticleAnchoredSpring()
{
}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3D *anchor,
                                               double sc, double rl)
: anchor(anchor), springConstant(sc), restLength(rl)
{
}

void ParticleAnchoredSpring::init(Vector3D *anchor, double springConstant,
                                  double restLength)
{
    ParticleAnchoredSpring::anchor = anchor;
    ParticleAnchoredSpring::springConstant = springConstant;
    ParticleAnchoredSpring::restLength = restLength;
}

void ParticleAnchoredBungee::updateForce(Particle* particle, double dElapsedFrameTime)
{
    // Calculate the vector of the spring
    Vector3D force;
    particle->getPosition(&force);
    force -= *anchor;

    // Calculate the magnitude of the force
    double magnitude = force.magnitude();
    if (magnitude < restLength) return;

    magnitude = magnitude - restLength;
    magnitude *= springConstant;

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}

void ParticleAnchoredSpring::updateForce(Particle* particle, double dElapsedFrameTime)
{
    // Calculate the vector of the spring
    Vector3D force;
    particle->getPosition(&force);
    force -= *anchor;

    // Calculate the magnitude of the force
    double magnitude = force.magnitude();
    magnitude = (restLength - magnitude) * springConstant;

    // Calculate the final force and apply it
    force.normalize();
    force *= magnitude;
    particle->addForce(force);
}