/***************************************************************************************
*  Program Name:    Matrix3.h                                                          *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            January 25, 2013                                                   *
*  Description:     This program fully functional Matrix3 class for the Physics Core   *
*                   using double type precision                                        *
*  Update:           name of author, date (mm/dd/yy)  separated by “;”                 *
*                   “ex.- Bryan Sweeney, 01/20/13; Mr. Jones, 01/20/13;”               *
***************************************************************************************/

#ifndef PHYSICSENGINE_MATRIX3_H_
#define PHYSICSENGINE_MATRIX3_H_

//-------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------
#include "AdditionalCode.h"
#include "vector3d.h"
#include "quaternion.h"

namespace PhysicsEngine 
{
class Matrix3
    {
    public:
        
        double data[9]; // holds matrix in array form

        // Creates a new matrix.
        
        Matrix3()
        {
            data[0] = data[1] = data[2] = data[3] = data[4] = data[5] =
                data[6] = data[7] = data[8] = 0;
        }

        // Creates a new matrix with the given three vectors making up its columns.
         
        Matrix3(const Vector3D &compOne, const Vector3D &compTwo,
            const Vector3D &compThree)
        {
            SetComponents(compOne, compTwo, compThree);
        }

       
         // Sets the matrix values from the given three vector components.
         // These are arranged as the three columns of the vector.
         
        void SetComponents(const Vector3D &compOne, const Vector3D &compTwo,
            const Vector3D &compThree)
        {
            data[0] = compOne.x;
            data[1] = compTwo.x;
            data[2] = compThree.x;
            data[3] = compOne.y;
            data[4] = compTwo.y;
            data[5] = compThree.y;
            data[6] = compOne.z;
            data[7] = compTwo.z;
            data[8] = compThree.z;

        }
		
		// Creates a new matrix with explicit coefficients.
         
        Matrix3(double c0, double c1, double c2, double c3, double c4, double c5,
            double c6, double c7, double c8)
        {
            data[0] = c0; data[1] = c1; data[2] = c2;
            data[3] = c3; data[4] = c4; data[5] = c5;
            data[6] = c6; data[7] = c7; data[8] = c8;
        }

        
         // Sets the matrix to be a diagonal matrix with the given
         // values along the leading diagonal.

        void SetDiagonal(double a, double b, double c)
        {
            SetInertiaTensorCoeffs(a, b, c);
        }

       
         // Sets the value of the matrix from inertia tensor values.
         
        void SetInertiaTensorCoeffs(double ix, double iy, double iz,
            double ixy=0, double ixz=0, double iyz=0)
        {
            data[0] = ix;
            data[1] = data[3] = -ixy;
            data[2] = data[6] = -ixz;
            data[4] = iy;
            data[5] = data[7] = -iyz;
            data[8] = iz;
        }

		// Sets the value of the matrix as an inertia tensor of a rectangular 
		// block aligned with the body's coordinate system with the given axis 
		// half-sizes and mass.
        
        void SetBlockInertiaTensor(const Vector3D &m_v3HalfSizeBox, double dMass)
        {
            Vector3D squares = m_v3HalfSizeBox.componentProduct(m_v3HalfSizeBox);
            SetInertiaTensorCoeffs(0.3f*dMass*(squares.y + squares.z),
                0.3f*dMass*(squares.x + squares.z),
                0.3f*dMass*(squares.x + squares.y));
        }

		// Sets the matrix to be a skew symmetric matrix based on the given vector. 
		// The skew symmetric matrix is the equivalent of the vector product. 
		// So if a,b are vectors. a x b = A_s b where A_s is the skew symmetric form
		// of a.
         
        void SetSkewSymmetric(const Vector3D vector)
        {
            data[0] = data[4] = data[8] = 0;
            data[1] = -vector.z;
            data[2] = vector.y;
            data[3] = vector.z;
            data[5] = -vector.x;
            data[6] = -vector.y;
            data[7] = vector.x;
        }

		// Transform the given vector by this matrix.
        // input:  vector- The vector to transform.
         
