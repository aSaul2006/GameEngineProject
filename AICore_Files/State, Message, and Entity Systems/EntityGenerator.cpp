/************************************************************************
*  Program Name:		EntityGenerator.cpp								*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Code class for EntityGenerator					*
*						Creates new entities for EntityManager			*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "EntityGenerator.h"

//-----------------------------------------------------------------------
// Initialize EntityGenerator Instance of self
//-----------------------------------------------------------------------
EntityGenerator* EntityGenerator::m_pInstance = 0;

//-----------------------------------------------------------------------
// EntityGenerator class (singleton design)
//-----------------------------------------------------------------------
// EntityGenerator function: default constructor
EntityGenerator::EntityGenerator()
{
	srand(time(0));
}

// Instance function: ensures only a single instance of this class exists
EntityGenerator* EntityGenerator::Instance()
{
	if(!m_pInstance)
		m_pInstance = new EntityGenerator;

	return m_pInstance;
}

// CreatePlayerEntity function: creates a player entity and sends it to the EntityManager
PlayerEntity* EntityGenerator::CreatePlayerEntity()
{
	// variables to make code more readable
	D3DXVECTOR2 v2Pos = D3DXVECTOR2(0.0f, 0.0f);
	float fRot = 0.0f;
	int iID = EntityManager::Instance()->GetNumberOfTotalEntities();

	PlayerEntity* entity = new PlayerEntity(g_iPlayerMaxHealth, v2Pos, fRot, PLAYER_ENTITY, iID);
	return entity;
}

// CreateTemplateEntity function: created a basic entity and sends it to the EntityManager
TemplateEntity* EntityGenerator::CreateTemplateEntity()
{
	// variables to make code more readable						/////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR2 v2Pos = D3DXVECTOR2(rand()%801, rand()%601);	// NOTE: position needs to be changed to a specific location in the game world //
	float fRot = 0.0f;											/////////////////////////////////////////////////////////////////////////////////
	int iID = EntityManager::Instance()->GetNumberOfTotalEntities();

	TemplateEntity* entity = new TemplateEntity(g_iTemplateEntityMaxHealth, v2Pos, fRot, TEMPLATE_ENTITY, iID);
	return entity;
}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// NOTE: each ENTITY_TYPE requires its own create function because it must return an instance of that entity class //
	// Use the CreateTemplateEntity function as a template																   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////