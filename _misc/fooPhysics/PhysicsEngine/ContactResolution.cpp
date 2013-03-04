

/*****************************************************************************************
*                                                                                        *
*  Program Name:          ContactResolution.cpp                                          *
*  Name:                  Jennifer Wickensimer                                           *
*  Date:                  January 29, 2012                                               *
*  Description:           cpp for implementing the functionality to provide for          *
*                         the resolution of contacts                                     *
*  Update:                                                                               *
*                                                                                        *
*****************************************************************************************/
//----------------------------------------------------------------------------------------
//               Includes
//----------------------------------------------------------------------------------------
#include "physicsengine\ContactResolution.h"
#include <memory.h>
#include <assert.h>
#include <math.h>



using namespace PhysicsEngine;
//---------------------------------------------------------------------------------------
// Class Contact implementation
//---------------------------------------------------------------------------------------

/*function that sets the data that is independent
of the objects posistion*/

void Contact::setBodyData(PhysBody *one, PhysBody *two, double m_dFriction, double m_dRestitution)
{
        Contact::body[0] = one;
        Contact::body [1] = two;
        Contact::m_dFriction = m_dFriction;
        Contact::m_dRestitution = m_dRestitution;
}

void Contact::integrateAwakeState()
{   
        /*if the collision is with the world the body does not wake up.
          Put another way, if the body is asleep there isn't a velocity 
          or penatration to reslove.*/
        if (!body[1])
        {
                return;
        }

        bool bodyZeroAwake = body[0] -> getObjectStatis(); 

        bool bodyOneAwake = body[1] -> getObjectStatis();


        //Only wake up the object that is sleeping 
        if (bodyZeroAwake ^ bodyOneAwake)
        {
                if (bodyZeroAwake) body[1] -> setObjectStatis();
                else body[0] -> setObjectStatis();
        }
}

/*  Contact reversal.  Switch the two PhysBody objects and reverse
        the normal.  Need to run calculateInternals again because this method
        does not update the internals. If we weren't going to have two body 
        collisions we wouldn't need this method, but we can't assume that is true*/
void Contact::switchBodies()
{
        //flip the normal 
        m_v3NormalofContactWorld *= -1;

         PhysBody *pTempBody = body[0];

         //here comes the switcher roo
         body[0] = body[1];
         body[1] = pTempBody;
}
/*determines an orthornormal basis for the contact point. 
It is stored as a 3x3 matrix where each vector is a column 
ex. : 
                 |a|          |d|           |g|
        x local =        |b| y local =|e| z local = |h| =
                         |c|          |f|           |i|

                  |a||d||g|
        M 3x3 basis = |b||e||h| = P world
                      |c||f||i|

  where P world is world cords. */
inline void Contact::calcBasisForContact()
{
        Vector3D contactTan[2];

        //is Z axis closer to  X or Y axis 
        if (fabs(m_v3NormalofContactWorld.x) > fabs(m_v3NormalofContactWorld.y))
        { 
                //var to make sure reslut is normalized
                const double scaleFactor = 1.0/ sqrt(m_v3NormalofContactWorld.z *m_v3NormalofContactWorld.z +
                                                         m_v3NormalofContactWorld.x * m_v3NormalofContactWorld.x);

                // the new X axis is now at a right andle with the world's Y axis
                contactTan[0].x = m_v3NormalofContactWorld.z * scaleFactor;
                contactTan[0].y = 0;
                contactTan[0].z = -m_v3NormalofContactWorld.x * scaleFactor;

                //meaning that the new Y axis must be at right angles with the X and Z axis 
                contactTan[1].x = m_v3NormalofContactWorld.y * contactTan[0].x;
                contactTan[1].y = m_v3NormalofContactWorld.z * contactTan[0].x - m_v3NormalofContactWorld.x * contactTan[0].z;
                contactTan[1].z = - m_v3NormalofContactWorld.y * contactTan[0].x;
        }
        else
        {
                //var to make sure reslut is normalized
                const double scaleFactor = 1.0/ sqrt(m_v3NormalofContactWorld.z *m_v3NormalofContactWorld.z +
                                                         m_v3NormalofContactWorld.y * m_v3NormalofContactWorld.y);

                // the new X axis is now at a right andle with the world's X axis
                contactTan[0].x = 0;
            contactTan[0].y = -m_v3NormalofContactWorld.z * scaleFactor;
                contactTan[0].z = m_v3NormalofContactWorld.y * scaleFactor;

                //meaning that the new Y axis must be at right angles with the X and Z axis 
                contactTan[1].x = m_v3NormalofContactWorld.y * contactTan[0].z - m_v3NormalofContactWorld.z * contactTan[0].y;
                contactTan[1].y = - m_v3NormalofContactWorld.x * contactTan[0].z;
                contactTan[1].z = m_v3NormalofContactWorld.x * contactTan[0].y;
        }
        
        // now make a mtrix from the three vectors 
        m_m3TransformContactToWorld.SetComponents(m_v3NormalofContactWorld,contactTan[0],contactTan[1]); 
}

