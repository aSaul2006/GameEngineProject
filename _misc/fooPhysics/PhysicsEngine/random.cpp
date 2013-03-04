/***************************************************************************************
*  Program Name:    random.cpp                                                         *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Implementation file for random number generators                   *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Implementation file for random number generation.

 */

#include <cstdlib>
#include <ctime>
#include "physicsengine\random.h"

using namespace PhysicsEngine;

Random::Random()
{
    seed(0);
}

Random::Random(unsigned seed)
{
    Random::seed(seed);
}

void Random::seed(unsigned s)
{
    if (s == 0) {
        s = (unsigned)clock();
    }

    // Fill the buffer with some basic random numbers
    for (unsigned i = 0; i < 17; i++)
    {
        // Simple linear congruential generator
        s = s * 2891336453 + 1;
        buffer[i] = s;
    }

    // Initialize pointers into the buffer
    p1 = 0;  p2 = 10;
}

unsigned Random::randomBits()
{
    unsigned result;

    // Rotate the buffer and store it back to itself
    result = buffer[p1] = _lrotl(buffer[p2], 13) + _lrotl(buffer[p1], 9);

    // Rotate pointers
    if (--p1 < 0) p1 = 16;
    if (--p2 < 0) p2 = 16;

    // Return result
    return result;
}

double Random::randomDouble()
{
    // Get the random number
    unsigned bits = randomBits();

    // Set up a reinterpret structure for manipulation
    union {
        double value;
        unsigned words[2];
    } convert;

    // Now assign the bits to the words. This works by fixing the ieee
    // sign and exponent bits (so that the size of the result is 1-2)
    // and using the bits to create the fraction part of the float. Note
    // that bits are used more than once in this process.
    convert.words[0] =  bits << 20; // Fill in the top 16 bits
    convert.words[1] = (bits >> 12) | 0x3FF00000; // And the bottom 20

    // And return the value
    return convert.value - 1.0;
}

double Random::randomDouble(double min, double max)
{
    return randomDouble() * (max-min) + min;
}

double Random::randomDouble(double scale)
{
    return randomDouble() * scale;
}

unsigned Random::randomDoubleInt(unsigned max)
{
    return randomBits() % max;
}

 double Random::randomBinomial(double scale)
{
    return (randomDouble()-randomDouble())*scale;
}

Quaternion Random::randomQuaternion()
{
    Quaternion q(
        randomDouble(),
        randomDouble(),
        randomDouble(),
        randomDouble()
        );
    q.normalize();
    return q;
}

Vector3D Random::randomVector(double scale)
{
    return Vector3D(
        randomBinomial(scale),
        randomBinomial(scale),
        randomBinomial(scale)
        );
}

Vector3D Random::randomXZVector(double scale)
{
    return Vector3D(
        randomBinomial(scale),
        0,
        randomBinomial(scale)
        );
}

Vector3D Random::randomVector(const Vector3D &scale)
{
    return Vector3D(
        randomBinomial(scale.x),
        randomBinomial(scale.y),
        randomBinomial(scale.z)
        );
}

Vector3D Random::randomVector(const Vector3D &min, const Vector3D &max)
{
    return Vector3D(
        randomDouble(min.x, max.x),
        randomDouble(min.y, max.y),
        randomDouble(min.z, max.z)
        );
}
