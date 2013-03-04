/*****************************************************************************************
*                                                                                        *
*  Program Name:          CollisionDetect.cpp                                   *
*  Name:                  Jennifer Wickensimer                                           *
*  Date:                  January 23, 2012                                               *
*  Description:           include for defining the functionality to provide for          *
*                         collision detection                                            *
*  Update:                Jennifer Wickensimer Feb.1, 2012                          *
*                                                                                        *
*****************************************************************************************/


#include "physicsengine\CollisionDetect.h"
#include <assert.h>
#include <memory.h>
#include <cstdlib>
#include <cstdio>


using namespace PhysicsEngine;

void CollisionPrimitive::calculatePrimitiveInternals()
{
	m_m4PrimitiveTransform = pBody->getTransform() * m_m4PrimitiveOffset;
}

/*The below function deterimes whether there is an 
intersection between a sphere and the plain*/

bool QuickIntersectionTest::sphereAndHalfSpaceIntersection(
	const CollisionSphere &sphere,
	const CollisionPlane &plane)

{
	//first, determine how far away from the origin

	double dDistanceOfBall =
		plane.m_v3direction *
		sphere.getObjectAxis(3) -
		sphere.m_dSphereRadius;

	/*check for the Intersection   returns
	true if the distance of the ball is less 
	than or equal to the offest of the plain*/

	return dDistanceOfBall <= plane.m_dPrimitiveOffsetPlane;
}

/*The below function determines whether the is an 
intersection between two spheres, one and two */

bool QuickIntersectionTest::sphereAndSphereIntersection(
	const CollisionSphere &one,
	const CollisionSphere &two)
{
	/* calculate the vector that exists between
	the two collision spheres */

	Vector3D v3VectorMidline = one.getObjectAxis(3) - two.getObjectAxis(3);

	/*Check to see if there is space between
	the two spheres, true if there is no space,
	false if there is*/

	return v3VectorMidline.squareOfTheMagnitude() <
		(one.m_dSphereRadius + two.m_dSphereRadius) * (one.m_dSphereRadius+two.m_dSphereRadius);
}

/* the following function is used in the determination of 
where two boxes overlap by returning the axis of the box */

static inline double transToAxis(
	const CollisionBox &box,
	const Vector3D &axis)

{
	return
		box.m_v3HalfSizeBox.x * fabs(axis * box.getObjectAxis(0)) +
		box.m_v3HalfSizeBox.y * fabs(axis * box.getObjectAxis(1)) +
		box.m_v3HalfSizeBox.z * fabs(axis * box.getObjectAxis(2));
}

/* using the axis of the collision boxes returned by transToAxis(), 
the following function determins whether two boxes overlap.  boxCenter
contains the vector between the two boxes so that it only needs to be
calculated once */

static inline bool overlapOnBoxAxis(
	const CollisionBox &one,
	const CollisionBox &two,
	const Vector3D &axis,
	const Vector3D &boxCenter)

{
	/*put the first and second boxes onto
	the axis*/
	// Project the half-size of one onto axis

	double dOneBoxOnAxis = transToAxis(one, axis);
	double dTwoBoxOnAxis = transToAxis(two, axis);

    //project this on axis
	double dDistance = abs(boxCenter * axis);

	/* now we can determine whether or not the 
	two boxes overlap.. if the distance calculated is less
	than the sum of the two boxes on the axis, then they overlap
	*/

	return (dDistance < dOneBoxOnAxis + dTwoBoxOnAxis);
}


/*the following preprocessor defintion is only used within
the boxAndBoxIntersection and done to speed things up duet to having
to calculate the cross product and getting the correct information out
of the boxes*/

#define TEST_OVERLAP_BOX(axis) overlapOnBoxAxis(one, two, (axis), v3BoxCenter)

bool QuickIntersectionTest::boxAndBoxIntersection(
	const CollisionBox &one,
	const CollisionBox &two)