        Vector3D operator*(const Vector3D &vector) const
        {
            return Vector3D(
                vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
                vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
                vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
            );
        }

        // Transform the given vector by this matrix.
        // input:  vector- The vector to transform.
         
        Vector3D transform(const Vector3D &vector) const
        {
            return (*this) * vector;
        }

        // Transform the given vector by the transpose of this matrix.
        // input:  vector-  The vector to transform.
         
       Vector3D transformTranspose(const Vector3D &vector) const
        {
            return Vector3D(
                vector.x * data[0] + vector.y * data[3] + vector.z * data[6],
                vector.x * data[1] + vector.y * data[4] + vector.z * data[7],
                vector.x * data[2] + vector.y * data[5] + vector.z * data[8]
            );
        }

        // Gets a vector representing one row in the matrix.
        // input:  i- The row to return.
         
        Vector3D GetRowVector(int i) const
        {
            return Vector3D(data[i*3], data[i*3+1], data[i*3+2]);
        }

        // Gets a vector representing one axis (i.e. one column) in the matrix.
        // input:  i- The row to return.
        // output: returns The axis as vector.
         
        Vector3D GetAxisVector(int i) const
        {
            return Vector3D(data[i], data[i+3], data[i+6]);
        }

        // Sets the matrix to be the inverse of the given matrix.
        // input:  m-  The matrix to invert and use to set this.
        
        void setInverse(const Matrix3 &m)
        {
            double t4 = m.data[0]*m.data[4];
            double t6 = m.data[0]*m.data[5];
            double t8 = m.data[1]*m.data[3];
            double t10 = m.data[2]*m.data[3];
            double t12 = m.data[1]*m.data[6];
            double t14 = m.data[2]*m.data[6];

            // Calculate the determinant
            double t16 = (t4*m.data[8] - t6*m.data[7] - t8*m.data[8]+
                        t10*m.data[7] + t12*m.data[5] - t14*m.data[4]);

            // Make sure the determinant is non-zero.
            if (t16 == (double)0.0f) return;
            double t17 = 1/t16;

            data[0] = (m.data[4]*m.data[8]-m.data[5]*m.data[7])*t17;
            data[1] = -(m.data[1]*m.data[8]-m.data[2]*m.data[7])*t17;
            data[2] = (m.data[1]*m.data[5]-m.data[2]*m.data[4])*t17;
            data[3] = -(m.data[3]*m.data[8]-m.data[5]*m.data[6])*t17;
            data[4] = (m.data[0]*m.data[8]-t14)*t17;
            data[5] = -(t6-t10)*t17;
            data[6] = (m.data[3]*m.data[7]-m.data[4]*m.data[6])*t17;
            data[7] = -(m.data[0]*m.data[7]-t12)*t17;
            data[8] = (t4-t8)*t17;
        }

        // Returns a new matrix containing the inverse of this matrix. 

        Matrix3 Inverse() const
        {
            Matrix3 result;
            result.setInverse(*this);
            return result;
        }

        // Inverts the matrix.
        
        void Invert()
        {
            setInverse(*this);
        }

        // Sets the matrix to be the transpose of the given matrix.
        // input: m- The matrix to transpose and use to set this.
        
        void SetTranspose(const Matrix3 &m)
        {
            data[0] = m.data[0];
            data[1] = m.data[3];
            data[2] = m.data[6];
            data[3] = m.data[1];
            data[4] = m.data[4];
            data[5] = m.data[7];
            data[6] = m.data[2];
            data[7] = m.data[5];
            data[8] = m.data[8];
        }

        // Returns a new matrix containing the transpose of this matrix. 

        Matrix3 Transpose() const
        {
            Matrix3 result;
            result.SetTranspose(*this);
            return result;
        }

