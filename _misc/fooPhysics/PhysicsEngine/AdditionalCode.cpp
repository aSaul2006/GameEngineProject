/***************************************************************************************
*  Program Name:    AdditionalCode.cpp                                                 *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the extra code needed for the Physics Engine    *
*                    to run                                                            *
*                                                                                      *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/

#include "physicsengine\AdditionalCode.h"

using namespace PhysicsEngine;

const Vector3D Vector3D::GRAVITY = Vector3D(0, -9.81, 0);
const Vector3D Vector3D::HIGH_GRAVITY = Vector3D(0, -19.62, 0);
const Vector3D Vector3D::UP = Vector3D(0, 1, 0);
const Vector3D Vector3D::RIGHT = Vector3D(1, 0, 0);
const Vector3D Vector3D::OUT_OF_SCREEN = Vector3D(0, 0, 1);
const Vector3D Vector3D::X = Vector3D(0, 1, 0);
const Vector3D Vector3D::Y = Vector3D(1, 0, 0);
const Vector3D Vector3D::Z = Vector3D(0, 0, 1);

double PhysicsEngine::statisFactor = ((double)0.3);

void PhysicsEngine::setStatisFactor(double value)
{
    PhysicsEngine::statisFactor = value;
}

double PhysicsEngine::getStatisFactor()
{
    return PhysicsEngine::statisFactor;
}

double Matrix4::GetDeterminant() const
{
    return data[8]*data[5]*data[2]+
        data[4]*data[9]*data[2]+
        data[8]*data[1]*data[6]-
        data[0]*data[9]*data[6]-
        data[4]*data[1]*data[10]+
        data[0]*data[5]*data[10];
}

void Matrix4::setInverse(const Matrix4 &m)
{
    // Make sure the determinant is non-zero.
    double det = GetDeterminant();
    if (det == 0) return;
    det = ((double)1.0)/det;

    data[0] = (-m.data[9]*m.data[6]+m.data[5]*m.data[10])*det;
    data[4] = (m.data[8]*m.data[6]-m.data[4]*m.data[10])*det;
    data[8] = (-m.data[8]*m.data[5]+m.data[4]*m.data[9])*det;

    data[1] = (m.data[9]*m.data[2]-m.data[1]*m.data[10])*det;
    data[5] = (-m.data[8]*m.data[2]+m.data[0]*m.data[10])*det;
    data[9] = (m.data[8]*m.data[1]-m.data[0]*m.data[9])*det;

    data[2] = (-m.data[5]*m.data[2]+m.data[1]*m.data[6])*det;
    data[6] = (+m.data[4]*m.data[2]-m.data[0]*m.data[6])*det;
    data[10] = (-m.data[4]*m.data[1]+m.data[0]*m.data[5])*det;

    data[3] = (m.data[9]*m.data[6]*m.data[3]
               -m.data[5]*m.data[10]*m.data[3]
               -m.data[9]*m.data[2]*m.data[7]
               +m.data[1]*m.data[10]*m.data[7]
               +m.data[5]*m.data[2]*m.data[11]
               -m.data[1]*m.data[6]*m.data[11])*det;
    data[7] = (-m.data[8]*m.data[6]*m.data[3]
               +m.data[4]*m.data[10]*m.data[3]
               +m.data[8]*m.data[2]*m.data[7]
               -m.data[0]*m.data[10]*m.data[7]
               -m.data[4]*m.data[2]*m.data[11]
               +m.data[0]*m.data[6]*m.data[11])*det;
    data[11] =(m.data[8]*m.data[5]*m.data[3]
               -m.data[4]*m.data[9]*m.data[3]
               -m.data[8]*m.data[1]*m.data[7]
               +m.data[0]*m.data[9]*m.data[7]
               +m.data[4]*m.data[1]*m.data[11]
               -m.data[0]*m.data[5]*m.data[11])*det;
}

Matrix3 Matrix3::LinearInterpolate(const Matrix3& a, const Matrix3& b, double prop)
{
    Matrix3 result;
    for (unsigned i = 0; i < 9; i++) {
        result.data[i] = a.data[i] * (1-prop) + b.data[i] * prop;
    }
    return result;
}