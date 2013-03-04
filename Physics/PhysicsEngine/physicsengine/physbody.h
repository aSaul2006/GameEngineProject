/***************************************************************************************
*  Program Name:    PhysBody.h                                                         *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            January 22, 2013                                                   *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/


#ifndef PHYSICSENGINE_PHYSBODY_H_
#define PHYSICSENGINE_PHYSBODY_H_

//-------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------

/***********************  These replace Core.h and .cpp *******************/
#include "AdditionalCode.h"
#include "matrix3.h"
#include "matrix4.h"
#include "Quaternion.h"
#include "vector3d.h"
/*****************************************************************************/


namespace PhysicsEngine
{



	/*defines the limit where an object goes into statis*/

	extern double statisFactor;


/*define the main class of the physics engine. Each game object
will be defined with its own body that will contain the elements
that are necessary to perform the physical effects, detect collisions, 
and the like*/

class PhysBody
{
public:
	
	/*  calculateObjectData - needs to be called after it is determined how the object's state
		has been modified.  Called automatically during physicsIntegration()*/

	void calculateObjectData();


	/* physicsIntegration - takes a float of the time elapsed from the last frame and will
							have the object moved based upon the phyiscal effects on the object*/

	void  physicsIntegration(double dElapsedFrameTime);

	
	/* setBodyMass()  - creates the mass of the body which cannot be zero. Issues
	                    may exist if the mass of the body is close to zero*/

	void setBodyMass(const double dMass);

	
	/*getBodyMass()  - gets the body's mass */

	double getBodyMass() const;


	/*Sets the inverse mass of the PhysBody*/

	void setInverseBodyMass(const double m_dInverseMass);



	/*gets the inverst mass of the PhysBody*/

	double getInverseBodyMass() const;

	
	/*check to see if the body has a finite mass*/

	bool bodyHasFiniteMass() const;



	/*Create the interia tensor for the PhysBody*/

	void setInertiaTensor(const Matrix3 &inertiaTensor);



	/*gets the interia tensor and places it into the given
	matrix*/

	void getInertiaTensor(Matrix3 *inertiaTensor) const;



	/*gets a copy of the inertia tensor of the PhysBody 
	defined in local space*/

	Matrix3 getInertiaTensor() const;




	/*gets the world interia tensor and places it into
	the give matrix*/

	void getInertiaTensorWorld(Matrix3 *inertiaTensor) const;



	/*gets a copy of the worrld inertia tensor of the 
	PhysBody*/

	Matrix3 getInertiaTensorWorld() const;


	/*sets the inverse inertia tensor.  Should be called before
	attempting to get any settings from PhysBody*/

	void setInverseInertiaTensor(const Matrix3 &inverseInertiaTensor);


	/*cgets a copy of the inverse inertia tensor and places 
	it in the supplied matrix*/

	void getInverseInertiaTensor(Matrix3 *inverseInertiaTensor) const;



	/*gets a copy of the inverse inertia tensor*/

	Matrix3  getInverseInertiaTensor() const;


	
	/*copies the inverse inertia tensor in world space*/

	void getInverseInertiaTensorWorld(Matrix3 *inverseInertiaTensor) const;



	/*gets a copy of the inverse inertia tensor in world
	space*/

	Matrix3 getInverseInertiaTensorWorld();



	/*the below function sets both the linear and angular 
	daming within a single call*/


	void setObjectDamping (const double m_dLinearDamping, const
		double m_dAngularDamping);


	/*the following functin sets the object's linear
	damping*/

	void setObjectLinearDamping (const double m_dLinearDamping);

	

	/*the following function gets the object's linear 
	damping*/

	double getObjectLinearDamping();



	/*the below function sets the object's angular
	damping*/

	void setObjectAngularDamping(const double m_dAngularDamping);



	/*the below function gets the object's angular
	damping*/

	double getObjectAngularDamping() const;



	/*the below the function sets the position of
	the body*/