{
	/*first, find the vector between the centers
	of the two boxes*/

	Vector3D v3BoxCenter = two.getObjectAxis(3) - one.getObjectAxis(3);

	return (

		//Check on the first box axis
		TEST_OVERLAP_BOX(one.getObjectAxis(0)) &&
		TEST_OVERLAP_BOX(one.getObjectAxis(1)) &&
		TEST_OVERLAP_BOX(one.getObjectAxis(2)) &&

		//Check on the second box axis
		TEST_OVERLAP_BOX(two.getObjectAxis(0)) &&
		TEST_OVERLAP_BOX(two.getObjectAxis(1)) &&
		TEST_OVERLAP_BOX(two.getObjectAxis(2)) &&

		/*and finally check the cross product of the
		two boxes */

		TEST_OVERLAP_BOX(one.getObjectAxis(0) % two.getObjectAxis(0)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(0) % two.getObjectAxis(1)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(0) % two.getObjectAxis(2)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(1) % two.getObjectAxis(0)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(1) % two.getObjectAxis(1)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(1) % two.getObjectAxis(2)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(2) % two.getObjectAxis(0)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(2) % two.getObjectAxis(1)) &&
        TEST_OVERLAP_BOX(one.getObjectAxis(2) % two.getObjectAxis(2))
		);
}
#undef TEST_OVERLAP_BOX

/* the below function determine whether there is an intersection 
between a collision box and a plane*/

bool QuickIntersectionTest::boxAndHalfSpaceIntersection(
	const CollisionBox &box,
	const CollisionPlane &plane)

{
	/*find out a hypothetical radius from the box and 
	direct it toward the plane*/
	// Work out the projected radius of the box onto the plane direction

	double hypotheticalBoxRadius = transToAxis(box, plane.m_v3direction);

	//Calculate the distance from origin the box is
	// Find out how far the box is from the origin.(^*BS*^/^*Feb. 14, 2012^*)
	double dDistanceOfBox = 
		plane.m_v3direction *
		box.getObjectAxis(3) -
		hypotheticalBoxRadius;

	/* finally, check for the interection: 
	true of the distanceOfBox is less or equal
	to the primitiveOffset of the plane */

	return dDistanceOfBox <= plane.m_dPrimitiveOffsetPlane;
}


unsigned CollisionDetector::sphereAndTruePlane(
    const CollisionSphere &sphere,
    const CollisionPlane &plane,
    CollisionData *data
    )
{
    // Make sure we have contacts
    if (data->m_iContactsMax <= 0) return 0;

    // Cache the sphere position
    Vector3D position = sphere.getObjectAxis(3);

    // Find the distance from the plane
    double dCenterDistance = plane.m_v3direction * position - plane.m_dPrimitiveOffsetPlane;

    // Check if we're within radius
    if (dCenterDistance*dCenterDistance > sphere.m_dSphereRadius*sphere.m_dSphereRadius)
    {
        return 0;
    }

    // Check which side of the plane we're on
    Vector3D normal = plane.m_v3direction;
    double penetration = -dCenterDistance;
    if (dCenterDistance < 0)
    {
        normal *= -1;
        penetration = -penetration;
    }
    penetration += sphere.m_dSphereRadius;

    // Create the contact - it has a normal in the plane direction.
    Contact* contact = data->contacts;
    contact->m_v3NormalofContactWorld = normal;
    contact->m_dDepthofContact = penetration;
    contact->m_v3PointOfContactWorld = position - plane.m_v3direction * dCenterDistance;
    contact->setBodyData(sphere.pBody, NULL,
        data->m_dFriction, data->m_dRestitution);

    data->addContacts(1);
    return 1;
}

/* following function is the collision detection functionality for
a Sphere and normal plane */

unsigned CollisionDetector::sphereAndHalfSpace(
	const CollisionSphere &sphere,
	const CollisionPlane &plane,
	CollisionData *data)

