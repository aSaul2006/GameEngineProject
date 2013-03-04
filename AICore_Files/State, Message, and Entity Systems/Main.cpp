/************************************************************************
*  Program Name:		Main.cpp										*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Code file for Main function						*
*						Tests State, Message, and Entity features		*
*  Update:																*
*																		*
************************************************************************/

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include <iostream>
#include <conio.h>
#include "EntityGenerator.h"

using namespace std;

//-----------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------
int main()
{
	// create a player entity for testing
	// always create the player entity first so it is the first position in the EntityMap
	EntityManager::Instance()->RegisterEntity(EntityGenerator::Instance()->CreatePlayerEntity());
	// create some template entities for testing
	EntityManager::Instance()->RegisterEntity(EntityGenerator::Instance()->CreateTemplateEntity());
	EntityManager::Instance()->RegisterEntity(EntityGenerator::Instance()->CreateTemplateEntity());
	EntityManager::Instance()->RegisterEntity(EntityGenerator::Instance()->CreateTemplateEntity());

	// call this function every frame in the Main core's Update function
	EntityManager::Instance()->UpdateAllEntities();

	// this next block of code shows how to get the positions and orientations of all the entities
	// (including the player which should always be the first position in the array)
	// to be sent to the Rendering core for drawing
	int iActiveEntities = EntityManager::Instance()->GetNumberOfActiveEntities();
	D3DXVECTOR2* v2EntityPositions = EntityManager::Instance()->GetAllEntityPositions();
	float* fEntityOrientations = EntityManager::Instance()->GetAllEntityOrientations();
	for(int i = 0; i < iActiveEntities; i++)
	{
		cout << "Position (" << v2EntityPositions[i].x << ", " << v2EntityPositions[i].y << ") Orientation (in radians): " << fEntityOrientations[i] << endl;
	}

	// use this function to clear the entity map and reset the entity counters before each level
	EntityManager::Instance()->RemoveAllEntities();

	////////////////////////////////////////////////////////////////////////////////
	// Many things still need to be done to get this working for the AI Core	  //
	// Use the Find feature (ctrl+F) to locate the word "NOTE"					  //
	// They will all be inside a box of forward slashes like this comment section //
	// Many notes are for the user of the engine itself in order for			  //
	// unique Entities and States to be created									  //
	// Other notes are for the AI core to add their code						  //
	//																			  //
	// Notes can be found on: Entity.h, Entity.cpp, EntityGenerator.h,			  //
	// EntityGenerator.cpp, State.h, State.cpp, MessageSystem.h					  //
	////////////////////////////////////////////////////////////////////////////////




	// Exit functionality which requires the user to press any key
	cout << "\nPress any key to continue...";
	cin.sync();
	_getch();
	return 0;
}