/* determine velocity on the body of its contact point*/
Vector3D Contact::calcVelocityLocal(unsigned bodyIndex, double dElapsedFrameTime)
{
        PhysBody *thisBody = body[bodyIndex];

        //find the velocity of the contact points 
        Vector3D velocity = thisBody -> getObjectRotation() %  m_v3RelContactWorldPosition[bodyIndex];
        velocity += thisBody ->getObjectVelocity();

        //turn that velocity into contact coords
        Vector3D contactVel = m_m3TransformContactToWorld.transformTranspose(velocity);
    
        //Now calc the amount of velocity that's because of forces without reactions
        Vector3D accelerationVelocity = thisBody ->getAccumulatedAcceleration()* dElapsedFrameTime;

        //turn that velocity into contact coords
         accelerationVelocity = m_m3TransformContactToWorld.transformTranspose(accelerationVelocity);

        
        /*Need to ignore acceleration in the direction of 
        the contact normal because we only care about acceleration on this plane*/
        accelerationVelocity.x = 0;

        //add the plannar velocity with enough friction they will be removed
        contactVel += accelerationVelocity;

        //do the return
        return contactVel;
}
//calculates the change in velocity 
void Contact::calcChangeOfVelocity(double dElapsedFrameTime){

        const static double velocityLimit = 0.25;

        //Calculate the velocity that coccured this frame due to acceleration
        double velocityFromAcceleration = 0;

        if (body[0] -> getObjectStatis()
)
        {
                body[0] -> getAccumulatedAcceleration()* dElapsedFrameTime * m_v3NormalofContactWorld;
        }

        if (body[1] && body[1] -> getObjectStatis())
        {
                velocityFromAcceleration -= body[1] ->getAccumulatedAcceleration()* dElapsedFrameTime * m_v3NormalofContactWorld;
        }

        //if the velocity is slow then limit the restitution 
        double thisRestitution = m_dRestitution;

        if (fabs(m_v3ContactClosingVelocity.x) < velocityLimit)
        {
                thisRestitution = 0.0;
        }

        //combine the bounce and removed acceleration velocity 
        m_dCalcChangedVelocity = -m_v3ContactClosingVelocity.x - thisRestitution 
                                       * (m_v3ContactClosingVelocity.x - velocityFromAcceleration);
                                       
}