	void setBodyPosition(const Vector3D &m_v3Position);

	/*the below function sets the position of
	the body based on the supplied x,y,z coordinates*/

	void setBodyPosition(const double x, const double y, const double z);

	
    /*gets the position and places in the given 
	vector*/

    void getBodyPosition(Vector3D *m_v3Position) const;


	/*gets the postion of the PhysBody*/

    Vector3D getBodyPosition() const;


	Matrix4 SetOrientationAndPos(const Quaternion &orientation, const Vector3D &pos);

	/*sets the orientation of the ridged body*/

    void setBodyOrientationQ(const Quaternion &qOrientation);

	

	/*The below function sets the orientation of PhysBody
	via the suypplied componets of the quaternion*/

    void setBodyOrientation(const double r, const double i,
            const double j, const double k);



	/*The below function retrieves the body's orientation 
	placing it in the supplied quaternion*/

    void getBodyOrientation(Quaternion *m_qOrientation) const;



	/*the below function returns the orientation of 
	the PhysBody*/

	Quaternion getBodyOrientation() const;



	/*the below function returns a given matrix
	with the object's orientation*/

	void getBodyOrientation(Matrix3 *matrix) const;



	/*The below function returns the orientation via the
	given matrix*/

	void getBodyOrientation(double matrix[9]) const;



	/* The below function returns the given matrix with a 
	tranformation of the body's orientation and position.  Used
	with a vector, will turn the body local to world space*/
       
    void getTransform(Matrix4 *transformMatrix) const;



	/*The belor function fills the given matrix contruct
	with a transformation of the body's orientation and 
	position*/

    void getTransform(double matrix[16]) const;



	/* Fills the given matrix structure with a transformation
	of the body's orientation and position*/

    void getGLTransform(double matrix[16]) const;




	/* the below matrix returns a tranformation that is the
	PhysBody's position and orientation*/

    Matrix4 getTransform() const;



	/*the below functin converts the point from world to 
	local space*/

    Vector3D getPointInLocalSpace(const Vector3D &point) const;


	/* The below function converts the supplied point from 
	local to world space */

	Vector3D getPointInWorldSpace(const Vector3D &point) const;



	/*The below function converts the supplied directin from world
	to local space */

	Vector3D getDirectionInLocalSpace(const Vector3D &direction) const;



	/* the below functionc convers direction from world to local
	space*/

    Vector3D getDirectionInWorldSpace(const Vector3D &direction) const;



	/*the below function sets the velocity of 
	PhysBody*/

	void setObjectVelocity(const Vector3D &velocity);


	/*The below function sets the object's velocity
	with the supplied componets*/

    void setObjectVelocity(const double x, const double y, const double z);


	/*returns the velocity of the object in world space*/

    void getObjectVelocity(Vector3D *velocity) const;


	/* Get the velocity of PhysBody */
      
	Vector3D getObjectVelocity() const;


	/*adds the velocity to an object*/

	void addVelocityToObject(const Vector3D &deltaVelocity);


	/*sets the roation of Physbody */
	
	void setObjectRotation(const Vector3D &rotation);



	/*sets the rotation of PhysBody by the
	supplied componets*/

	void setObjectRotation(const double x, const double y, const double z);


	/*returns the object's vector into the given vector*/

	void getObjectRotation(Vector3D *rotation) const;

	
	/*returns the rotation of PhysBody*/

	Vector3D getObjectRotation() const;


	/*adds the rotation to the PhysBody*/
	
	void addObjectRotation(const Vector3D &deltaRotation);


	/*check to see if PhysBody is in statis and able 
	to run through integration*/

	bool getObjectStatis() const
        {
            return m_bIsInStatis;
        }

	 

	/*Sets whether the body is in Statis or not.  If it is in 
	statis, velocities are cancelled*/


	void setObjectStatis (const bool awake=true);


	/*Boolen returns wheter the body can go into 
	stasis at any time*/

	bool getGoIntoStatis() const
        {
            return m_bCanBePutInStatis;
        }




