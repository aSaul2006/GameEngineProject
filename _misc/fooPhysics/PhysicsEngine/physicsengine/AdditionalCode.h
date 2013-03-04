/***************************************************************************************
*  Program Name:    AdditionalCode.h                                                   *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the extra code needed for the Physics Engine    *
*                    to run                                                            *
*                                                                                      *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/

#include <math.h>

#ifndef PHYSICSENGINE_ADDITIONALCODE_H_
#define PHYSICSENGINE_ADDITIONALCODE_H_


#include "matrix3.h"
#include "matrix4.h"
#include "quaternion.h"
#include "vector3d.h"

namespace PhysicsEngine
{
	extern double statisFactor;

	void setStatisFactor(double value);

	double getStatisFactor();
}

#define DBL_MAX  1.7976931348623158e+308 /* max value */

#endif