void Contact::calculateInternals (double dElapsedFrameTime){
        
        //make sure first body is NULL if not swap
        if (!body[0]) 
        {
                switchBodies();
        }
        assert(body[0]);

        // calc a set of axis at the contact point 
        calcBasisForContact();

        // Store the relitive position of contact relative to the body
        m_v3RelContactWorldPosition[0] = m_v3PointOfContactWorld  - body[0] -> getBodyPosition();
          
        if (body[1]) 
        {
           m_v3RelContactWorldPosition[1] = m_v3PointOfContactWorld  - body[1] -> getBodyPosition();
        }

        //Find the relative velocity of the bodies at the contact point 
        m_v3ContactClosingVelocity = calcVelocityLocal (0, dElapsedFrameTime);

        if (body[1])
        {
                m_v3ContactClosingVelocity -= calcVelocityLocal (1, dElapsedFrameTime);
        }

        //calculate the desired change in velocity 
        calcChangeOfVelocity(dElapsedFrameTime);

}
void Contact::applyChangeInVelocity (Vector3D v3ChangeInVelocity[2],  Vector3D v3ChangeInRotation[2])
{ 
        //Get the inverse mass and inverse inertia tensor in world coords
        Matrix3 inverseInertiaTensor[2];

        body[0] -> getInverseInertiaTensorWorld (&inverseInertiaTensor[0]);

        if (body[1])
        {
                body[1] -> getInverseInertiaTensorWorld (&inverseInertiaTensor[1]);
        }

        //Now calculate the impulse for each contact axis 
        Vector3D  impulseContact;

        if (m_dFriction == 0.0)
        {
                //Calculate theh frictionless version
                impulseContact = calculateMomentumWithoutFriction(inverseInertiaTensor);
        }
        else
        {
                //the contacts arent in the same direction so have to calculate with friction
                impulseContact = calculateMomementIncludingFriction(inverseInertiaTensor);
        }

        //converte the impluse to world coords
        Vector3D impulse = m_m3TransformContactToWorld.transform(impulseContact);

        //Now split the impluse into its linear and rotational components 
        Vector3D implusiveTorque = m_v3RelContactWorldPosition[0] % impulse;
    v3ChangeInRotation[0] = inverseInertiaTensor[0].transform(implusiveTorque);
        v3ChangeInVelocity[0].clear();
        v3ChangeInVelocity[0].addScaledVector(impulse, body[0] ->getInverseBodyMass());

        //apply the changes 
        body[0]-> addVelocityToObject(v3ChangeInVelocity[0]);
        body[0] -> addObjectRotation(v3ChangeInRotation[0]);

        if (body[1])
        {
     //Now split the impluse into its linear and rotational components 
         // remember because its the other body everything is flip flopped
         Vector3D implusiveTorque = impulse % m_v3RelContactWorldPosition[1];
     v3ChangeInRotation[1] = inverseInertiaTensor[1].transform(implusiveTorque);
         v3ChangeInVelocity[1].clear();
         v3ChangeInVelocity[1].addScaledVector(impulse, -body[1] ->getInverseBodyMass());

         //apply the changes 
         body[1]-> addVelocityToObject(v3ChangeInVelocity[1]);
         body[1] -> addObjectRotation(v3ChangeInRotation[1]);
        }
}
/*determine the momentum neded to resolve the contact without
  friction taken into account.  Uses the inertia tensors for
  each object*/
inline Vector3D Contact::calculateMomentumWithoutFriction(Matrix3 *m3InverseInertiaTensor)
{
     Vector3D impulseContact;
    
         /*build a vector to represent the change in velocity 
         in world space in the direction of the contact norm*/
         Vector3D deltaVelocityWorld = m_v3RelContactWorldPosition[0] % m_v3NormalofContactWorld;
         deltaVelocityWorld = m3InverseInertiaTensor[0].transform(deltaVelocityWorld);
         deltaVelocityWorld = deltaVelocityWorld % m_v3RelContactWorldPosition[0];

         // figure out the change in velocity in contact coords
         double deltaVelocity = deltaVelocityWorld * m_v3NormalofContactWorld;

         //now add the linear component of the velocity change
         deltaVelocity += body[0] -> getInverseBodyMass();
          
         //do we need the other body's data 
         if(body[1])
         {
                 /*Do it again. AH Oh (<- pretending to be Davey from AFI)...  build a vector to represent the change in velocity 
         in world space in the direction of the contact norm*/
         Vector3D deltaVelocityWorld = m_v3RelContactWorldPosition[1] % m_v3NormalofContactWorld;
         deltaVelocityWorld = m3InverseInertiaTensor[1].transform(deltaVelocityWorld);
         deltaVelocityWorld = deltaVelocityWorld % m_v3RelContactWorldPosition[1];

         //rotation let's figure it out now 
         deltaVelocity += deltaVelocityWorld *  m_v3NormalofContactWorld;

          //now add the linear component of the velocity change
         deltaVelocity += body[1] -> getInverseBodyMass();
         }

         //what's the size of the impulse? Lets find out by calculating it. 
         impulseContact.x = m_dCalcChangedVelocity / deltaVelocity;
         impulseContact.y = 0;
         impulseContact.z = 0;

         //give it back now 
         return impulseContact; 
}