	/*sets whether the body is ever able to go into statis*/

    void setCanGoIntoStatis(const bool m_bCanBePutInStatis=true);



	/* returns the given vector containing the summed value for
	linear acceleration*/
       
	void getAccumulatedAcceleration(Vector3D *linearAcceleration) const;



	/* returns the given vector containing the summed value for
	linear acceleration*/

	Vector3D getAccumulatedAcceleration() const;

     

	/*Clears the force/torque accumulators*/

    void emptyAccumulators();



	/* adds the force to the center of mass of PhysBody*/

	void addForceToObject(const Vector3D &force);

	/* Adds the supplied force to the supplied point
	on PhysBody*/

	void addForceAtPoint(const Vector3D &force, const Vector3D &point);

	/* Adds the supplied force to the supplied pooint on 
	Physbody, but application point is in body space, not world 
	space*/

    void addForceAtBodyPoint(const Vector3D &force, const Vector3D &point);


	/*add torque to the body*/

	void addTorqueToBody(const Vector3D &torque);


	/*set the acceleration of the PhysBody*/

    void setBodyAcceleration(const Vector3D &acceleration);


	/*set the acceleration og the PhysBody by
	component*/

	void setBodyAcceleration(const double x, const double y, const double z);

	/*returns given vector with body acceleration*/


	void getBodyAcceleration(Vector3D *acceleration) const;

	/*Retunrs objects Accelertion */

	Vector3D getBodyAcceleration() const;



	/*holds the motion of the body*/

	// redundant already exists (BS jan/08/13)double dMotion;

	/* sets the current sleepFactor value*/

/*	void setStatisFactor(double value);*/


	/*gets the current sleepFactor value*/

/*	void getStatisFactor();*/



protected:
	

/* accumulatedForce contains all of the forces that need to be applied at the 
	next physicsIntegration() step */

	Vector3D m_v3AccumulatedForce;

	/* accummulatedTorque contains all of the torque forces that need to be applied
	at the next physicsIntegration() step*/

	Vector3D m_v3AccumulatedTorque;

	// Contains the inertia tensor
	Matrix3 inertiaTensor;

	/*Contains the inverse of the body interia tensor. */

	Matrix3 m_m3InverseInertiaTensor;

	/*  inverseInertiaTensorWorld holdes the inverse intertia tensor of the body in the 
	wordspace.  Used to calculate angular acceleration*/

	Matrix3 m_m3InverseInertiaTensorWorld;

	/* holds the previous frame linear velocity of the body within
	the world space */

	Vector3D m_v3AccelerationLastFrame;

	/*  contains the linear veolicty of the body within
	world space */

	Vector3D m_v3Velocity;


	/*contains the angular velocity (or rotation) of a body
	within the world space*/

	Vector3D m_v3Rotation;

	/*contains the position of the a body within the
	world space*/

	Vector3D m_v3Position;

	/*Contains the angular orientation of a body within 
	the world space*/

	Quaternion m_qOrientation;

	/*The transform matrix used to convert body space 
	into world space.*/

	Matrix4 m_m4TransformMatrix;


	/*a boolean to determine wheter the object is in statis
	and can have phyical forces impact it*/

	bool   m_bIsInStatis; 

	/*a boolean that signifies that a body is able to be 
	placed in statis so that physical forces do not impact
	it*/

	bool   m_bCanBePutInStatis;

	/*The following holds the amount of motion energy and
	is used to determine whether the object should be put
	into statis */

	double m_dMotion;

	/*The following holds the acceleration vector of the
	body. */

	Vector3D m_v3Acceleration;


	/*The following holds the damping coefficent for 
	the linear accelleration of the object*/

	double m_dLinearDamping;


	/*The following holds the damping coefficent for
	the angular acceleration of the object*/

	double m_dAngularDamping;


	/*The following hold the inverse mass of the
	object*/

	double m_dInverseMass;
	
	double dMass;

	};
}



#endif