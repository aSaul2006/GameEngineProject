/************************************************************************
*  Program Name:		EntityManager.h									*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Header file for EntityManager class				*
*						Keeps track of and stores all entities			*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

#pragma once

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "Entity.h"
#include <map>

//-----------------------------------------------------------------------
// EntityManager class (singleton design)
//-----------------------------------------------------------------------
class EntityManager
{
private:
	// EntityManager function: default constructor
	EntityManager();
	// EntityManager function: copy constructor
	EntityManager(EntityManager const&) {};
	// operator= overload so instance can't be copied
	EntityManager& operator=(EntityManager const&) {};

	static EntityManager* m_pInstance;			// static pointer to own class

	typedef std::map<int, Entity*> EntityMap;	// define private map datatype
	EntityMap m_EntityMap;						// stores all entities
	int m_iActiveEntities;						// stores the number of active entities
	int m_iTotalEntities;						// stores the number of total entities created
	int m_iDeletedEntities;						// stores the number of entities that have been deleted

public:
	// Instance function: ensures only a single instance of this class exists
	static EntityManager* Instance();

	// RegisterEntity function: adds a new entity to the entity map
	void RegisterEntity(Entity* NewEntity);

	// GetEntityFromID function: returns an entity based on the ID provided
	Entity* GetEntityFromID(int iID) const;

	// RemoveEntity function: deletes an entity from the entity map
	void RemoveEntity(Entity* entity);
	// RemoveAllEntities function: deletes all entities from the EntityMap
	void RemoveAllEntities();

	// GetNumberOfActiveEntities function: returns the number of active entities
	int GetNumberOfActiveEntities() const;
	// GetNumberOfTotalEntities function: returns the number of total entities
	int GetNumberOfTotalEntities() const;
	// GetNumberOfDeletedEntities function: returns the number of entities that have been deleted
	int GetNumberOfDeletedEntities() const;

	// GetAllEntityPositions function: loops through EntityMap storing all active entity's positions in an array
	D3DXVECTOR2* GetAllEntityPositions();
	// GetAllEntityOrientations function: loops through EntityMap storing all active entity's orientations in an array
	float* GetAllEntityOrientations();

	// UpdateAllEntities function: loops through EntityMap and runs each one's Update function
	void UpdateAllEntities();
};