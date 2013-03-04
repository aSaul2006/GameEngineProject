

/*****************************************************************************************
*                                                                                        *
*  Program Name:          contactresolution.h                                           *
*  Name:                  Jennifer Wickensimer                                           *
*  Date:                  January 23, 2012                                               *
*  Description:           include for defining the functionality to provide for          *
*                         the resolution of contacts                                     *
*  Update:                Jennifer Wickensimer January 29, 2012                          *
*                                                                                        *
*****************************************************************************************/

#ifndef PHYSICSENGINE_CONTACTRESOLUTION_H
#define PHYSICSENGINE_CONTACTRESOLUTION_H

#include "PhysBody.h"

namespace PhysicsEngine
{

class ContactResolve;

/*main class that defines what a contact is
and what data in contains*/

class Contact
{

        /*the ContactResolve will require access to
        the data within Contact in order to put/modify
        that data*/

        friend ContactResolve;

public:

        /*a container to hold the objects that are
        in contact.  One can be NULL in the case of
        an object being in contact with scenery*/

        PhysBody* body[2];

        /*since there is nothing that is truely
        frictionless, need a variable to hold the 
        coefficent of friction of the contact*/

        double m_dFriction;

        /*when two objects collide, there is calculation that is
        done to determine what difference the seperation velocity
        is compared to the closing velocity.  This is stored in the
        variable resitution, with 1 signifying that there is no 
        difference between the two velocities, and a 0 meaning that 
        there is no separation velocity*/

        double m_dRestitution;

        /*Position of the contact expresed in
        world coordinates*/

        Vector3D m_v3PointOfContactWorld;

        /*Contains the direction in world coordinates 
        of the contact*/

        Vector3D m_v3NormalofContactWorld;

        /*Contains the depth of the contact point.  If there
        are two bodies, the contact point's depth should be 
        half the distance of the points of each object that
        are in contact*/

        double m_dDepthofContact;

        /*function that sets the data that is independent
        of the objects posistion*/

        void setBodyData(PhysBody* one, PhysBody *two, double m_dFriction,
                             double m_dRestitution);


protected:
        //----------------------------------------------------------
        //Data members
        //----------------------------------------------------------

        /* A transform matrix that is used to convert to world
        coordinates */

        Matrix3  m_m3TransformContactToWorld;


        /*the velocity of the object approaching each 
        other*/

        Vector3D m_v3ContactClosingVelocity;


        /*contains the calculated seperation velocity
        with taking the m_dRestitution into account*/

        double m_dCalcChangedVelocity;


        /*contains the position in world space of the contact
        point in relation to the center of each body*/

        Vector3D  m_v3RelContactWorldPosition[2];

        //--------------------------------------------------------------
        //Methods
        //--------------------------------------------------------------

        /* function to kick of all the interna data.  Needs to be run 
        before the resolution function is run*/

        void calculateInternals (double dElapsedFrameTime);


        /* Contact reversal.  Switch the two PhysBody objects and reverse
        the normal.  Need to run calculateInternals again*/

        void switchBodies();


        /* if a body is asleep, wake it up if needed */
        
        void integrateAwakeState();
        
        
        /*calculates the change in velocity */

        void calcChangeOfVelocity(double dElapsedFrameTime);


        /* determine velocity on the body of its contact point*/

        Vector3D  calcVelocityLocal (unsigned bodyIndex, double dElapseFrameTime);

        /*determines an orthornormal basis for the contact point*/

        void calcBasisForContact();

        /*function to apply the change of momementum to the
        objects*/

        void applyMomentumChange (Vector3D &momentem, PhysBody *body,
                                                          Vector3D *v3ChangeInVelocity, Vector3D *v3ChangeInRotation);


        /*calculates the change in velocity taking inertia 
        into account*/

        void applyChangeInVelocity (Vector3D v3ChangeInVelocity[2],  Vector3D v3ChangeInRotation[2]);


        /*calculate the chage in postion taking into 
        account inertia */

        void applyChangeInPosition (Vector3D v3ChangeInLinear[2], Vector3D v3ChangeInAngle[2],
                                        double  d_DepthofContact);



        /*determine the momentum neded to resolve the contact without
        friction taken into account.  Uses the inertia tensors for
        each object*/