{
		/* first we have to check to see if there are
	any contacts, or return*/

	if (data->m_iContactsMax <= 0) return 0;

	/*ok, there are contacts to process, store
	the sphere position */

	Vector3D v3SpherePosition = sphere.getObjectAxis(3);


	/* Calculate the distance the sphere is 
	from the plane */

	double dDistanceOfSphere = plane.m_v3direction * v3SpherePosition - 
		sphere.m_dSphereRadius - plane.m_dPrimitiveOffsetPlane;

	if (dDistanceOfSphere >=0) return 0;

	/* need to create the contact.
	Note: contact has a normal in the plane direction */
	
	//create the contact in the array
	Contact* contact = data->contacts;


	//populate the information

	contact->m_v3NormalofContactWorld = plane.m_v3direction;
	contact->m_dDepthofContact = -dDistanceOfSphere;
	contact->m_v3PointOfContactWorld = 
		v3SpherePosition - plane.m_v3direction * (dDistanceOfSphere + sphere.m_dSphereRadius);
	contact->setBodyData(sphere.pBody, NULL, data->m_dFriction, data->m_dRestitution);

	//add the information

	data->addContacts(1);
	
	return 1;
}


/* the below function is the collision detection 
function for a collision between two spheres one and two */

unsigned CollisionDetector::sphereAndSphere(
	const CollisionSphere &one,
	const CollisionSphere &two,
	CollisionData* data)

{
	//need to ensure that there are contacts to process

	if (data->m_iContactsMax <= 0) return 0;

	/* since there are contacts to process store 
	the spheres positions */

	Vector3D v3SphereOnePosition = one.getObjectAxis(3);
	Vector3D v3SphereTwoPosition = two.getObjectAxis(3);

	//calculate the vector between the two spheres

	Vector3D v3SphereMidline = 
		v3SphereOnePosition - v3SphereTwoPosition;
	double size = v3SphereMidline.magnitude();

	//check to see if the midline is long enough to continue

	if (size <= 0.0 || size >= one.m_dSphereRadius + two.m_dSphereRadius)
	{
		return 0;
	}

	/* need to create the normal manuualy, since we 
	have the size */

	Vector3D v3SphereNormal = v3SphereMidline * (((double)1.0)/size);

	//get ready to store the information in the array

	Contact* contact = data->contacts;

	//populate the data
	
	contact->m_v3NormalofContactWorld = v3SphereNormal;
	contact->m_v3PointOfContactWorld = v3SphereOnePosition + v3SphereMidline * (double)0.5;
	contact->m_dDepthofContact = (one.m_dSphereRadius + two.m_dSphereRadius - size);
	contact->setBodyData(one.pBody, two.pBody, data->m_dFriction, data->m_dRestitution);

	//and finally add the data to the array

	data->addContacts(1);

	return 1;
}

/*
 * This function checks if the two boxes overlap
 * along the given axis, returning the ammount of overlap.
 * The last input parameter of this Method, v3BoxCenter, is used 
 * to pass in the vector between the boxes center points, 
 * to avoid having to recalculate it each time.
 */    //(^*BS*^^*Feb.03,2013^*)

/* The below function determines whether two boxes are overlapping, 
and if so, returns the amount of overlap.  if the overlap is
negative, the objects are seperated*/

static inline double penetrationOnAxisBox(
	const CollisionBox &one,
	const CollisionBox &two,
	const Vector3D &axis,
	const Vector3D &boxCenter)

{
	//put the boxes on an axis // Project the half-size of one onto axis

	double dOneBoxProject = transToAxis(one, axis);
	double dTwoBoxProject = transToAxis(two, axis);

	//calculate the distance

	double dDistance = fabs(boxCenter * axis);

	//Return the overlap

	return dOneBoxProject + dTwoBoxProject - dDistance;
}


static inline bool tryAxisPenetration(
	const CollisionBox &one,
	const CollisionBox &two,
	Vector3D& axis,
	const Vector3D& boxCenter,
	unsigned index,
	// These values may be updated (^*BS*^^*Feb.14,2013*^)
	double& minPen,
	unsigned &minCase)

{
	// We make sure we have a normalized axis so we don't check almost parallel axes
	// (^*BS*^^*Feb.14,2013*^)
	//need to normalize the axis

	if (axis.squareOfTheMagnitude() < 0.0001) return true;

	axis.normalize();

	double dPent = penetrationOnAxisBox(one, two, axis, boxCenter);

	if (dPent < 0) return false;

	if (dPent < minPen) 
	{
		minPen = dPent;
		minCase = index;
	}
	
	return true;
}

/* the following function is called when it has been
determined that a vertex from box two is contacting
box one*/

void  fillPointBox(
	const CollisionBox &one,
	const CollisionBox &two,
	const Vector3D &boxCenter,
	CollisionData *data,
	unsigned bestFound,
	double dPenTemp)

