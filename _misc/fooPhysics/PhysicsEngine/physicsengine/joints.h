/***************************************************************************************
*  Program Name:    joints.h                                                           *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Interface file for joints between Physics bodies.                  *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Interface file for joints between Physics bodies.
 */

/**
  * This file contains the definitions for joints that link together
 * different Physics bodies.
 */


#ifndef PHYSICSENGINE_JOINTS_H
#define PHYSICSENGINE_JOINTS_H



#include "AdditionalCode.h"


namespace PhysicsEngine {

    /**
     * Joints link together two rigid bodies and make sure they do not
     * separate.  In a general phyiscs engine there may be many
     * different types of joint, that reduce the number of relative
     * degrees of freedom between two objects. This joint is a common
     * position joint: each object has a location (given in
     * body-coordinates) that will be kept at the same point in the
     * simulation.
     */
    class Joint : public ContactGen
    {
    public:
        /**
         * Holds the two rigid bodies that are connected by this joint.
         */
        PhysBody* body[2];

        /**
         * Holds the relative location of the connection for each
         * body, given in local coordinates.
         */
        Vector3D position[2];

        /**
         * Holds the maximum displacement at the joint before the
         * joint is considered to be violated. This is normally a
         * small, epsilon value.  It can be larger, however, in which
         * case the joint will behave as if an inelastic cable joined
         * the bodies at their joint locations.
         */
        double error;

        /**
         * Configures the joint in one go.
         */
        void set(
            PhysBody *a, const Vector3D& a_pos,
            PhysBody *b, const Vector3D& b_pos,
            double error
            );

        /**
         * Generates the contacts required to restore the joint if it
         * has been violated.
         */
        unsigned addContact(Contact *contact, unsigned limit) const;
    };

}

#endif 