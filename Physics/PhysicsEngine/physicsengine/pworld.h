/***************************************************************************************
*  Program Name:    pworld.h                                                           *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 08, 2013                                                  *
*  Description:     This program holds the information necessary for the Physics Core  *
*                    to use,  this is done through the PhysWorld entity                *
*                   Interface file for the particle or mass aggregate world            *
*                           structure.                                                 *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * Interface file for the particle / mass aggregate world structure.
 */


#ifndef PHYSICSENGINE_PWORLD_H_
#define PHYSICSENGINE_PWORLD_H_


#include "pfgen.h"
#include "plinks.h"


/**
 * This file contains the definitions for a structure to hold any number o
 * particle masses, and their connections.
 */

namespace PhysicsEngine
{
	/*Keeps track of a set of particles, and provides the means to
     * update them all.
     */
    class ParticleWorld
    {
    public:
        typedef std::vector<Particle*> Particles;
        typedef std::vector<ParticleContactGenerator*> ContactGenerators;

    protected:
        /**
         * Holds the particles
         */
        Particles particles;

        /**
         * True if the world should calculate the number of iterations
         * to give the contact resolver at each frame.
         */
        bool calculateIterations;

        /**
         * Holds the force generators for the particles in this world.
         */
        ParticleForceRegistry registry;

        /**
         * Holds the resolver for contacts.
         */
        ParticleContactResolver resolver;

        /**
         * Contact generators.
         */
        ContactGenerators contactGenerators;

        /**
         * Holds the list of contacts.
         */
        ParticleContact *contacts;

        /**
         * Holds the maximum number of contacts allowed (i.e. the
         * size of the contacts array).
         */
        unsigned maxContacts;

    public:

        /**
         * Creates a new particle simulator that can handle up to the
         * given number of contacts per frame. You can also optionally
         * give a number of contact-resolution iterations to use. If you
         * don't give a number of iterations, then twice the number of
         * contacts will be used.
         */
        ParticleWorld(unsigned maxContacts, unsigned iterations=0);

        /**
         * Deletes the simulator.
         */
        ~ParticleWorld();

        /**
         * Calls each of the registered contact generators to report
         * their contacts. Returns the number of generated contacts.
         */
        unsigned generateContacts();

        /**
         * Integrates all the particles in this world forward in time
         * by the given duration.
         */
        void integrate(double dElapsedFrameTime);

        /**
         * Processes all the physics for the particle world.
         */
        void runPhysics(double dElapsedFrameTime);

        /**
         * Initializes the world for a simulation frame. This clears
         * the force accumulators for particles in the world. After
         * calling this, the particles can have their forces for this
         * frame added.
         */
        void startFrame();

        /**
         *  Returns the list of particles.
         */
        Particles& getParticles();

        /**
         * Returns the list of contact generators.
         */
        ContactGenerators& getContactGenerators();

        /**
         * Returns the force registry.
         */
        ParticleForceRegistry& getForceRegistry();
    };

    /**
      * A contact generator that takes an STL vector of particle pointers and
     * collides them against the ground.
     */
    class GroundContacts : public PhysicsEngine::ParticleContactGenerator
    {
        PhysicsEngine::ParticleWorld::Particles *particles;

    public:
        void init(PhysicsEngine::ParticleWorld::Particles *particles);

        virtual unsigned addContact(PhysicsEngine::ParticleContact *contact,
            unsigned limit) const;
    };

} 

#endif 