{
    // This method is called when we know that a vertex from
    // box two is in contact with box one. (^*BS*^^*Feb.14,2013*^)

	Contact* contact = data->contacts;

	/*the collision is on the axis stored in bestFound,
	now need to determine what faces are on the axis */

	Vector3D v3Normal = one.getObjectAxis(bestFound);
	if (one.getObjectAxis(bestFound) * boxCenter >0)
	{
		v3Normal = v3Normal * -1.0f;
	}

	//determine what vertex of box two is in the collision

	Vector3D v3VertexUsed = two.m_v3HalfSizeBox;

	if (two.getObjectAxis(0) * v3Normal < 0) v3VertexUsed.x = -v3VertexUsed.x;
	if (two.getObjectAxis(1) * v3Normal < 0) v3VertexUsed.y = -v3VertexUsed.y;
	if (two.getObjectAxis(2) * v3Normal < 0) v3VertexUsed.z = -v3VertexUsed.z;

	//create data for the contact

	contact->m_v3NormalofContactWorld = v3Normal;
	contact->m_dDepthofContact = dPenTemp;
	contact->m_v3PointOfContactWorld == two.getTransform() * v3VertexUsed;
	contact->setBodyData(one.pBody, two.pBody, data->m_dFriction, data->m_dRestitution);
}

// There be Dragons here.


static inline Vector3D contactPointDetermine(
const Vector3D &pOne,
    const Vector3D &dOne,
    double oneSize,
    const Vector3D &pTwo,
    const Vector3D &dTwo,
    double twoSize,

    // If this is true, and the contact point is outside
    // the edge (in the case of an edge-face contact) then
    // we use one's midpoint, otherwise we use two's.
    bool useOne)
{
    Vector3D toSt, cOne, cTwo;
    double dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    double denom, mua, mub;

    smOne = dOne.squareOfTheMagnitude();
    smTwo = dTwo.squareOfTheMagnitude();
    dpOneTwo = dTwo * dOne;

    toSt = pOne - pTwo;
    dpStaOne = dOne * toSt;
    dpStaTwo = dTwo * toSt;

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    // Zero denominator indicates parrallel lines
    if (abs(denom) < 0.0001f) 
	{
        return useOne?pOne:pTwo;
    }

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne)/denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne)/denom;

    // If either of the edges has the nearest point out
    // of bounds, then the edges aren't crossed, we have
    // an edge-face contact. Our point is on the edge, which
    // we know from the useOne parameter.
    if (mua > oneSize ||
        mua < -oneSize ||
        mub > twoSize ||
        mub < -twoSize)
    {
        return useOne?pOne:pTwo;
    }
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;

        return cOne * 0.5 + cTwo * 0.5;
    }
}

/* the following preprocessor is used in the
box and box contaction generation method  */

#define CHECK_BOX_OVERLAP(axis, index) \
	if (!tryAxisPenetration(one, two, (axis), v3BoxCenter, (index), dPenIck, wBestFound)) return 0;




unsigned CollisionDetector::boxAndBox(
	const CollisionBox &one,
	const CollisionBox &two,
	CollisionData *data)

