/*****************************************************************************************
*                                                                                        *
*  Program Name:          force_generator.h                                              *
*  Name:                  Jennifer Wickensimer                                           *
*  Date:                  January 23, 2012                                               *
*  Description:           Include file that defines functions to apply physical forces   *
*                         to objects that have undergone a collision or are impacted by  *
*                         natural forces                                                 *
*                                                                                        *
*  Update:                Jennifer Wickensimer, 01/28/2012                               *
*                                                                                        *
*****************************************************************************************/
#ifndef PHYSICSENGINE_FORCE_GENERATOR_H_
#define PHYSICSENGINE_FORCE_GENERATOR_H_

//-------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------

#include "AdditionalCode.h"
#include "pfgen.h"
#include <vector>

namespace PhysicsEngine    
{
	// creates the forces to apply to objects
    class ForceGenerator
    {
    public:

	/* the following can be overloaded in order to calculate
	and update the forces applied to the PhysBody*/

	    virtual void updateForce(PhysBody *pBody, double dElapsedFrameTime) = 0;

    };


    class Gravity : public ForceGenerator
    {
	/* contains the acceleration of an object
	affected by gravity*/

	    Vector3D m_v3Gravity;

    public:

	/* create the generator with the given 
	acceleration*/

	    Gravity(const Vector3D &v3Gravity);

	/*now to apply the gravitational force to
	the selected PhysBody*/

	    virtual void updateForce(PhysBody *pBody, double dElapsedFrameTime);

    };

    /**
     * A force generator that applies a Spring force.
     */
    class Spring : public ForceGenerator
    {
        /**
         * The point of connection of the spring, in local
         * coordinates.
         */
        Vector3D connectionPoint;

        /**
         * The point of connection of the spring to the other object,
         * in that object's local coordinates.
         */
        Vector3D otherConnectionPoint;

        /** The particle at the other end of the spring. */
        PhysBody *other;

        /** Holds the sprint constant. */
        double springConstant;

        /** Holds the rest length of the spring. */
        double restLength;

    public:

        /** Creates a new spring with the given parameters. */
        Spring(const Vector3D &localConnectionPt,
               PhysBody *other,
               const Vector3D &otherConnectionPt,
               double springConstant,
               double restLength);

        /** Applies the spring force to the given rigid body. */
        virtual void updateForce(PhysBody *pBody, double dElapsedFrameTime);
    };

    /**
     * A force generator showing a three component explosion effect.
     * This force generator is intended to represent a single
     * explosion effect for multiple rigid bodies. The force generator
     * can also act as a particle force generator.
     */
    class Explosion : public ForceGenerator,
                      public ParticleForceGenerator
    {
        /**
         * Tracks how long the explosion has been in operation, used
         * for time-sensitive effects.
         */
        double timePassed;

    public:
        // Properties of the explosion, these are public because
        // there are so many and providing a suitable constructor
        // would be cumbersome:

        /**
         * The location of the detonation of the weapon.
         */
        Vector3D detonation;

        // ... Other Explosion code as before ...


        /**
         * The radius up to which objects implode in the first stage
         * of the explosion.
         */
        double implosionMaxRadius;

        /**
         * The radius within which objects don't feel the implosion
         * force. Objects near to the detonation aren't sucked in by
         * the air implosion.
         */
        double implosionMinRadius;

        /**
         * The length of time that objects spend imploding before the
         * concussion phase kicks in.
         */
        double implosionDuration;

        /**
         * The maximal force that the implosion can apply. This should
         * be relatively small to avoid the implosion pulling objects
         * through the detonation point and out the other side before
         * the concussion wave kicks in.
         */
        double implosionForce;

        /**
         * The speed that the shock wave is traveling, this is related
         * to the thickness below in the relationship:
         *
         * thickness >= speed * minimum frame duration
         */
        double shockwaveSpeed;

        /**
         * The shock wave applies its force over a range of distances,
         * this controls how thick. Faster waves require larger
         * thicknesses.
         */
        double shockwaveThickness;

        /**
         * This is the force that is applied at the very centre of the
         * concussion wave on an object that is stationary. Objects
         * that are in front or behind of the wavefront, or that are
         * already moving outwards, get proportionally less
         * force. Objects moving in towards the centre get
         * proportionally more force.
         */
         double peakConcussionForce;

         /**
          * The length of time that the concussion wave is active.
          * As the wave nears this, the forces it applies reduces.
          */
         double concussionDuration;

         /**
          * This is the peak force for stationary objects in
          * the centre of the convection chimney. Force calculations
          * for this value are the same as for peakConcussionForce.
          */
         double peakConvectionForce;

         /**
          * The radius of the chimney cylinder in the xz plane.
          */
         double chimneyRadius;

         /**
          * The maximum height of the chimney.
          */
         double chimneyHeight;

         /**
          * The length of time the convection chimney is active. Typically
          * this is the longest effect to be in operation, as the heat
          * from the explosion outlives the shock wave and implosion
          * itself.
          */
         double convectionDuration;

    public:
        /**
         * Creates a new explosion with sensible default values.
         */
        Explosion();

        /**
         * Calculates and applies the force that the explosion
         * has on the given rigid body.
         */
        virtual void updateForce(PhysBody * pBody, double dElapsedFrameTime);

        /**
         * Calculates and applies the force that the explosion has
         * on the given particle.
         */
        virtual void updateForce(Particle *particle, double dElapsedFrameTime) = 0;

    };

    /**
     * A force generator that applies an aerodynamic force.
     */
    class Aero : public ForceGenerator
    {
    protected:
        /**
         * Holds the aerodynamic tensor for the surface in body
         * space.
         */
        Matrix3 tensor;

