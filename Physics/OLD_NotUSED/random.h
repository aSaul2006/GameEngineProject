/*
 * Interface file for the random number generator.
 *
 * Part of the Cyclone physics system.
 *
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */

/**
 * @file
 *
 * This file contains the definitions for a random number generator.
 */
#ifndef RANDOM_H
#define RANDOM_H

#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3D.h"

namespace physEngine {


    /**
     * Keeps track of one random stream: i.e. a seed and its output.
     * This is used to get random numbers. Rather than a funcion, this
     * allows there to be several streams of repeatable random numbers
     * at the same time. Uses the RandRotB algorithm.
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
         * Returns a random floating point number between 0 and 1.
         */
        double randomReal();

        /**
         * Returns a random floating point number between 0 and scale.
         */
        double randomReal(double scale);

        /**
         * Returns a random floating point number between min and max.
         */
        double randomReal(double min, double max);

        /**
         * Returns a random integer less than the given value.
         */
        unsigned randomInt(unsigned max);

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

} // namespace

#endif 