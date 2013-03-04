/***************************************************************************************
*  Program Name:    Vector3D.h                                                         *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            January 22, 2013                                                   *
*  Description:     This program is a fully functional 3D vector class                 *
*                     in double type precision                                         *
*  Update:           Jen 1/30/2013                                                     *
*                                                                                      *
***************************************************************************************/


#ifndef PHYSICSENGINE_VECTOR3D_H_
#define PHYSICSENGINE_VECTOR3D_H_


#define PI  3.14159265358979323846

#include "AdditionalCode.h"



namespace PhysicsEngine
{
	class Vector3D
	{
	private:
		static Vector3D* Vector; // not sure if needed marked 4 possible removal
		/** Padding to ensure 4 word alignment. */
        double pad;
    public:
	    // moved so matrix can access
	    double x, y, z;
	    // purpose:    Our constructor
	    // input:      ex- out vector's i component
	    //             why- our vector's j component
	    //             zee- our vector's k component
	    // output:     no explicit output
	


	    //CODE ADDED FOR TEST PURPOSES
	    // default constructor
	    Vector3D() : x(0), y(0), z(0) {}

	    //explict constructor
	    Vector3D(const double x, const double y, const double z)
		: x(x), y(y), z(z) {}

		const static Vector3D GRAVITY;
        const static Vector3D HIGH_GRAVITY;
        const static Vector3D UP;
        const static Vector3D RIGHT;
        const static Vector3D OUT_OF_SCREEN;
        const static Vector3D X;
        const static Vector3D Y;
        const static Vector3D Z;


	//Vector3D(double ex = 0, double why = 0, double zee = 0)
	//{
	//	x = ex;  y = why;  z = zee;
	//}

	// purpose:    Our destructor
	// input:      none
	// output:     none
	
	~Vector3D() { }

	// purpose:    Calculate the magnitude of our invoking vector
	// input:      no explicit input
	// output:     the magnitude of out invoking vector

	double Vector3D::magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}

	//BELOW IS FOR TESTING PURPOSES TO DEBUG function getMagnitude NOT FOUND IN INITIAL COMPILE

	//B E G I N    T E S T