        /**
         * Holds the relative position of the aerodynamic surface in
         * body coordinates.
         */
        Vector3D position;

        /**
         * Holds a pointer to a vector containing the windspeed of the
         * environment. This is easier than managing a separate
         * windspeed vector per generator and having to update it
         * manually as the wind changes.
         */
        const Vector3D* windspeed;

    public:
        /**
         * Creates a new aerodynamic force generator with the
         * given properties.
         */
        Aero(const Matrix3 &tensor, const Vector3D &position,
             const Vector3D *windspeed);

        /**
         * Applies the force to the given rigid body.
         */
        virtual void updateForce(PhysBody *pBody, double dElapsedFrameTime);

    protected:
        /**
         * Uses an explicit tensor matrix to update the force on
         * the given rigid body. This is exactly the same as for updateForce
         * only it takes an explicit tensor.
         */
        void updateForceFromTensor(PhysBody *pBody, double dElapsedFrameTime,
                                   const Matrix3 &tensor);
    };

    /**
    * A force generator with a control aerodynamic surface. This
    * requires three inertia tensors, for the two extremes and
    * 'resting' position of the control surface.  The latter tensor is
    * the one inherited from the base class, the two extremes are
    * defined in this class.
    */
    class AeroControl : public Aero
    {
    protected:
        /**
         * The aerodynamic tensor for the surface, when the control is at
         * its maximum value.
         */
        Matrix3 maxTensor;

        /**
         * The aerodynamic tensor for the surface, when the control is at
         * its minimum value.
         */
        Matrix3 minTensor;

        /**
        * The current position of the control for this surface. This
        * should range between -1 (in which case the minTensor value
        * is used), through 0 (where the base-class tensor value is
        * used) to +1 (where the maxTensor value is used).
        */
        double controlSetting;

    private:
        /**
         * Calculates the final aerodynamic tensor for the current
         * control setting.
         */
        Matrix3 getTensor();

    public:
        /**
         * Creates a new aerodynamic control surface with the given
         * properties.
         */
        AeroControl(const Matrix3 &base,
                    const Matrix3 &min, const Matrix3 &max,
                    const Vector3D &position, const Vector3D *windspeed);

        /**
         * Sets the control position of this control. This * should
        range between -1 (in which case the minTensor value is *
        used), through 0 (where the base-class tensor value is used) *
        to +1 (where the maxTensor value is used). Values outside that
        * range give undefined results.
        */
        void setControl(double value);

        /**
         * Applies the force to the given rigid body.
         */
        virtual void updateForce(PhysBody *pBody, double dElapsedFrameTime);
    };

    /**
     * A force generator with an aerodynamic surface that can be
     * re-oriented relative to its rigid body. This derives the
     */
    class AngledAero : public Aero
    {
        /**
         * Holds the orientation of the aerodynamic surface relative
         * to the rigid body to which it is attached.
         */
        Quaternion orientation;

    public:
        /**
         * Creates a new aerodynamic surface with the given properties.
         */
        AngledAero(const Matrix3 &tensor, const Vector3D &position,
             const Vector3D *windspeed);

        /**
         * Sets the relative orientation of the aerodynamic surface,
         * relative to the rigid body it is attached to. Note that
         * this doesn't affect the point of connection of the surface
         * to the body.
         */
        void setOrientation(const Quaternion &quat);

        /**
         * Applies the force to the given rigid body.
         */
        virtual void updateForce(PhysBody *pBody, double dElapsedFrameTime);
    };

    /**
     * A force generator to apply a buoyant force to a rigid body.
     */
    class Buoyancy : public ForceGenerator
    {
        /**
         * The maximum submersion depth of the object before
         * it generates its maximum buoyancy force.
         */
        double maxDepth;

        /**
         * The volume of the object.
         */
        double volume;

        /**
         * The height of the water plane above y=0. The plane will be
         * parallel to the XZ plane.
         */
        double waterHeight;

        /**
         * The density of the liquid. Pure water has a density of
         * 1000kg per cubic meter.
         */
        double liquidDensity;

        /**
         * The centre of buoyancy of the rigid body, in body coordinates.
         */
        Vector3D centreOfBuoyancy;

    public:

        /** Creates a new buoyancy force with the given parameters. */
        Buoyancy(const Vector3D &cOfB,
            double maxDepth, double volume, double waterHeight,
            double liquidDensity = 1000.0f);

        /**
         * Applies the force to the given rigid body.
         */
        virtual void updateForce(PhysBody *pBody, double dElapsedFrameTime);
    };

    /**
    * Holds all the force generators and the bodies they apply to.
    */


    class ForceRegistry
    {
    protected:

	/* the following will keep track of a single
	force and the body it is applicable to*/

	struct  ForceRegistration
	{
		PhysBody *pBody;
		ForceGenerator *pFg;
	};

	//Something to hold the list of registrations

	typedef std::vector<ForceRegistration> Registry;
	Registry registrations;

    public:

	/*function to register the force generator 
	to the body*/

	void addToRegistry(PhysBody* pBody, ForceGenerator *pFg);

	/*removes the body information from the registry.
	If nothing is registered, nothing occurs...
	Note: this is a wrapper for 
	the functionality provided in the Vector class*/

	void remove(PhysBody* pBody, ForceGenerator *pFg);

	/*empties the complete registry information.  Not the
	bodies, but the just the information of the connections between
	the bodies and the force generators  Note: this is a wrapper for 
	the functionality provided in the Vector class*/

	void clear();

	/*Causes all force generators to update the forces of the 
	bodies that they are connected to*/

	void updateForcesOnBodies(double dElapsedFrameTime);
};


} 

#endif