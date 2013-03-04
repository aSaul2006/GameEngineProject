/*****************************************************************************************
*                                                                                        *
*  Program Name:          PhysWorld.h                                                    *
*  Name:                  Jennifer Wickensimer                                           *
*  Date:                  January 23, 2012                                               *
*  Description:           World class function file - allows interactions between cores  *
*                                                                                        *
*  Update:                Jennifer Wickensimer, 01/28/2012                               *
*                                                                                        *
*****************************************************************************************/

#include <cstdlib>
#include "physicsengine\PhysWorld.h"

using namespace PhysicsEngine;

World::World(unsigned m_wMaxContacts, unsigned wIterations)
:
firstBody(NULL),
firstContactGen(NULL),
resolver(wIterations),
m_wMaxContacts(m_wMaxContacts)
{
	contacts = new Contact[m_wMaxContacts];
	b_mCalculateInterations = (wIterations == 0);
}


/*World::World()
{
	World  *pWorld;  // added for physicsmanager trying something different
}
*/

World::~World()

{
	delete[] contacts;
}


void World::startFrame()
{
	BodyRegistration *reg = firstBody;
	while (reg)
	{
		//remove all accumlated forces
		reg->body->emptyAccumulators();
		reg->body->calculateObjectData();

		//go to the next reg
		reg = reg->next;

	}
}

unsigned World::generateContacts()
{
	unsigned wLimite = m_wMaxContacts;
	Contact *nextContact = contacts;

	ContactGenRegistration * reg = firstContactGen;
	while (reg)
	{
		unsigned used = reg->gen->addContact(nextContact, wLimite);
		wLimite -=used;
		nextContact += used;
		

		if (wLimite <=0) break;
		
		reg = reg->next;
	}

	//send back the number of contacts used
	return m_wMaxContacts - wLimite;

}


void World::runPhysics(double dElapsedFrameTime)
{
	//apply the force generators
	//registry.updateForces(dElapsedFrameTime);

	//Integrate the objects
	BodyRegistration *reg = firstBody;

	while (reg)
	{
		//remove all forces the the accumulator
		reg->body->physicsIntegration(dElapsedFrameTime);

		//Next
		reg = reg->next;
	}

	//generate contacts
	unsigned wUsedContacts = generateContacts();

	//process the bloody things
	if (b_mCalculateInterations) resolver.determineIterations(wUsedContacts * 4);
	resolver.resolveContact(contacts, wUsedContacts, dElapsedFrameTime);

}

		
/*get the needed information from the main core
void WorldWrapper::getDataFromCore(Vector3D velocity,
							       Vector3D position,
							       int bodyID)

{

	m_v3Velocity = velocity;
	m_v3Position = position;
	m_iBodyID = bodyID;

}




unsigned WorldWrapper::sendResultsToCore()
{
	return (m_v3Velocity, m_v3Position, m_iBodyID, m_bCollisionOccurred);
}
*/