	double Vector3D::getMagnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}






    // Gets the squared magnitude of this vector. 
    double Vector3D::squareOfTheMagnitude() const
    {
		return x * x + y * y + z * z;
    }

	// purpose:    Multiply our vector by a scalar value
	// input:      num- the scalar value being multiplied
	// output:     our newly created vector

	Vector3D operator*(const double num) const
	{
		return Vector3D(x*num,y*num,z*num);
	}

	// purpose:    multiply out vector by a scalar value
	// input:      num- the scalar value to be multiplied
	//             vec- the vector we are multiplying to
	// output:     our newly created vector

	//friend Vector3D operator *(double num, const Vector3D &vec)
	//{
	//	return Vector3D(vec.x * num, vec.y * num, vec.z * num);
	//}

	double Vector3D::operator *(const Vector3D &vec) const
	{
		return x*vec.x + y*vec.y +z*vec.z;
	}

	// purpose:    Ading two vectors
	// input: vec- the being added to our invoking object
	// output: our newly created sum of the two vectors

	Vector3D Vector3D::operator+(const Vector3D &vec) const
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	// purpose:    Subtracting two vectors
	// input:      vec- the vector being subtracted from our invoking object
	// output:     our newly created difference of the two vectors

	Vector3D Vector3D::operator-(const Vector3D &vec) const
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	double Vector3D::operator[](unsigned i) const
    {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    double& Vector3D::operator[](unsigned i)
    {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    // Adds the given vector to this. 
    void Vector3D::operator+=(const Vector3D& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    // Subtracts the given vector from this. 
    void Vector3D::operator-=(const Vector3D& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    // Multiplies this vector by the given scalar. */
    void Vector3D::operator *=(const double value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    // Updates this vector to be the cross product of its current value 
	// and the given vector.
    void Vector3D::operator %=(const Vector3D &vector)
    {
        *this = vectorProduct(vector);
    }

    // Calculates and returns the cross product of this vector with the given vector.
    Vector3D Vector3D::operator%(const Vector3D &vector) const
    {
        return Vector3D(y*vector.z - z * vector.y,
                        z * vector.x - x * vector.z,
                        x * vector.y - y * vector.x);
    }


    // Turns a non-zero vector into a vector of unit length. 
	// input:       no explicit input
	// output:      normalised vector
    void Vector3D::normalize()
    {
        double l = magnitude();
		if (l > 0)
        {
           (*this) *= ((double)1)/l;
        }
    }

	// purpose:     Normalize our invoking vector *this changes our vector*
	// input:       no explicit input
	// output:      none

	void Vector3D::normalizeVector3D(void)
	{
		double mag = sqrt(x * x + y * y + z * z);
		x /= mag;  y /= mag;  z /= mag;
	}

	// purpose:     Returns the normalised version of a vector.
	// input:       no rxplicit input
	// output:      returns the normalised vector
    Vector3D Vector3D::Unit() const
    {
        Vector3D result = *this;
        result.normalize();
        return result;
    }


	// purpose:     Dot Product two vectors
	// input:       vec- the vector being dotted with our invoking object
	// output:      the dot product of the two vectors

	double Vector3D::dotVector3D(const Vector3D &vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	// purpose:     Cross product two vectors
	// input:       vec- the vector being crossed with our invoking object
	// output:      our newly created resultant vector
	Vector3D Vector3D::vectorProduct(const Vector3D &vec) const
	{
		return Vector3D( y * vec.z - z * vec.y, 
			z * vec.x - x * vec.z,
			x * vec.y - y * vec.x);
	}

	// purpose:     To calculate the angle between to vectors
	// input:       vec- the vector being ceing used with our invoking object
	// output:      the angle 

	double Vector3D::angleBetween3DVectors(Vector3D &vec) 
	{
		return (acos(dotVector3D(vec) / (getMagnitude() * vec.getMagnitude())))
			* (180 / PI);
	}

	// purpose:     To calculate and return a component-wise product of two vectors 
	// input:       vec- the vector being multiplied with our invoking object
	// output:      the resultant vector
	Vector3D Vector3D::componentProduct(const Vector3D &vec) const
	{
		return Vector3D(x * vec.x, y * vec.y, z * vec.z);
	}

	// Calculates and returns the scalar product of this vector with the given vector.
    double Vector3D::scalarProduct(const Vector3D &vector) const
    {
        return x*vector.x + y*vector.y + z*vector.z;
    }

	// Adds the given vector to this, scaled by the given amount.
    void Vector3D::addScaledVector(const Vector3D& vector, double scale)
    {
        x += vector.x * scale;
        y += vector.y * scale;
        z += vector.z * scale;
    }

	// Zero all the components of the vector.
    void Vector3D::clear()
    {
		x = y = z = 0;
    }

	// Flips all the components of the vector. 
    void Vector3D::invert()
    {
        x = -x;
        y = -y;
        x = -z;
    }

	// Checks if the two vectors have identical components. 
    bool Vector3D::operator==(const Vector3D& other) const
    {
        return x == other.x &&
            y == other.y &&
            z == other.z;
    }

    // Checks if the two vectors have non-identical components. 
    bool Vector3D::operator!=(const Vector3D& other) const
    {
        return !(*this == other);
    }

    // Checks if this vector is component-by-component less than the other.
    // *note* This does not behave like a single-value comparison:
    // !(a < b) does not imply (b >= a).
    bool Vector3D::operator<(const Vector3D& other) const
    {
        return x < other.x && y < other.y && z < other.z;
    }

    // Checks if this vector is component-by-component less than the other.
    // *note* This does not behave like a single-value comparison:
    // !(a < b) does not imply (b >= a).
    bool Vector3D::operator>(const Vector3D& other) const
    {
        return x > other.x && y > other.y && z > other.z;
    }

    // Checks if this vector is component-by-component less than the other.
    // *note* This does not behave like a single-value comparison:
    // !(a <= b) does not imply (b > a).
    bool Vector3D::operator<=(const Vector3D& other) const
    {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    // Checks if this vector is component-by-component less than the other.
    // *note* This does not behave like a single-value comparison:
    // !(a <= b) does not imply (b > a).
    bool Vector3D::operator>=(const Vector3D& other) const
    {
        return x >= other.x && y >= other.y && z >= other.z;
    }
       /** Limits the size of the vector to the given maximum. */
        void trim(double size)
        {
            if (squareOfTheMagnitude() > size*size)
            {
                normalize();
                x *= size;
                y *= size;
                z *= size;
            }
        }

};

}

#endif