{
	//quick exit if no intersection exists

	if (!QuickIntersectionTest::boxAndBoxIntersection(one, two)) return 0;

	/*there is an intersection between box, so continu and 
	start by finding the vectors between the center of the two
	boxes */

	Vector3D v3BoxCenter = two.getObjectAxis(3) - one.getObjectAxis(3);

	// set the logic to state there is no contact
	
	double dPenIck = DBL_MAX;

	/* Since the QuickIntersectionTest has shown that the two boxes are intersecting one
	another, it is highly improbably (but not impossible) that there is not a collision. 
	The below code will find the collision, in the off chance that no collision is found, the
	rules of physics have somehow been irrevocably broken in a manner unknown.  The use of the
	assertion is based upon a exception being used to catch things that are known, assertions
	are used to catch things that should never occur
	*/
	unsigned wBestFound = 0xffffff;

	/* check each axis to determine whether ther
	is a seperating axis.  We also need to keep 
	track of the minPen */

	CHECK_BOX_OVERLAP(one.getObjectAxis(0), 0);
	CHECK_BOX_OVERLAP(one.getObjectAxis(1), 1);
	CHECK_BOX_OVERLAP(one.getObjectAxis(2), 2);
	CHECK_BOX_OVERLAP(two.getObjectAxis(0), 3);
	CHECK_BOX_OVERLAP(two.getObjectAxis(1), 4);
	CHECK_BOX_OVERLAP(two.getObjectAxis(2), 5);

	//store the best axis found.
	unsigned wBestSingleAxisFound = wBestFound;

	//continue with finding where collisions can occur
	CHECK_BOX_OVERLAP(one.getObjectAxis(0) % two.getObjectAxis(0), 6);
	CHECK_BOX_OVERLAP(one.getObjectAxis(0) % two.getObjectAxis(1), 7);
	CHECK_BOX_OVERLAP(one.getObjectAxis(0) % two.getObjectAxis(2), 8);
	CHECK_BOX_OVERLAP(one.getObjectAxis(1) % two.getObjectAxis(0), 9);
	CHECK_BOX_OVERLAP(one.getObjectAxis(1) % two.getObjectAxis(1), 10);
	CHECK_BOX_OVERLAP(one.getObjectAxis(1) % two.getObjectAxis(2), 11);
	CHECK_BOX_OVERLAP(one.getObjectAxis(2) % two.getObjectAxis(0), 12);
	CHECK_BOX_OVERLAP(one.getObjectAxis(2) % two.getObjectAxis(1), 13);
	CHECK_BOX_OVERLAP(one.getObjectAxis(2) % two.getObjectAxis(2), 14);

	//need to check for a result

	//now to ensure that the physically improbable remains that way

	assert(wBestFound != 0xffffff);

	/*we have collision..   The axis with minPen has been found
	and that will impact how we determine the colision point*/

	if (wBestFound < 3)
	{
		//vertex box one on face box two
		fillPointBox(one, two, v3BoxCenter*-1.0f, data, wBestFound, dPenIck);
		data->addContacts(1);
		return 1;
	}
	else if (wBestFound < 6)
	{
		//vertex box two on box one.. just switch the data in function
		fillPointBox(two, one, v3BoxCenter*-1.0f, data, wBestFound-3, dPenIck);
		data->addContacts(1);
		return 1;
	}
	else
	{
		//we have the edge to edge contact.. determine what axis
		wBestFound -= 6;
		unsigned wOneBoxAxisIndex = wBestFound / 3;
		unsigned wTwoBoxAxisIndex = wBestFound % 3;
		Vector3D v3OneBoxAxis = one.getObjectAxis(wOneBoxAxisIndex);
		Vector3D v3TwoBoxAxis = two.getObjectAxis(wTwoBoxAxisIndex);
		Vector3D v3Axis = v3OneBoxAxis % v3TwoBoxAxis;
		v3Axis.normalize();

		//need to have the axxese point from box one to two

		if (v3Axis * v3BoxCenter >0) v3Axis = v3Axis * -1.0f;

		//have the axis, need to find the edges.

		Vector3D v3PointOnOneEdge = one.m_v3HalfSizeBox;
		Vector3D v3PointOnTwoEdge = two.m_v3HalfSizeBox;

		for (unsigned i = 0; i < 3; i++)
		{
			if (i == wOneBoxAxisIndex) v3PointOnOneEdge[i] = 0;

			else if (one.getObjectAxis(i) * v3Axis > 0) v3PointOnOneEdge[i]= -v3PointOnOneEdge[i];

			if (i == wTwoBoxAxisIndex) v3PointOnTwoEdge[i] = 0;

			else if (two.getObjectAxis(i) * v3Axis < 0) v3PointOnTwoEdge[i] = -v3PointOnTwoEdge[i];
		}

		//now to move them to the wolrd coordinates

		v3PointOnOneEdge = one.m_m4PrimitiveTransform * v3PointOnOneEdge;
		v3PointOnTwoEdge = two.m_m4PrimitiveTransform * v3PointOnTwoEdge;

		/*now need is a direction/point for the edges that are in 
		collision.  Find out the closest approach point for the 
		two segments */

		Vector3D v3VertexFound = contactPointDetermine(
			v3PointOnOneEdge, v3OneBoxAxis, one.m_v3HalfSizeBox[wOneBoxAxisIndex],
			v3PointOnTwoEdge, v3TwoBoxAxis, two.m_v3HalfSizeBox[wTwoBoxAxisIndex],
			wBestSingleAxisFound > 2);

		//we can finall fill out the contact
		
		Contact* contact = data->contacts;
		contact->m_dDepthofContact = dPenIck;
		contact->m_v3NormalofContactWorld = v3Axis;
		contact->setBodyData(one.pBody, two.pBody, data->m_dFriction, data->m_dRestitution);
		data->addContacts(1);
		return 1;
	}
	return 0;
}

