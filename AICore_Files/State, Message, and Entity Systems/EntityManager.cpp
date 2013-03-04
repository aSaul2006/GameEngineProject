/************************************************************************
*  Program Name:		EntityManager.cpp								*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Code file for EntityManager class				*
*						Keeps track of and stores all entities			*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "EntityManager.h"

//-----------------------------------------------------------------------
// Initialize EntityManager Instance of self
//-----------------------------------------------------------------------
EntityManager* EntityManager::m_pInstance = 0;

//-----------------------------------------------------------------------
// EntityManager class (singleton design)
//-----------------------------------------------------------------------
// EntityManager function: default constructor
EntityManager::EntityManager()
{
	m_iActiveEntities = 0;
	m_iTotalEntities = 0;
	m_iDeletedEntities = 0;
}

// Instance function: ensures only a single instance of this class exists
EntityManager* EntityManager::Instance()
{
	if(!m_pInstance)
		m_pInstance = new EntityManager;

	return m_pInstance;
}

// RegisterEntity function: adds a new entity to the entity map
void EntityManager::RegisterEntity(Entity* NewEntity)
{
	m_EntityMap[NewEntity->GetID()] = NewEntity;
//	m_EntityMap.insert(std::make_pair(NewEntity->GetID(), NewEntity));
	m_iActiveEntities++;
	m_iTotalEntities++;
}

// GetEntityFromID function: returns an entity based on the ID provided
Entity* EntityManager::GetEntityFromID(int iID) const
{
	EntityMap::const_iterator ent = m_EntityMap.find(iID);

	assert((ent != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

	return ent->second;
}

// RemoveEntity function: deletes an entity from the entity map
void EntityManager::RemoveEntity(Entity* entity)
{
	m_EntityMap.erase(m_EntityMap.find(entity->GetID()));
	m_iActiveEntities--;
	m_iDeletedEntities++;
}

// RemoveAllEntities function: deletes all entities from the EntityMap
void EntityManager::RemoveAllEntities()
{
	m_EntityMap.clear();

	Entity::ResetNextValidID();
	m_iActiveEntities = 0;
	m_iTotalEntities = 0;
	m_iDeletedEntities = 0;
}

// GetNumberOfActiveEntities function: returns the number of active entities
int EntityManager::GetNumberOfActiveEntities() const
{
	return m_iActiveEntities;
}

// GetNumberOfTotalEntities function: returns the number of total entities
int EntityManager::GetNumberOfTotalEntities() const
{
	return m_iTotalEntities;
}

// GetNumberOfDeletedEntities function: returns the number of entities that have been deleted
int EntityManager::GetNumberOfDeletedEntities() const
{
	return m_iDeletedEntities;
}

// GetAllEntityPositions function: loops through EntityMap storing all active entity's positions in an array
D3DXVECTOR2* EntityManager::GetAllEntityPositions()
{
	D3DXVECTOR2* v2EntityPositionArray = new D3DXVECTOR2[m_iActiveEntities];
	int i = 0;

	for(EntityMap::iterator it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		v2EntityPositionArray[i] = it->second->GetPos();
		i++;
	}

	return v2EntityPositionArray;
}

// GetAllEntityOrientations function: loops through EntityMap storing all active entity's orientations in an array
float* EntityManager::GetAllEntityOrientations()
{
	float* fEntityOrientationArray = new float[m_iActiveEntities];
	int i = 0;

	for(EntityMap::iterator it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		fEntityOrientationArray[i] = it->second->GetRot();
		i++;
	}

	return fEntityOrientationArray;
}

// UpdateAllEntities function: loops through EntityMap and runs each one's Update function
void EntityManager::UpdateAllEntities()
{
	EntityMap::iterator itPrevious;

	for(EntityMap::iterator it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		if(it->second->Update())
			it = itPrevious;

		itPrevious = it;
	}
}