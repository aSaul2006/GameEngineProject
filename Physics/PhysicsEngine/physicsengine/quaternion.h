/***************************************************************************************
*  Program Name:    Quaternion.h                                                        *         
*  Name:            Jennifer Wickensimer                                               *
*  Date:            January 24, 2013                                                   *
*  Description:     Used to manipulate the orentation of the PhysBody.                 *                        
*                                                                                      *
*  Update:           name of author, date (mm/dd/yy)  separated by “;”                 *
*                   “ex.- Bryan Sweeney, 01/20/13; Mr. Jones, 01/20/13;”               *
***************************************************************************************/


#ifndef PHYSICSENGINE_QUATERNION_H_
#define PHYSICSENGINE_QUATERNION_H_

//------------------------------------------------------------------------------------
//Include Files
//------------------------------------------------------------------------------------

#include "AdditionalCode.h"
#include "vector3d.h"

namespace PhysicsEngine
{
//-----------------------------------------------------------------------------------
// Class Quaternion 
//-----------------------------------------------------------------------------------
class Quaternion 
{
public:
	union{
		struct{
			/* this represents the form :
										 |w|
			        w + xi + yj + zk =   |x|
										 |y|
										 |z|
			where w, x, y,and z are in vector format 
			*/ 

			//holds the scalar component in the quarternion
			double m_r;  
			
            // the next three variables hold the 
			//imaginary number componets of the quaternion
			double m_i;
			double m_j;
			double m_k;
		};

		// array that holds the quaternion data 
		double data[4];
	};

	// default constructor no (0)rotation
	Quaternion() : m_r(1), m_i(0), m_j(0), m_k(0) {}

	//constructor to create quaternion with given components 
	Quaternion(const double m_r, const double m_i, const double m_j, const double m_k)
		: m_r(m_r), m_i(m_i), m_j(m_j), m_k(m_k) 
	{
	}
//-------------------------------------------------------------------------------------
// Class Methods
//-------------------------------------------------------------------------------------

	//function for normilizing the quaternion to unit length 
	void normalize()
	{
		// magnitude = sqrt(w^2 +x^2 +y^2 +z^2)        
		double dMagnitude = m_r*m_r + m_i*m_i +m_j*m_j + m_k*m_k;

		//in case of zero length quaternion we need to use the no rotation quaternion 
		if (dMagnitude == 0){
			m_r = 1;
			return;
		}

		/* this line of code is done so that instead of dividing each component by 
		the magnitude  it could be multiplied instead */
		dMagnitude = ((double)1.0)/(sqrt(dMagnitude));
        
		m_r *= dMagnitude;
		m_i *= dMagnitude;
		m_j *= dMagnitude;
		m_k *= dMagnitude;

	}
    
	//used to multiply a quaternion by another given quaternion
	void operator *=(const Quaternion &multiplyBy)
	{ 
		Quaternion q = *this;
		
		// uses quaternion multiplication 
		m_r =   q.m_r * multiplyBy.m_r - q.m_i * multiplyBy.m_i
			  - q.m_j * multiplyBy.m_j - q.m_k * multiplyBy.m_k;

		m_i =   q.m_r * multiplyBy.m_i + q.m_i * multiplyBy.m_r
			  + q.m_j * multiplyBy.m_k - q.m_k * multiplyBy.m_j;

		m_j =   q.m_r * multiplyBy.m_j + q.m_j * multiplyBy.m_r
			  + q.m_k * multiplyBy.m_i - q.m_i * multiplyBy.m_k;

		m_k =   q.m_r * multiplyBy.m_k + q.m_k * multiplyBy.m_r
			  + q.m_i * multiplyBy.m_j - q.m_j * multiplyBy.m_i;
	}

     /* Adds the given vector to this, scaled by the given amount.
         * This is used to update the orientation quaternion by a rotation
         * and time.
         *
     *input: vector- The vector to add.
     *input: scale-  The amount of the vector to add.
      */
	void calcOrientation(const Vector3D& vector, double scale)
	{
		/* Takes the form:
		       q' = q + (delta t/2) *(w)*(q)

			   where t = time, 
			   w = angular velocity represented as a quaternion
			   q = the orginal quaternion
	    */  

		Quaternion q(0,
                vector.x * scale,
                vector.y * scale,
                vector.z * scale);
            q *= *this;
            m_r += q.m_r * ((double)0.5);
            m_i += q.m_i * ((double)0.5);
            m_j += q.m_j * ((double)0.5);
            m_k += q.m_k * ((double)0.5);
        }
	
   void rotateByVector (const Vector3D& vector)
   {
	   Quaternion q(0, vector.x, vector.y, vector.z);
	   (*this) *= q;
   }
};

}
#endif // _QUATERNION_H_