        Vector3D calculateMomentumWithoutFriction(Matrix3 *m3InverseInertiaTensor);


                /*determine the momentum neded to resolve the contact with
        friction taken into account.  Uses the inertia tensors for
        each object*/

        Vector3D calculateMomementIncludingFriction(Matrix3 *m3InverseInertiaTensor);

};

/* Below is the contact resolution routine

RATIONAL:

The algorithm iterates through each contact and attempts to resolve
it.  This may cause other contacts to become worse, but the algorithm
will revist the other contacts up to a limit set on the interatiuons.
Since this is done locally, it is quicker than a global method, thus
allowing more iterations in the same ammount of time.

Due to the multiple iterations, the algorithm has issues with stacked
bodies.  The iterations of calculations may cause the stacked objects
to vibrate visually, and in extreme cases cause an object to "slide
off" the stack. */

class ContactResolve
{
public:

        /*need something to track the number of iterations
        used for velocity*/

        unsigned m_wNumberOfVelocityItsUsed;

        /*same thing to track number of position iterations
        used */

        unsigned m_wNumberOfPositionItsUsed;

        /*create a new ResolveContact object with
        predefined iteration count and Omega values for
        velocity and position*/

        ContactResolve (unsigned wIterationCount,
                                        double   m_dOmegaVelocity = (double)0.01,
                                        double   m_dOmegaDepth = (double)0.01);


        /*create the same object, just that it can accept
        optional iteration counts*/

        ContactResolve (unsigned m_wNumberOfIterationsForVelocity,
                            unsigned m_wNumberOfIterationsForPosition,
                                        double   m_dOmegaVelocity=(double)0.01,
                                        double   m_dOmegaDepth= (double)0.01);




        
        /*Sanity check on data passed to resolve contact */

        bool settingsCorrect()
        {
                return (m_wNumberOfIterationsForVelocity > 0) &&
                           (m_wNumberOfIterationsForPosition > 0) &&
                           (m_dOmegaVelocity > 0.0f) &&
                           (m_dOmegaDepth > 0.0f);
        }


                /*determine the iterations for each stage of resolution*/

                void determineIterations(unsigned m_wNumberOfIterationsForVelocity,
                               unsigned m_wNumberOfIterationsForPosition);


                void determineIterations(unsigned wIterationCount);


                /*set the Omega for both velocity
                and depth*/

                void setOmega(double m_dOmegaVelocity, double m_dOmegaDepth);


                /*  the function that resolves the contacts for both depth 
                and velocity.  */

                void resolveContact (Contact *contactArray,  unsigned m_wNumberOfContacts,
                        double dElapsedFrameTime);




protected:

        /* the number of iterations used as described above. This
        setting may need to be modifed based upon the game or 
        simulation being run*/



        unsigned m_wNumberOfIterationsForVelocity;

        unsigned m_wNumberOfIterationsForPosition;



        /*issues may occur when objects have an extremely
        small value.  The following contains a value
        of what is considered to be 0 velocity.  May need
        to be modified based on game/simulation*/

        double m_dOmegaVelocity;

        
        
        
        /*as with velocity, small depth between object
        penetration may cause issues.  The below stores 
        a value that is considered to be no depth.*/

        double m_dOmegaDepth;
        
        
        
        /* prepares the contacts for processing. */
        
        void contactPrep(Contact *contactArray, unsigned m_wNumberOfContacts,
                double dElapsedFrameTime);


        /*function that will adjust the velocitied that have 
        constraints*/

        void adjustContactVelocity(Contact *contactArray,  unsigned m_wNumberOfContacts, double dElapsedFrameTime);

        /*function that will adjust the depth for contacts that
        have constraints.*/

        void adjustContactDepth(Contact *contactArray,  unsigned m_wNumberOfContacts, double dElapsedFrameTime);


private:

        /*Sanity check on whether the settings
        are correct*/

        bool m_bSettingsCorrect;


};

        
/*interface for contact generators*/
class  ContactGen
{
public:

        /* add contact to the contact array for the object */

        virtual unsigned addContact(Contact *contact, unsigned wLimit) const = 0;
};
}

#endif 