inline Vector3D Contact::calculateMomementIncludingFriction(Matrix3 *m3InverseInertiaTensor)
{
        //vars for equations
        Vector3D impulseContact;
        double inversePhysBodyMass = body[0] -> getInverseBodyMass();

        //build the matrix that coverts between angular and linear
        Matrix3 impluseToTorque;
        
        //work it out
        impluseToTorque.SetSkewSymmetric (m_v3RelContactWorldPosition[0]);

        /*create the matrix needed to covert the contact
        impluse to delta velocity in world coords*/
        Matrix3 deltaVelocityWorld = impluseToTorque;
        deltaVelocityWorld *= m3InverseInertiaTensor[0];
        deltaVelocityWorld *= impluseToTorque;
        deltaVelocityWorld *= -1;


        //now we gotta check to see if we need the other bodies data 
        if (body[1])
        {
                //cross product matrix 
                impluseToTorque.SetSkewSymmetric(m_v3RelContactWorldPosition[1]);

                //calc delta velocity matrix
                Matrix3 deltaVelocityWorldx2 = impluseToTorque;
                deltaVelocityWorldx2 *= m3InverseInertiaTensor[1];
                deltaVelocityWorldx2 *= impluseToTorque;
                deltaVelocityWorldx2 *= -1;  //opposite first body 

                // now we gotta add all that to delta velocity 
                deltaVelocityWorld += deltaVelocityWorldx2;

                //the same for the inverse mass 
                inversePhysBodyMass += body[1] ->getInverseBodyMass();
        }

        //basis change to covert into contact coords
        Matrix3 changeInVelocity = m_m3TransformContactToWorld.Transpose();
        changeInVelocity *= deltaVelocityWorld;
        changeInVelocity *= m_m3TransformContactToWorld;

        //we're adding agian...add change in the linear velocity 
        //0,4, 8, in a 3 X 3 matrix 
        changeInVelocity.data[0] += inversePhysBodyMass;
        changeInVelocity.data[4] += inversePhysBodyMass;
        changeInVelocity.data[8] += inversePhysBodyMass;

        // invert fo the unit that is needed for the units of velocity 
        Matrix3 impulseMatrix = changeInVelocity.Inverse();

        //find the velocities that need destroyed
        Vector3D destroyVelocity (m_dCalcChangedVelocity, -m_v3ContactClosingVelocity.y, 
                                                              -m_v3ContactClosingVelocity.z);

        //find the impulse to kill the velocities
        impulseContact = impulseMatrix.transform(destroyVelocity);
        
        //check for friction 
        double planarImpulse = sqrt(impulseContact.y * impulseContact.y + impulseContact.z *impulseContact.z);
        
        //make sure we are with in the limit of static friction 
        if(planarImpulse > impulseContact.x * m_dFriction)
        {
                 //if not take care of kinect friction 
                //scaling 
                impulseContact.y /= planarImpulse;
        impulseContact.z /= planarImpulse;

                
                impulseContact.x = changeInVelocity.data[0] + changeInVelocity.data[1] * m_dFriction *
                                       impulseContact.y + changeInVelocity.data[2] * m_dFriction *impulseContact.z;

                impulseContact.x = m_dCalcChangedVelocity / impulseContact.x;

                //newvalues for each component 
                impulseContact.y *= m_dFriction * impulseContact.x;
                impulseContact.z *= m_dFriction * impulseContact.x;
        }
        return impulseContact;
}
void Contact::applyChangeInPosition (Vector3D v3ChangeInLinear[2], Vector3D v3ChangeInAngle[2],
                                                                                                         double  d_DepthofContact)
{
        const double angLimit = 0.2;
        double angMove[2];
        double linearMove[2];

        double totalInertia = 0;
    double linearInertia[2];
    double angInertia[2];

        //work out intertia in direction of contact norm 

        for( unsigned i = 0; i < 2; i ++){
                if(body[i]){
                        Matrix3 inverseInertiaTensor;
                        body[i] -> getInverseInertiaTensorWorld(&inverseInertiaTensor);

                        //same deal as calciong the velocity with/o friction 
                        //this time to work angular inertia 
                        Vector3D angInertiaTensorWorld = m_v3RelContactWorldPosition [i] % m_v3NormalofContactWorld;
                        angInertiaTensorWorld = inverseInertiaTensor.transform(angInertiaTensorWorld);
                        angInertiaTensorWorld = angInertiaTensorWorld % m_v3RelContactWorldPosition [i];
                        angInertia[i] = angInertiaTensorWorld * m_v3NormalofContactWorld;

                        //linear compinet == inverse mass 
                        linearInertia[1] = body[i] -> getInverseBodyMass();

                        //keep track  of the total inertia 
                        totalInertia += linearInertia[i] + angInertia[i];
        

                }
        
        }
        // apply changes 
        for (unsigned i = 0; i < 2 ; i++)
        { 
                if (body[i])
                { 
                        //angular and linear poroportional to the inverse intertias 
                        double sign = ( i == 0)?1: -1;
                        angMove[i] = sign * d_DepthofContact * (angInertia[i]/totalInertia);
                        linearMove [i] = sign *  d_DepthofContact * (linearInertia[i] / totalInertia);


                        //limit the angualr move to aviod projections that are too big 
                        Vector3D  projection =  m_v3RelContactWorldPosition [i]; 
                        projection.addScaledVector(m_v3NormalofContactWorld, - m_v3RelContactWorldPosition[i].scalarProduct(m_v3NormalofContactWorld));

                        //use the small angle approximation for the sine of the angle 
                        double maxMagnitude = angLimit * projection.magnitude();

                        if (angMove[i] < -maxMagnitude)
                        {
                                double totalMove  = angMove[i] + linearMove[i];
                                angMove[i] = -maxMagnitude; 
                                linearMove[i]= totalMove - angMove[i];
                        }
                        else if (angMove[i] > maxMagnitude) 
                        {
                                double totalMove = angMove[i] + linearMove[i];
                                angMove[i] = maxMagnitude;
                                linearMove[i] = totalMove - angMove[i];
                        }

           //calc the rotation required for move 
                        if (angMove[i]==0)
                        {
                                //no movement so clear 
                                v3ChangeInAngle[i].clear();
                        }
                        else
                        {
                                // the direction for rotation 
                                Vector3D targetDirection = m_v3RelContactWorldPosition[i].vectorProduct(m_v3NormalofContactWorld);

                                Matrix3 inverseInertiaTensor;
                                body[i] -> getInverseInertiaTensorWorld(&inverseInertiaTensor);
                                
                                //calc how to move in that direction 
                                v3ChangeInAngle[i] = inverseInertiaTensor.transform (targetDirection) * (angMove[i]/ angInertia[i]);
                        }

            //linear move along contact norm 
                        v3ChangeInLinear[i] = m_v3NormalofContactWorld * linearMove[i];

                        //apply linear movement 
                        Vector3D position;
                        body[i] -> getBodyPosition(&position);
                        position.addScaledVector(m_v3NormalofContactWorld, linearMove[i]);
                        body[i]->setBodyPosition(position);

                        //add change in orientation 
                        Quaternion q;
                        body[i] ->getBodyOrientation(&q);
                        q.calcOrientation(v3ChangeInAngle[i],1.0);
                        body[i] -> setBodyOrientationQ(q);

                        //calc any data for objects that are in statis 
                        if (body[i] -> getObjectStatis()) 
                        {
                                body[i]->calculateObjectData();
                        }
                }

        }

}
//---------------------------------------------------------------------------------------
// Contact Resolve Implementation
//---------------------------------------------------------------------------------------

