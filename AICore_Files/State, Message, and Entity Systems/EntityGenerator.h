/************************************************************************
*  Program Name:		EntityGenerator.h								*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Header class for EntityGenerator				*
*						Creates new entities for EntityManager			*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

#pragma once

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "EntityManager.h"

//-----------------------------------------------------------------------
// EntityGenerator class (singleton design)
//-----------------------------------------------------------------------
class EntityGenerator
{
private:
	// EntityGenerator function: default constructor
	EntityGenerator();
	// EntityGenerator function: copy constructor
	EntityGenerator(EntityGenerator const&) {};
	// operator= overload so instance can't be copied
	EntityGenerator& operator=(EntityGenerator const&) {};

	static EntityGenerator* m_pInstance;			// static pointer to own class

public:
	// Instance function: ensures only a single instance of this class exists
	static EntityGenerator* Instance();

	// CreatePlayerEntity function: creates a player entity and sends it to the EntityManager
	PlayerEntity* CreatePlayerEntity();
	// CreateTemplateEntity function: created a basic entity and sends it to the EntityManager
	TemplateEntity* CreateTemplateEntity();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// NOTE: each ENTITY_TYPE requires its own create function because it must return an instance of that entity class //
	// Use the CreateTemplateEntity function as a template																   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};