/***************************************************************************************
*  Program Name:    Matrix4.h                                                          *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            January 26, 2013                                                   *
*  Description:     This program fully functional Matrix4 class for the Physics Core   *
*                   using double type precision                                        *
*  Update:           name of author, date (mm/dd/yy)  separated by “;”                 *
*                   “ex.- Bryan Sweeney, 01/20/13; Mr. Jones, 01/20/13;”               *
***************************************************************************************/



#ifndef PHYSICSENGINE_MATRIX4_H_
#define PHYSICSENGINE_MATRIX4_H_

//-------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------

#include "AdditionalCode.h"
#include "quaternion.h"

namespace PhysicsEngine
{

class Matrix4
{
public:
	
	double data[12];  // Holds the transform matrix data in array form.

    // Creates an identity matrix.
        
    Matrix4()
	{
		data[1] = data[2] = data[3] = data[4] = data[6] = 
			data[7] = data[8] = data[9] = data[11] = 0;
        data[0] = data[5] = data[10] = 1;
    }

    // Sets the matrix to be a diagonal matrix with the given coefficients.
         
    void SetDiagonal(double a, double b, double c)
    {
		data[0] = a;
        data[5] = b;
        data[10] = c;
    }

    // Returns a matrix which is this matrix multiplied by the given other matrix.
    // o = other     
    Matrix4 operator*(const Matrix4 &o) const
    {
		Matrix4 result;
        result.data[0] = (o.data[0]*data[0]) + (o.data[4]*data[1]) + (o.data[8]*data[2]);
        result.data[4] = (o.data[0]*data[4]) + (o.data[4]*data[5]) + (o.data[8]*data[6]);
        result.data[8] = (o.data[0]*data[8]) + (o.data[4]*data[9]) + (o.data[8]*data[10]);

        result.data[1] = (o.data[1]*data[0]) + (o.data[5]*data[1]) + (o.data[9]*data[2]);
        result.data[5] = (o.data[1]*data[4]) + (o.data[5]*data[5]) + (o.data[9]*data[6]);
        result.data[9] = (o.data[1]*data[8]) + (o.data[5]*data[9]) + (o.data[9]*data[10]);

        result.data[2] = (o.data[2]*data[0]) + (o.data[6]*data[1]) + (o.data[10]*data[2]);
        result.data[6] = (o.data[2]*data[4]) + (o.data[6]*data[5]) + (o.data[10]*data[6]);
        result.data[10] = (o.data[2]*data[8]) + (o.data[6]*data[9]) + (o.data[10]*data[10]);

        result.data[3] = (o.data[3]*data[0]) + (o.data[7]*data[1]) + (o.data[11]*data[2]) + data[3];
        result.data[7] = (o.data[3]*data[4]) + (o.data[7]*data[5]) + (o.data[11]*data[6]) + data[7];
        result.data[11] = (o.data[3]*data[8]) + (o.data[7]*data[9]) + (o.data[11]*data[10]) + data[11];

        return result;
    }

    // Transform the given vector by this matrix.
    // input: vector- The vector to transform.
        
    Vector3D operator*(const Vector3D &vector) const
    {
		return Vector3D(
			vector.x * data[0] +
            vector.y * data[1] +
            vector.z * data[2] + data[3],

            vector.x * data[4] +
            vector.y * data[5] +
            vector.z * data[6] + data[7],

            vector.x * data[8] +
            vector.y * data[9] +
            vector.z * data[10] + data[11]
		);
    }

    // Transform the given vector by this matrix.
    // input:  vector- The vector to transform.
        
    Vector3D transform(const Vector3D &vector) const
    {
		return (*this) * vector;
    }

    /**
         * Returns the determinant of the matrix.
         */
        double GetDeterminant() const;

		/**
         * Sets the matrix to be the inverse of the given matrix.
         *
         * @param m The matrix to invert and use to set this.
         */
        void setInverse(const Matrix4 &m);

    // Returns a new matrix containing the inverse of this matrix. 
        
	Matrix4 Inverse() const
    {
		Matrix4 result;
        result.setInverse(*this);
        return result;
    }

    // Inverts the matrix.
         
    void Invert()
    {
         setInverse(*this);
    }

     // Transform the given direction vector by this matrix.
     // *note* -  When a direction is converted between frames of reference, 
	 //           there is no translation required.
     // input:  vector- The vector to transform.
     