/*create a new ResolveContact object with predefined iteration count and Omega values for
  velocity and position*/
ContactResolve::ContactResolve (unsigned wIterationCount, double m_dOmegaVelocity,double m_dOmegaDepth)

{
        determineIterations(wIterationCount,wIterationCount);
        setOmega(m_dOmegaVelocity, m_dOmegaDepth);
}

/*create the same object, just that it can accept optional iteration counts*/
ContactResolve::ContactResolve(unsigned m_wNumberOfIterationsForVelocity, unsigned m_wNumberOfIterationsForPosition,
                                                           double  m_dOmegaVelocity, double  m_dOmegaDepth)
{
        determineIterations(m_wNumberOfIterationsForVelocity);
        setOmega( m_dOmegaVelocity, m_dOmegaDepth);
}

/*determine the iterations for each stage of resolution*/
void ContactResolve::determineIterations(unsigned m_wNumberOfIterationsForVelocity,
                               unsigned m_wNumberOfIterationsForPosition)
{
        ContactResolve::m_wNumberOfIterationsForVelocity = m_wNumberOfIterationsForVelocity;
        ContactResolve::m_wNumberOfIterationsForPosition = m_wNumberOfIterationsForPosition;
}

void ContactResolve::determineIterations(unsigned wIterationCount)
{
        determineIterations(wIterationCount,wIterationCount);
}