#undef CHECK_BOX_OVERLAP



/* the below function is the collision detection routine
for collisions between a box and point */




unsigned CollisionDetector::boxAndPoint(
	const CollisionBox &box,
	const Vector3D &point,
	CollisionData *data)

{
	//switch the pint into box coordinates

	Vector3D v3RelativePoint = box.m_m4PrimitiveTransform.transformInverse(point);

	Vector3D v3Normal;


	double dMinimum_depth = box.m_v3HalfSizeBox.x - fabs(v3RelativePoint.x);
	if (dMinimum_depth < 0) return 0;
	v3Normal = box.getObjectAxis(0) * ((v3RelativePoint.x < 0)?-1:1);

	double dActual_depth = box.m_v3HalfSizeBox.y - fabs(v3RelativePoint.y);
	if (dActual_depth < 0) return 0;

	else if (dActual_depth < dMinimum_depth)
	{
		dMinimum_depth = dActual_depth;
		v3Normal = box.getObjectAxis(1) * ((v3RelativePoint.y < 0)?-1:1);
	}

	dActual_depth = box.m_v3HalfSizeBox.z - abs(v3RelativePoint.z);
	if (dActual_depth < 0) return 0;

	if (dActual_depth < dMinimum_depth)
	{
		dMinimum_depth = dActual_depth;
		v3Normal = box.getObjectAxis(2) * ((v3RelativePoint.z <0)?-1:1);
	}

	//create the contact

	Contact* contact = data->contacts;
	contact->m_v3NormalofContactWorld = v3Normal;
	contact->m_v3PointOfContactWorld = point;
	contact->m_dDepthofContact = dMinimum_depth;

	contact->setBodyData(box.pBody, NULL, data->m_dFriction, data->m_dRestitution);

	data->addContacts(1);
	return 1;
}


/* the below is the Collision Detector for collisions between
a box and sphere*/

unsigned CollisionDetector::boxAndSphere(
	const CollisionBox &box,
	const CollisionSphere &sphere,
	CollisionData *data)

