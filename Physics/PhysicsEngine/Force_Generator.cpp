/*****************************************************************************************
*                                                                                        *
*  Program Name:          force_generator.cpp                                            *
*  Name:                  Jennifer Wickensimer                                           *
*  Date:                  January 23, 2012                                               *
*  Description:           contains function definitions for the application of forces to *
*                         objects that have collided or are impacted by natural forces   *
*  Update:                Jennifer Wickensime 01/28/2012                                 *
*                                                                                        *
*****************************************************************************************/

#include "physicsengine\Force_Generator.h"

using namespace PhysicsEngine;

/* the following function iterates through all the registered
bodies and updates the information based upon the forces assigned
to them*/


void ForceRegistry::updateForcesOnBodies(double dElapsedFrameTime)  
{
	// set up an iterator where the registration inforamtion begins
	Registry::iterator i = registrations.begin();

	/*go through each registration, update the forcesgenerated 
	that is stored in the body*/

	for (; i != registrations.end(); i++)
	{
		i->pFg->updateForce(i->pBody, dElapsedFrameTime);
	}
}


/* the following function adds a Physbody to the registry
with initial forces*/

void ForceRegistry::addToRegistry(PhysBody *pBody, ForceGenerator *pFg)
{
	//create the data holder 
	ForceRegistry::ForceRegistration registration;
	
	/*put the body and force generator information into
	the data holder */
	registration.pBody = pBody;
	registration.pFg = pFg;
	
	/* utilizing the STD::Vector::push_back since it is 
	already written */
	registrations.push_back(registration);
}

/* Now to allow gravity to affect the objects in
world space*/

Gravity::Gravity(const Vector3D& v3Gravity)
	: m_v3Gravity(v3Gravity)
{
}

void Gravity::updateForce(PhysBody* pBody, double dElapsedFrameTime)
{
	/*need to check if the body does not have infinite mass
	if it does, gravity will not affect it, at least in the 
	universes that I am familar with*/

	if (!pBody->bodyHasFiniteMass()) return;

	/* ok, the body can be affected by the force of gravity, so
	apply the gravity based upon the scale of the mass */

	pBody->addForceToObject(m_v3Gravity * pBody->getBodyMass()  );
}

Buoyancy::Buoyancy(const Vector3D &cOfB, double maxDepth, double volume,
                   double waterHeight, double liquidDensity /* = 1000.0f */)
{
    centreOfBuoyancy = cOfB;
    Buoyancy::liquidDensity = liquidDensity;
    Buoyancy::maxDepth = maxDepth;
    Buoyancy::volume = volume;
    Buoyancy::waterHeight = waterHeight;
}

void Buoyancy::updateForce(PhysBody *pBody, double duration)
{
    // Calculate the submersion depth
    Vector3D pointInWorld = pBody->getPointInWorldSpace(centreOfBuoyancy);
    double depth = pointInWorld.y;

    // Check if we're out of the water
    if (depth >= waterHeight + maxDepth) return;
    Vector3D force(0,0,0);

    // Check if we're at maximum depth
    if (depth <= waterHeight - maxDepth)
    {
        force.y = liquidDensity * volume;
        pBody->addForceAtBodyPoint(force, centreOfBuoyancy);
        return;
    }

    // Otherwise we are partly submerged
    force.y = liquidDensity * volume *
        (depth - maxDepth - waterHeight) / 2 * maxDepth;
    pBody->addForceAtBodyPoint(force, centreOfBuoyancy);
}

Spring::Spring(const Vector3D &localConnectionPt,
               PhysBody *other,
               const Vector3D &otherConnectionPt,
               double springConstant,
               double restLength)
: connectionPoint(localConnectionPt),
  otherConnectionPoint(otherConnectionPt),
  other(other),
  springConstant(springConstant),
  restLength(restLength)
{
}

void Spring::updateForce(PhysBody* pBody, double duration)
{
    // Calculate the two ends in world space
    Vector3D lws = pBody->getPointInWorldSpace(connectionPoint);
    Vector3D ows = other->getPointInWorldSpace(otherConnectionPoint);

    // Calculate the vector of the spring
    Vector3D force = lws - ows;

    // Calculate the magnitude of the force
    double magnitude = force.magnitude();
    magnitude = abs(magnitude - restLength);
    magnitude *= springConstant;

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    pBody->addForceAtPoint(force, lws);
}

Aero::Aero(const Matrix3 &tensor, const Vector3D &position, const Vector3D *windspeed)
{
    Aero::tensor = tensor;
    Aero::position = position;
    Aero::windspeed = windspeed;
}

void Aero::updateForce(PhysBody *pBody, double duration)
{
    Aero::updateForceFromTensor(pBody, duration, tensor);
}

void Aero::updateForceFromTensor(PhysBody *pBody, double duration,
                                 const Matrix3 &tensor)
{
    // Calculate total velocity (windspeed and body's velocity).
    Vector3D velocity = pBody->getObjectVelocity();
    velocity += *windspeed;

    // Calculate the velocity in body coordinates
    Vector3D bodyVel = pBody->getTransform().transformInverseDirection(velocity);

    // Calculate the force in body coordinates
    Vector3D bodyForce = tensor.transform(bodyVel);
    Vector3D force = pBody->getTransform().transformDirection(bodyForce);

    // Apply the force
    pBody->addForceAtBodyPoint(force, position);
}

AeroControl::AeroControl(const Matrix3 &base, const Matrix3 &min, const Matrix3 &max,
                              const Vector3D &position, const Vector3D *windspeed)
:
Aero(base, position, windspeed)
{
    AeroControl::minTensor = min;
    AeroControl::maxTensor = max;
    controlSetting = 0.0f;
}

Matrix3 AeroControl::getTensor()
{
    if (controlSetting <= -1.0f) return minTensor;
    else if (controlSetting >= 1.0f) return maxTensor;
    else if (controlSetting < 0)
    {
        return Matrix3::LinearInterpolate(minTensor, tensor, controlSetting+1.0f);
    }
    else if (controlSetting > 0)
    {
        return Matrix3::LinearInterpolate(tensor, maxTensor, controlSetting);
    }
    else return tensor;
}

void AeroControl::setControl(double value)
{
    controlSetting = value;
}

void AeroControl::updateForce(PhysBody *pBody, double duration)
{
    Matrix3 tensor = getTensor();
    Aero::updateForceFromTensor(pBody, duration, tensor);
}

void Explosion::updateForce(PhysBody* pBody, double duration)
{

}