/*set the Omega for both velocity and depth*/
void ContactResolve::setOmega(double m_dOmegaVelocity, double m_dOmegaDepth)
{
        ContactResolve::m_dOmegaVelocity = m_dOmegaVelocity;
        ContactResolve::m_dOmegaDepth = m_dOmegaDepth;
}

/*  the function that resolves the contacts for both depth and velocity.  */
void ContactResolve::resolveContact (Contact *contactArray,  unsigned m_wNumberOfContacts, double dElapsedFrameTime)
{
        //make sure we have to do something 
        if (m_wNumberOfContacts ==0)
        {
                return;
        }
        if(!settingsCorrect())
        { 
                return;
        }

        //prepare the contacts 
        contactPrep(contactArray, m_wNumberOfContacts,dElapsedFrameTime);

        //resolve any interpenetration problems 
        adjustContactDepth(contactArray, m_wNumberOfContacts,dElapsedFrameTime);

        //resolve any velocity problems
        adjustContactVelocity(contactArray, m_wNumberOfContacts,dElapsedFrameTime);

}

/* prepares the contacts for processing. */     
void ContactResolve::contactPrep(Contact *contactArray, unsigned m_wNumberOfContacts,double dElapsedFrameTime)
{ 
     //generate the needed velocity and contact info 
        Contact *lastContact = contactArray + m_wNumberOfContacts;
        
        for (Contact *contact = contactArray; contact < lastContact; contact++)
        {
                //calc the internal contact data 
                contact -> calculateInternals(dElapsedFrameTime);

        }
}