{
	//need to get the center of the sphere into box coordinates

	Vector3D v3CenterSphere = sphere.getObjectAxis(3);
	Vector3D v3RelativeCenterSphere = box.m_m4PrimitiveTransform.transformInverse(v3CenterSphere);

	/*check to see if we can exit out of
	the function early since there is not
	a possiblity for collision to occur */

	if (fabs(v3RelativeCenterSphere.x) - sphere.m_dSphereRadius > box.m_v3HalfSizeBox.x ||
		fabs(v3RelativeCenterSphere.y) - sphere.m_dSphereRadius > box.m_v3HalfSizeBox.y ||
		fabs(v3RelativeCenterSphere.z) - sphere.m_dSphereRadius > box.m_v3HalfSizeBox.z)
	{
		return 0;
	}

	Vector3D v3ClosestPoint(0,0,0);
	double dDistance;

	//need to clamp each coordinate to those of the box
	
	//first the X coordinate

	dDistance = v3RelativeCenterSphere.x;
	if (dDistance > box.m_v3HalfSizeBox.x) dDistance = box.m_v3HalfSizeBox.x;
	if (dDistance < -box.m_v3HalfSizeBox.x) dDistance = -box.m_v3HalfSizeBox.x;
	v3ClosestPoint.x = dDistance;

	//now the Y coordinate

	dDistance = v3RelativeCenterSphere.y;
	if (dDistance > box.m_v3HalfSizeBox.y) dDistance = box.m_v3HalfSizeBox.y;
	if (dDistance < -box.m_v3HalfSizeBox.y) dDistance = -box.m_v3HalfSizeBox.y;
	v3ClosestPoint.y = dDistance;

	//finally the z coordinate

	dDistance = v3RelativeCenterSphere.z;
	if (dDistance > box.m_v3HalfSizeBox.z) dDistance = box.m_v3HalfSizeBox.z;
	if (dDistance < -box.m_v3HalfSizeBox.z) dDistance = -box.m_v3HalfSizeBox.z;
	v3ClosestPoint.z = dDistance;

	//check to see if there is contact

	dDistance = (v3ClosestPoint - v3RelativeCenterSphere).squareOfTheMagnitude();
	if (dDistance > sphere.m_dSphereRadius * sphere.m_dSphereRadius) return 0;

	/* there is a collision, so populate the contact, first
	the contact must be in world coords*/

	Vector3D v3ClosestPointWorld = box.m_m4PrimitiveTransform.transform(v3ClosestPoint);

	//and now populate the contact as normal

    Contact* contact = data->contacts;
    contact->m_v3NormalofContactWorld = (v3ClosestPointWorld - v3CenterSphere);
    contact->m_v3NormalofContactWorld.normalize();
    contact->m_v3PointOfContactWorld = v3ClosestPointWorld;
    contact->m_dDepthofContact= sphere.m_dSphereRadius - sqrt(dDistance);
    contact->setBodyData(box.pBody, sphere.pBody,
        data->m_dFriction, data->m_dRestitution);

    data->addContacts(1);
    return 1;
}

/* finally the collision detection for 
box and plane */

unsigned CollisionDetector::boxAndHalfSpace(
	const CollisionBox &box,
	const CollisionPlane &plane,
	CollisionData *data)

{
	//make sure contacts exist

	if (data->m_iContactsMax <= 0) return 0;

	/*Check for intersection to exit out if
	there is no possiblity of collision (i.e. allow
	the program to be lazy) */

	if (!QuickIntersectionTest::boxAndHalfSpaceIntersection(box, plane))
	{
		return 0;
	}

	/* OK, an intersection exists so the collision has to be determined
	and the contacts populated.  Do need to do much except for checking 
	contact between vertices.  If it is an edge in contact with the plain, 
	there will be two vertices, if the box is resting on its face, there 
	will be four vertices */

	//cycle each combination of postive an negative for the halfsize box

	static double multiples[8][3] = {{1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                               {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1}};

	Contact* contact = data->contacts;

	unsigned wContactsChecked = 0;

	for (unsigned i =0; i <8; i++)
	{
		// calculate vertex position

		Vector3D v3VertexPosition(multiples[i][0], multiples [i][1],
			multiples [i][2]);
		v3VertexPosition.componentProduct(box.m_v3HalfSizeBox);
		v3VertexPosition = box.m_m4PrimitiveTransform.transform(v3VertexPosition);

		//calculate distance from vertex to plane

		double dVertexDistance = v3VertexPosition * plane.m_v3direction;

		//compare to the plane's primitive.Offset

		if (dVertexDistance <= plane.m_dPrimitiveOffsetPlane)
		{
			/*create the contact data since there is a collision with 
			one issue:  the contact is half the distance from the 
			vertex and plane*/ // we multiply the direction by half the separation
            // distance and add the vertex location. (^*BS*^^*Feb.14,2013*^)

			contact->m_v3PointOfContactWorld = plane.m_v3direction;
			contact->m_v3PointOfContactWorld *= (dVertexDistance-plane.m_dPrimitiveOffsetPlane);
			contact->m_v3PointOfContactWorld = v3VertexPosition;
			contact->m_dDepthofContact = plane.m_dPrimitiveOffsetPlane - dVertexDistance;

			//commit the data
			contact->setBodyData(box.pBody, NULL, data->m_dFriction, data->m_dRestitution);

			//move to the next one
			contact++;
			wContactsChecked++;
			if (wContactsChecked == data->m_iContactsMax) return wContactsChecked;
		}
	}
		data->addContacts(wContactsChecked);
		return wContactsChecked;
}