    Vector3D transformDirection(const Vector3D &vector) const
    {
		return Vector3D(
			vector.x * data[0] +
            vector.y * data[1] +
            vector.z * data[2],

            vector.x * data[4] +
            vector.y * data[5] +
            vector.z * data[6],

            vector.x * data[8] +
            vector.y * data[9] +
            vector.z * data[10]
		);
    }

    // Transform the given direction vector by the transformational inverse 
	 // of this matrix.
     // *note* - This function relies on the fact that the inverse of a pure 
	 //           rotation matrix is its transpose. It separates the translational 
	 //           and rotation components, transposes the rotation, and multiplies out.
	 //           If the matrix is not a scale and shear free transform matrix, 
	 //           then this function will not give correct results.
         
     // *note* -  When a direction is converted between frames of reference, 
	 //           there is no translation required.
     // input: vector-  The vector to transform.
        
    Vector3D transformInverseDirection(const Vector3D &vector) const
    {
		return Vector3D(
			vector.x * data[0] +
            vector.y * data[4] +
            vector.z * data[8],

            vector.x * data[1] +
            vector.y * data[5] +
            vector.z * data[9],

            vector.x * data[2] +
            vector.y * data[6] +
            vector.z * data[10]
		);
    }

     // Transform the given vector by the transformational inverse of this matrix.
     // *note* -  This function relies on the fact that the inverse of a pure rotation
	 //           matrix is its transpose. It separates the translational and rotation 
	 //           components, transposes the rotation, and multiplies out. 
	 //           If the matrix is not a scale and shear free transform matrix, then 
	 //           this function will not give correct results.
      
     // input: vector- The vector to transform.
        
    Vector3D transformInverse(const Vector3D &vector) const
    {
		Vector3D tmp = vector;
        tmp.x -= data[3];
        tmp.y -= data[7];
        tmp.z -= data[11];
        return Vector3D(
		    tmp.x * data[0] +
            tmp.y * data[4] +
            tmp.z * data[8],

            tmp.x * data[1] +
            tmp.y * data[5] +
            tmp.z * data[9],

            tmp.x * data[2] +
            tmp.y * data[6] +
            tmp.z * data[10]
		);
    }

     // Gets a vector representing one axis (i.e. one column) in the matrix.
     //    
     // input:  i- The row to return. 
	 //         Row 3 corresponds to the position of the transform matrix.
       
     // output:  returns The row as a vector.
        
    Vector3D GetAxisVector(int i) const
    {
        return Vector3D(data[i], data[i+4], data[i+8]);
    }

       
    // Sets this matrix to be the rotation matrix corresponding to the given quaternion.
        
    void SetOrientationAndPos(const Quaternion &q, const Vector3D &pos)
    {
        data[0] = 1 - (2*q.m_j*q.m_j + 2*q.m_k*q.m_k);
        data[1] = 2*q.m_i*q.m_j + 2*q.m_k*q.m_r;
        data[2] = 2*q.m_i*q.m_k - 2*q.m_j*q.m_r;
        data[3] = pos.x;

        data[4] = 2*q.m_i*q.m_j - 2*q.m_k*q.m_r;
        data[5] = 1 - (2*q.m_i*q.m_i  + 2*q.m_k*q.m_k);
        data[6] = 2*q.m_j*q.m_k + 2*q.m_i*q.m_r;
        data[7] = pos.y;

        data[8] = 2*q.m_i*q.m_k + 2*q.m_j*q.m_r;
        data[9] = 2*q.m_j*q.m_k - 2*q.m_i*q.m_r;
        data[10] = 1 - (2*q.m_i*q.m_i  + 2*q.m_j*q.m_j);
        data[11] = pos.z;
    }

	    void fillGLArray(float array[16]) const
        {
            array[0] = (float)data[0];
            array[1] = (float)data[4];
            array[2] = (float)data[8];
            array[3] = (float)0;

            array[4] = (float)data[1];
            array[5] = (float)data[5];
            array[6] = (float)data[9];
            array[7] = (float)0;

            array[8] = (float)data[2];
            array[9] = (float)data[6];
            array[10] = (float)data[10];
            array[11] = (float)0;

            array[12] = (float)data[3];
            array[13] = (float)data[7];
            array[14] = (float)data[11];
            array[15] = (float)1;
        }
};
}
#endif