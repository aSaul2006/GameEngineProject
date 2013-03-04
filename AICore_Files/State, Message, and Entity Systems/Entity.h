/************************************************************************
*  Program Name:		Entity.h										*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Header file for Entity classes					*
*						Stores health, position, rotation, type, ID,	*
*						and a state machine for each entity				*
*  Update:																*
*																		*
************************************************************************/

#pragma once

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <assert.h>
#include "StateMachine.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

//-----------------------------------------------------------------------
// Entity type identifiers
//-----------------------------------------------------------------------
															//////////////////////////////////////////////////////
enum ENTITY_TYPE {NONE, PLAYER_ENTITY, TEMPLATE_ENTITY};	/* NOTE: for each new entity add an identifier here */ 
															//////////////////////////////////////////////////////
// NONE is used as a default for constructors
// TEMPLATE_ENTITY is to be used as an example for all new entity types

//-----------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------
static int g_iPlayerMaxHealth = 100;
static const int g_iTemplateEntityMaxHealth = 30;
		///////////////////////////////////////////////////////////////////////////
		// NOTE: each entity should have a variable to define its maximum health //
		///////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Entity base class
//-----------------------------------------------------------------------
class Entity
{
private:
	int m_iHealth;				// stores the health as an integer
	D3DXVECTOR2 m_v2Position;	// stores the position as a 2D vector
	float m_fRotation;			// stores the rotation in radians as a float
	ENTITY_TYPE m_EntityType;	// stores the entity's type
	int m_iID;					// stores the entity's ID so it can be accessed by it

	static int m_iNextValidID;	// stores the next valid ID to be used for an entity's creation

	// SetID function: sets the next valid ID for a newly created entity
	void SetID(int iNextID);

public:
	// Entity function: default constructor
	Entity();
	// Entity function: parameterized constructor
	Entity(int iHP, D3DXVECTOR2 v2Pos, float fRot, ENTITY_TYPE EntityType, int iID);
	// ~Entity function: default destructor
	virtual ~Entity() {};

	// ResetNextValidID function: resets m_iNextValidID back to 0 after EntityManager's RemoveAllEntities function is called
	static void ResetNextValidID();

	// GetHealth function: returns the current health of entity
	int GetHealth() const;
	// GetPos function: returns the current 2D vector position
	D3DXVECTOR2 GetPos() const;
	// GetRot function: returns the current rotation
	float GetRot() const;
	// GetEntityType function: returns the entity type
	ENTITY_TYPE GetEntityType() const;
	// GetID function: returns the entity ID
	int GetID() const;

	// DamageHealth function: subtracts a damage value from current health
	void DamageHealth(int iDamage);
	// SetPos function: sets a new 2D vector position
	void SetPos(D3DXVECTOR2 v2Pos);
	// SetPosX function: sets a new X- position in the 2D vector position
	void SetPosX(float fPosX);
	// SetPosY function: sets a new Y- position in the 2D vector position
	void SetPosY(float fPosY);
	// RotateEntity function: updates current rotation by adding some amount of radians
	void RotateEntity(float fRadians);

	// Update function: runs entity's state logic every frame, returns true if entity was deleted/killed during this update call
	virtual bool Update() = 0;
	// HandleMessage function: sends message through state machine to change states
	virtual bool HandleMessage(const Message& msg) = 0;

	// operator= overload allows entity health, position, rotation, and type to be copied
	Entity& operator=(const Entity&);
};

//-----------------------------------------------------------------------
// PlayerEntity sub class
//-----------------------------------------------------------------------
class PlayerEntity : public Entity
{
public:
	// PlayerEntity function: default constructor
	PlayerEntity() : Entity() {};
	// PlayerEntity function: parameterized constructor
	PlayerEntity(int iHP, D3DXVECTOR2 v2Pos, float fRot, ENTITY_TYPE EntityType, int iID) : Entity(iHP, v2Pos, fRot, EntityType, iID) {};
	// ~PlayerEntity function: default destructor
	~PlayerEntity() {};

	// Update function: runs entity's state logic every frame, returns true if entity was deleted/killed during this update call
	bool Update() {return false;};
	// HandleMessage function: sends message through state machine to change states
	bool HandleMessage(const Message& msg) {return false;};

	// None of these functions will be used because the player is not controlled by a state machine
	// This class is here simply for a way to track and update player variables
};

//-----------------------------------------------------------------------
// TemplateEntity sub class
//-----------------------------------------------------------------------
class TemplateEntity : public Entity
{
private:
	 StateMachine<TemplateEntity>* m_pStateMachine; // stores a state machine instance for this type of entity

public:
	// TemplateEntity function: default constructor
	TemplateEntity();
	// TemplateEntity function: parameterized constructor
	TemplateEntity(int iHP, D3DXVECTOR2 v2Pos, float fRot, ENTITY_TYPE EntityType, int iID);
	// ~TemplateEntity function: default destructor
	~TemplateEntity();

	// Update function: runs entity's state logic every frame, returns true if entity was deleted/killed during this update call
	bool Update();
	// HandleMessage function: sends message through state machine to change states
	bool HandleMessage(const Message& msg);

	// GetFSM function: returns a pointer to this entity's state machine
	StateMachine<TemplateEntity>* GetFSM() const;
};

	/////////////////////////////////////////////////////////////////////////////////////
	// NOTE: Add a class for each ENTITY_TYPE (use TemplateEntity class as a template) //
	/////////////////////////////////////////////////////////////////////////////////////