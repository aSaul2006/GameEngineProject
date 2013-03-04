/***************************************************************************************
*  Program Name:    random.h                                                           *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Interface file for the random number generators                    *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Interface file for the random number generator.
 */

/*
 * This file contains the definitions for a random number generator.
 */
#pragma once

#ifndef PHYSICSENGINE_RANDOM_H
#define PHYSICSENGINE_RANDOM_H

// gigitty
#include "PhysicsEngine.h"
#include "AdditionalCode.h"
#include "vector3d.h"

namespace PhysicsEngine 

{

	//

    /**
     * Keeps track of one random stream: i.e. a seed and its output.
     * This is used to get random numbers. Rather than a funcion, this
     * allows there to be several streams of repeatable random numbers
     * at the same time. Uses the RandRotB algorithm.
     */  /* Changes made: not using Precisoin so everything changed from 
	 * REAL to double to match Physics Engine ***/  /*
	 */
    class Random
    {
    public:

        /**
         * Creates a new random number stream with a seed based on
         * timing data.
         */
        Random();

        /**
         * Creates a new random stream with the given seed.
         */
        Random(unsigned seed);

        /**
         * Sets the seed value for the random stream.
         */
        void seed(unsigned seed);

        /**
         * Returns the next random bitstring from the stream. This is
         * the fastest method.
         */
        unsigned randomBits();

        /**
         * Returns a random double number between 0 and 1.
         */
        double randomDouble();

        /**
         * Returns a random double number between 0 and scale.
         */
        double randomDouble(double scale);

        /**
         * Returns a random double number between min and max.
         */
        double randomDouble(double min, double max);

        /**
         * Returns a random double number less than the given value.
         */
        unsigned randomDoubleInt(unsigned max);

        /**
         * Returns a random binomially distributed number between -scale
         * and +scale.
         */
        double randomBinomial(double scale);

        /**
         * Returns a random vector where each component is binomially
         * distributed in the range (-scale to scale) [mean = 0.0f].
         */
        Vector3D randomVector(double scale);

        /**
         * Returns a random vector where each component is binomially
         * distributed in the range (-scale to scale) [mean = 0.0f],
         * where scale is the corresponding component of the given
         * vector.
         */
        Vector3D randomVector(const Vector3D &scale);

        /**
         * Returns a random vector in the cube defined by the given
         * minimum and maximum vectors. The probability is uniformly
         * distributed in this region.
         */
        Vector3D randomVector(const Vector3D &min, const Vector3D &max);

        /**
         * Returns a random vector where each component is binomially
         * distributed in the range (-scale to scale) [mean = 0.0f],
         * except the y coordinate which is zero.
         */
        Vector3D randomXZVector(double scale);

        /**
         * Returns a random orientation (i.e. normalized) quaternion.
         */
        Quaternion randomQuaternion();

    private:
        // Internal mechanics
        int p1, p2;
        unsigned buffer[17];
    };

} 
#endif 