        // Returns a matrix which is this matrix multiplied by the given other matrix.
        // o = other
        Matrix3 operator*(const Matrix3 &o) const
        {
            return Matrix3(
                data[0]*o.data[0] + data[1]*o.data[3] + data[2]*o.data[6],
                data[0]*o.data[1] + data[1]*o.data[4] + data[2]*o.data[7],
                data[0]*o.data[2] + data[1]*o.data[5] + data[2]*o.data[8],

                data[3]*o.data[0] + data[4]*o.data[3] + data[5]*o.data[6],
                data[3]*o.data[1] + data[4]*o.data[4] + data[5]*o.data[7],
                data[3]*o.data[2] + data[4]*o.data[5] + data[5]*o.data[8],

                data[6]*o.data[0] + data[7]*o.data[3] + data[8]*o.data[6],
                data[6]*o.data[1] + data[7]*o.data[4] + data[8]*o.data[7],
                data[6]*o.data[2] + data[7]*o.data[5] + data[8]*o.data[8]
                );
        }

        // Multiplies this matrix in place by the given other matrix.
        // o = other 
        void operator*=(const Matrix3 &o)
        {
            double t1;
            double t2;
            double t3;

            t1 = data[0]*o.data[0] + data[1]*o.data[3] + data[2]*o.data[6];
            t2 = data[0]*o.data[1] + data[1]*o.data[4] + data[2]*o.data[7];
            t3 = data[0]*o.data[2] + data[1]*o.data[5] + data[2]*o.data[8];
            data[0] = t1;
            data[1] = t2;
            data[2] = t3;

            t1 = data[3]*o.data[0] + data[4]*o.data[3] + data[5]*o.data[6];
            t2 = data[3]*o.data[1] + data[4]*o.data[4] + data[5]*o.data[7];
            t3 = data[3]*o.data[2] + data[4]*o.data[5] + data[5]*o.data[8];
            data[3] = t1;
            data[4] = t2;
            data[5] = t3;

            t1 = data[6]*o.data[0] + data[7]*o.data[3] + data[8]*o.data[6];
            t2 = data[6]*o.data[1] + data[7]*o.data[4] + data[8]*o.data[7];
            t3 = data[6]*o.data[2] + data[7]*o.data[5] + data[8]*o.data[8];
            data[6] = t1;
            data[7] = t2;
            data[8] = t3;
        }

        // Multiplies this matrix in place by the given scalar.
         
        void operator*=(const double scalar)
        {
            data[0] *= scalar; data[1] *= scalar; data[2] *= scalar;
            data[3] *= scalar; data[4] *= scalar; data[5] *= scalar;
            data[6] *= scalar; data[7] *= scalar; data[8] *= scalar;
        }

        // Does a component-wise addition of this matrix and the given matrix.
        
        void operator+=(const Matrix3 &o)
        {
            data[0] += o.data[0]; data[1] += o.data[1]; data[2] += o.data[2];
            data[3] += o.data[3]; data[4] += o.data[4]; data[5] += o.data[5];
            data[6] += o.data[6]; data[7] += o.data[7]; data[8] += o.data[8];
        }

        // Sets this matrix to be the rotation matrix corresponding to the 
		// given quaternion.
         
		void SetOrientationQ(const PhysicsEngine::Quaternion &q)
        {
            data[0] = 1 - (2*q.m_j*q.m_j + 2*q.m_k*q.m_k);
            data[1] = 2*q.m_i*q.m_j + 2*q.m_k*q.m_r;
            data[2] = 2*q.m_i*q.m_k - 2*q.m_j*q.m_r;
            data[3] = 2*q.m_i*q.m_j - 2*q.m_k*q.m_r;
            data[4] = 1 - (2*q.m_i*q.m_i  + 2*q.m_k*q.m_k);
            data[5] = 2*q.m_j*q.m_k + 2*q.m_i*q.m_r;
            data[6] = 2*q.m_i*q.m_k + 2*q.m_j*q.m_r;
            data[7] = 2*q.m_j*q.m_k - 2*q.m_i*q.m_r;
            data[8] = 1 - (2*q.m_i*q.m_i  + 2*q.m_j*q.m_j);
        }

       
        // Interpolates a couple of matrices.
        
		 static Matrix3 LinearInterpolate(const Matrix3& a, const Matrix3& b, double prop);
    };
}
#endif
