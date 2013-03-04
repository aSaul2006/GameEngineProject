/*****************************************************************
Program Name: Vector2D.h
Name: Zach Fetters
Description: This file allows for the creation of 2D vectors holding
			  an x and y value for points in the game world, and
			  also implements functions to perform various actions
			  on these vectors
*****************************************************************/

#include <math.h>
#include <Windows.h>
#include <iosfwd>
#include <limits>
#include "Constants.h"


struct Vector2D
{

	double x;
	double y;

	Vector2D():x(0.0),y(0.0)
	{
	
	}

	Vector2D(double a, double b):x(a),y(b)
	{
	
	}

	//set x and y to 0
	void SetZero() 
	{
		x=0.0; 
		y=0.0;
	
	} 
	
	//return true if x and y are 0
	bool IsZero()const 
	{
		
		return (x*x + y*y) < MinDouble;
	
	} 

	inline double Length()const; //get length of vector
	inline double LengthSq()const; //get squared length of vector
	inline void Normalize(); //normalize this vector
	inline double Dot(const Vector2D& v2)const; //return the dot product of this vector
	inline int Sign(const Vector2D& v2)const; //check if v2 is clockwise or counter-clockwise from this vector
	inline Vector2D Perpendicular()const; //get a vector which is perpendicular to this one
	inline void Truncate(double Max); //adjust x and y to keep vector from exceeding max
	inline double Distance(const Vector2D &v2)const; //return the distance between this vector and v2
	inline double DistanceSq(const Vector2D &v2)const; //return the squared distance between this vector and v2
	inline void Reflect(const Vector2D& v2);  //reflect this vector
	inline Vector2D GetReverse()const; //get the reverse of this vector

//overload the += operator
const Vector2D& operator+=(const Vector2D &v2)
 {
    x += v2.x;
    y += v2.y;

    return *this;
 }

//overlaod the -= operator
 const Vector2D& operator-=(const Vector2D &v2)
 {
    x -= v2.x;
    y -= v2.y;

    return *this;
 }

//overload the *= operator
const Vector2D& operator*=(const double& v2)
 {
    x *= v2;
    y *= v2;

    return *this;
 }

//overload the /= operator
 const Vector2D& operator/=(const double& v2)
 {
    x /= v2;
    y /= v2;

    return *this;
 }

 //overload the == operator
 bool operator==(const Vector2D& v2)const
 {
    return((x == v2.x) &&(y == v2.y));
 }

 //overload the != operator
 bool operator!=(const Vector2D& v2)const
 {
    return (x != v2.x) || (y != v2.y);
 }


};


//return the length of this vector
inline double Vector2D::Length()const
{
  return sqrt(x * x + y * y);
}


//return the squared length of this vector
inline double Vector2D::LengthSq()const
{
  return (x * x + y * y);
}


//return the dot product of this vector
inline double Vector2D::Dot(const Vector2D &v2)const
{
  return x*v2.x + y*v2.y;
}

//enumerate values for clockwise and counter-clockwise
enum 
{
	clockwise = 1,
	anticlockwise = -1
};

//return whether v2 is clockwise or counter-clockwise of this vector
inline int Vector2D::Sign(const Vector2D& v2)const
{
  if (y*v2.x > x*v2.y)
  { 
    return anticlockwise;
  }
  else 
  {
    return clockwise;
  }
}

//return the vector perpendicular to this vector
inline Vector2D Vector2D::Perpendicular()const
{
  return Vector2D(-y, x);
}

//return euclidean distance between this vector and v2
inline double Vector2D::Distance(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}


//return squared euclidean distance between this vector and v2
inline double Vector2D::DistanceSq(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

//adjust this vector so it does not exceed max
inline void Vector2D::Truncate(double max)
{
  if (this->Length() > max)
  {
    this->Normalize();

    *this *= max;
  } 
}

//reflect this vector
inline void Vector2D::Reflect(const Vector2D& norm)
{
  *this += 2.0 * this->Dot(norm) * norm.GetReverse();
}


//return the reverse of this vector
inline Vector2D Vector2D::GetReverse()const
{
  return Vector2D(-this->x, -this->y);
}


//normalize this vector
inline void Vector2D::Normalize()
{ 
  double vector_length = this->Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
  }
}


/***********************Non-Member Functions******************************/

//returns a normalized version of vector v
inline Vector2D Vec2DNormalize(const Vector2D &v)
{
  Vector2D vec = v;

  //get the length of the vector
  double vector_length = vec.Length();

  //if the length is greater than the max then normalize the vector
  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    vec.x /= vector_length;
    vec.y /= vector_length;
  }

  return vec;
}

//return the distance between two vectors
inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

//return the squared distance between two vectors
inline double Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

//return the length of a vector
inline double Vec2DLength(const Vector2D& v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

//return the squared length of a vector
inline double Vec2DLengthSq(const Vector2D& v)
{
  return (v.x*v.x + v.y*v.y);
}

//overload the * operator
inline Vector2D operator*(const Vector2D &v1, double v2)
{
  Vector2D result(v1);
  result *= v2;
  return result;
}

//overload the * operator another way
inline Vector2D operator*(double v1, const Vector2D &v2)
{
  Vector2D result(v2);
  result *= v1;
  return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &v1, const Vector2D &v2)
{
  Vector2D result(v1);
  result.x -= v2.x;
  result.y -= v2.y;
  
  return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &v1, const Vector2D &v2)
{
  Vector2D result(v1);
  result.x += v2.x;
  result.y += v2.y;
  
  return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &v1, double value)
{
  Vector2D result(v1);
  result.x /= value;
  result.y /= value;

  return result;
}