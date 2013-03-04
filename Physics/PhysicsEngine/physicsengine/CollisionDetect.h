/*****************************************************************************************
*                                                                                        *
*  Program Name:          CollisionDetect.cpp                                            *
*  Name:                  Jennifer Wickensimer                                           *
*  Date:                  January 23, 2012                                               *
*  Description:           include for defining the functionality to provide for          *
*                         collision detection                                            *
*  Update:                Feb.1,2013                                                     *
*                                                                                        *
*****************************************************************************************/


#ifndef PHYSICSENGINE_COLLISIONDETECT_H_
#define PHYSICSENGINE_COLLISIONDETECT_H_

//--------------   Includes  -----------------
#include "ContactResolution.h"


namespace PhysicsEngine  {

    // Forward declarations of primitive friends
    class QuickIntersectionTests;
    class CollisionDetector;

    //Need to have a primative to test collisions against.
    class CollisionPrimitive
    {
    public:
        
        friend QuickIntersectionTests;
        friend CollisionDetector;

      
        //The PhysBody that is represented by this primitive.
         
		PhysBody *pBody;

       
        //The offset of this primitive from the given rigid body.
         
        Matrix4 m_m4PrimitiveOffset;

        //Calculate the primative internals
        
		void calculatePrimitiveInternals();

        //easy way to get access to the axis
        Vector3D getObjectAxis(unsigned index) const
        {
            return m_m4PrimitiveTransform.GetAxisVector(index);
        }

		/* returns the transform of the primitive,  this is derived
		from the primitive's offest and the transform of the
		PhysBody assigned the primitive */

        const Matrix4& getTransform() const
        {
			return m_m4PrimitiveTransform;
        }


    protected:
		
		/* Caluclate the transform of the primitive by using the
		offest of the primitive and the transform of the PhysBody */

        Matrix4 m_m4PrimitiveTransform;
    };
	

/* defines a sphere as a primitive to be used
within collision detection */


   class CollisionSphere : public CollisionPrimitive
   {
    public:

	/*a sphere needs a radius in order
	to be created*/

	    double m_dSphereRadius;
    };

/* need to have a plane for the world geometery
so that objects do not fall through the floor, go through
walls, etc.  The plane is not a primitive and will not be assigned
as part of CollisionPrimitive*/


    class CollisionPlane 
    {
    public:

	/* the plain itself
	*/

	    Vector3D m_v3direction;

	/*and again, we need to figure the distance of the 
	collision plain from the object that it is assigned to*/

	    double m_dPrimitiveOffsetPlane;
    };


/*Collisions can also use boxes as the collision
boundery.  This is a collision primitive and is
assigned as such*/
 
    class CollisionBox : public CollisionPrimitive
    {
    public:

	/*the two half size of the box
	*/

	    Vector3D m_v3HalfSizeBox;
    };




 /* the following is a class that can be used to perform
quick intersection tests.  
Uses:
	coarse collisions
	quick exit in fine collision determination */



   class QuickIntersectionTest
    {
    public:

	/*there are four types of tests defined:
		Sphere and half space
		Sphere and sphere
		Box and box
		Box and halfspace  
		
	each of these methods are boolean in order to determine
	whether the intersection exists*/

	static bool sphereAndHalfSpaceIntersection (
		const CollisionSphere &sphere,
		const CollisionPlane &plane);

	static bool sphereAndSphereIntersection (
		const CollisionSphere &one,
		const CollisionSphere &two);

	static bool boxAndBoxIntersection (
		const CollisionBox &one,
		const CollisionBox &two);

	static bool boxAndHalfSpaceIntersection (
		const CollisionBox &box,
		const CollisionPlane &plane);
    };



/* The following is used to build the contact data that occurs during 
the collision. */


    struct CollisionData
    {
	/*The base part of the collision data is in an array.  A pointer is used
	so that it can be incrememted each time a new contact is found.  An additional 
	pointer is used to indicate the first contact found*/

	    Contact *contactArray;

	/* and now the pointer for incrementing the pointer
	for new found contacts*/

	    Contact *contacts;

	/*maximum number of contacts that 
	the array can hold (contactsLeft)*/

	    int m_iContactsMax;


	/*contains the number of contacts
	that have been found(contactCount)*/

	    unsigned wNumberOfContacts;

	/*contains the friction force for 
	the collision*/

	    double m_dFriction;

	/*the speed that which the objects will seperate after
	the collision occurs is called restitution */

	    double m_dRestitution;

	/*Contains the tolerance for the collision, if objects
	become exteremly close, they may require a collision*/

	    double dCollisionTolerance;

	/*need to check if there are any more contacts available 
	in the contact array */

	    bool hasAnyMoreContacts()
	    {
		    return m_iContactsMax > 0;
	    }

	/* clears out the contact information,
	basically a reset*/

	    void resetContacts (unsigned maxContacts)
	   {
		    m_iContactsMax = maxContacts;
		    wNumberOfContacts = 0;
		    contacts = contactArray;
	   }

	/*Notification that the annumber of contacts
	have be added */

	   void addContacts (unsigned count)
	   {
		//need to reduce the number of contacts that can be used
		    m_iContactsMax -= count;
		    wNumberOfContacts += count;

		//move the array up

		    contacts += count;
	   }
    };

/* the wrapper for the fine grained detection control.  In order 
to speed things up, the program can use the QuickInteresection to 
determine whether the fine collision needs to be executed.

For simplicity sake, each function is formated the same.  Two objects
and the pointer to an array to fill with contact information.  The
return is the number of contacts that were stored within the array  */

    class CollisionDetector
    {
    public:

        static unsigned sphereAndHalfSpace(
            const CollisionSphere &sphere,
            const CollisionPlane &plane,
            CollisionData *data
            );

         static unsigned sphereAndTruePlane(
            const CollisionSphere &sphere,
            const CollisionPlane &plane,
            CollisionData *data
            );   

        static unsigned sphereAndSphere(
            const CollisionSphere &one,
            const CollisionSphere &two,
            CollisionData *data
            );

        /**
         * Does a collision test on a collision box and a plane representing
         * a half-space (i.e. the normal of the plane
         * points out of the half-space).
         */
        static unsigned boxAndHalfSpace(
            const CollisionBox &box,
            const CollisionPlane &plane,
            CollisionData *data
            );

        static unsigned boxAndBox(
            const CollisionBox &one,
            const CollisionBox &two,
            CollisionData *data
            );

        static unsigned boxAndPoint(
            const CollisionBox &box,
            const Vector3D &point,
            CollisionData *data
            );

        static unsigned boxAndSphere(
            const CollisionBox &box,
            const CollisionSphere &sphere,
            CollisionData *data
            );
    };
}

#endif 