/*function that will adjust the velocitied that have constraints*/
void ContactResolve::adjustContactVelocity(Contact *contactArray,  unsigned m_wNumberOfContacts,double dElapsedFrameTime)
{
        Vector3D velChange[2], rotChange[2];
        Vector3D deltaVelocity;

        //resolve in order of severity 
        m_wNumberOfVelocityItsUsed = 0;

        while (m_wNumberOfVelocityItsUsed < m_wNumberOfIterationsForVelocity )
        {
                // find the contact with the max probable vel change 
                double max = m_dOmegaVelocity;
                unsigned index = m_wNumberOfContacts; 

                for (unsigned i = 0; i < m_wNumberOfContacts; i ++)
                {
                        if (contactArray[i].m_dCalcChangedVelocity > max)
                        {
                                max = contactArray[i].m_dCalcChangedVelocity;
                                index = i;
                        }
                }
                //max number of contacts reached end loop 
                if (index == m_wNumberOfContacts)
                {
                        break;
                }

                //match the awake state 
                contactArray[index].integrateAwakeState();

                //complete the resloution for the contact that was choosen 
                contactArray[index].applyChangeInVelocity(velChange,rotChange);

                //recompote vlosing velocities due to updates 
                for (unsigned i = 0; i < m_wNumberOfContacts; i ++)
                {
                        
                        //check the bodies in contact 
                        for(unsigned b = 0; b < 2; b++)
                        {
                                if (contactArray[i].body[b])
                                {
                                        // check for a match 
                                        for (unsigned d = 0; d < 2 ; d++)
                                        {
                                                if (contactArray[i].body[b] == contactArray[index].body[d])
                                                {
                                                        deltaVelocity = velChange[d] + rotChange[d].vectorProduct(contactArray[i].m_v3RelContactWorldPosition[b]);
                                                        
                                                        //change the the direction sign if dealing with body 2
                                                        contactArray[i].m_v3ContactClosingVelocity += 
                                                                contactArray[i].m_m3TransformContactToWorld.transformTranspose(deltaVelocity)
                                                                *(b?-1:1);  //first body or second body to know which sign

                                                        contactArray[i].calcChangeOfVelocity(dElapsedFrameTime);

                                                }
                                        }
                                }
                        }
                }
        
                m_wNumberOfVelocityItsUsed++;
        }

}

/*function that will adjust the depth for contacts that have constraints.*/

void ContactResolve::adjustContactDepth(Contact *contactArray,  unsigned m_wNumberOfContacts,double dElapsedFrameTime)
{
        unsigned i, index; // vars for incrementing 

        Vector3D linearChange[2], angularChange[2];
        double max;
        Vector3D changeInPosition;

        //resolve in order of severity 
        m_wNumberOfPositionItsUsed = 0;

        while(m_wNumberOfPositionItsUsed < m_wNumberOfIterationsForPosition)
        {
                //find the biggest depth 
                max = m_dOmegaDepth;
                index = m_wNumberOfContacts;
                
                for(i = 0; i <  m_wNumberOfContacts; i++)
                {
                        if(contactArray[i].m_dDepthofContact > max)
                        {
                                max = contactArray[i].m_dDepthofContact;
                                index = i;
                        }

                }
                //stop if index reaches number of contacts 
                if(index == m_wNumberOfContacts)
                {
                        break;
                }

                //awake state at the contact 
                contactArray[index].integrateAwakeState();

                //resolve the penetration 
                contactArray[index].applyChangeInPosition(linearChange, angularChange, max);

                //may have change the penetration of other bodies so update
                for (i = 0; i < m_wNumberOfContacts; i++)
                {
                        // check each body that is in contact 
                        for (unsigned b = 0; b < 2; b++)
                        {
                                if (contactArray[i].body[b])
                                {
                                        //check for  a match 
                                        for(unsigned d = 0; d < 2; d++)
                                        {
                                                if(contactArray[i].body[b] == contactArray[index].body[d])
                                                {
                                                        changeInPosition = linearChange[d] + 
                                                                angularChange[d].vectorProduct(contactArray[i].m_v3RelContactWorldPosition[b]);

                                                        //if dealling with secod body sign is positive so check
                                                        contactArray[i].m_dDepthofContact += 
                                                                changeInPosition.scalarProduct(contactArray[i].m_v3NormalofContactWorld)
                                                                *(b?1:-1);              

                                                }
                                        }
                                }
                        }
                }

                m_wNumberOfPositionItsUsed++;

        }

}

