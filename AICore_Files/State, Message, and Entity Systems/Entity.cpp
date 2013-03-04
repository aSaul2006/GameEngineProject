/************************************************************************
*  Program Name:		Entity.cpp										*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Code file for Entity classes					*
*						Stores health, position, rotation, type, ID,	*
*						and a state machine for each entity				*
*  Update:																*
*																		*
************************************************************************/

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "Entity.h"

//-----------------------------------------------------------------------
// Initialize next valid ID to 0
//-----------------------------------------------------------------------
int Entity::m_iNextValidID = 0;

//-----------------------------------------------------------------------
// Entity base class
//-----------------------------------------------------------------------
// Entity function: default constructor
Entity::Entity()
{
	m_iHealth = 0;
	m_v2Position = D3DXVECTOR2(0.0f, 0.0f);
	m_fRotation = 0.0f;
	m_EntityType = NONE;
	SetID(m_iNextValidID);
}
	
// Entity function: parameterized constructor
Entity::Entity(int iHP, D3DXVECTOR2 v2Pos, float fRot, ENTITY_TYPE EntityType, int iID)
{
	m_iHealth = iHP;
	m_v2Position = v2Pos;
	m_fRotation = fRot;
	m_EntityType = EntityType;
	SetID(iID);
}

void Entity::ResetNextValidID()
{
	Entity::m_iNextValidID = 0;
}

// GetHealth function: returns the current health of entity
int Entity::GetHealth() const
{
	return m_iHealth;
}

// GetPos function: returns the current 2D vector position
D3DXVECTOR2 Entity::GetPos() const
{
	return m_v2Position;
}

// GetRot function: returns the current rotation
float Entity::GetRot() const
{
	return m_fRotation;
}

// GetEntityType function: returns the entity type
ENTITY_TYPE Entity::GetEntityType() const
{
	return m_EntityType;
}

// GetID function: returns the entity ID
int Entity::GetID() const
{
	return m_iID;
}

// DamageHealth function: subtracts a damage value from current health
void Entity::DamageHealth(int iDamage)
{
	// subtract the damage from health (negative damage will heal)
	m_iHealth -= iDamage;

	// make sure health stays within a maximum and 0
	if(m_iHealth < 0)
		m_iHealth = 0;

	switch(m_EntityType)
	{
	case PLAYER_ENTITY:
		{
			if(m_iHealth > g_iPlayerMaxHealth)
				m_iHealth = g_iPlayerMaxHealth;
			break;
		}
	case TEMPLATE_ENTITY:
		{
			if(m_iHealth > g_iTemplateEntityMaxHealth)
				m_iHealth = g_iTemplateEntityMaxHealth;
			break;
		}
	////////////////////////////////////////////////////////////////////////////////
	// NOTE: each entity type needs its own case because they each have different //
	//		 max health values													  //
	////////////////////////////////////////////////////////////////////////////////
	}
}

// SetPos function: sets a new 2D vector position
void Entity::SetPos(D3DXVECTOR2 v2Pos)
{
	m_v2Position = v2Pos;
}

// SetPosX function: sets a new X- position in the 2D vector position
void Entity::SetPosX(float fPosX)
{
	m_v2Position.x = fPosX;
}

// SetPosY function: sets a new Y- position in the 2D vector position
void Entity::SetPosY(float fPosY)
{
	m_v2Position.y = fPosY;
}

// SetID function: sets the next valid ID for a newly created entity
void Entity::SetID(int iNextID)
{
	if(iNextID >= m_iNextValidID)
	{
		m_iID = iNextID;
		m_iNextValidID++;
	}
}

// RotateEntity function: updates current rotation by adding radians
void Entity::RotateEntity(float fRadians)
{
	m_fRotation += fRadians;
}

// operator= overload allows entity health, position, rotation, and type to be copied
Entity& Entity::operator=(const Entity& other)
{
	this->m_iHealth = other.m_iHealth;
	this->m_v2Position = other.m_v2Position;
	this->m_fRotation = other.m_fRotation;
	this->m_EntityType = other.m_EntityType;

	return *this;
}

//-----------------------------------------------------------------------
// TemplateEntity sub class
//-----------------------------------------------------------------------
// TemplateEntity function: default constructor
TemplateEntity::TemplateEntity() : Entity()
{
	m_pStateMachine = new StateMachine<TemplateEntity>(this, TemplateEntity_InitState::Instance());
}

// PlayerEntity function: parameterized constructor
TemplateEntity::TemplateEntity(int iHP, D3DXVECTOR2 v2Pos, float fRot, ENTITY_TYPE EntityType, int iID) : Entity(iHP, v2Pos, fRot, EntityType, iID)
{
	m_pStateMachine = new StateMachine<TemplateEntity>(this, TemplateEntity_InitState::Instance());
}

// ~PlayerEntity function: default destructor
TemplateEntity::~TemplateEntity()
{
	delete m_pStateMachine;
}

// Update function: runs entity's state logic every frame, returns true if entity was deleted/killed during this update call
bool TemplateEntity::Update()
{
	return m_pStateMachine->Update();
}

// HandleMessage function: sends message through state machine to change states
bool TemplateEntity::HandleMessage(const Message& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

// GetFSM function: returns a pointer to this entity's state machine
StateMachine<TemplateEntity>* TemplateEntity::GetFSM() const
{
	return m_pStateMachine;
}

	/////////////////////////////////////////////////////////////////////////////////////
	// NOTE: Add a class for each ENTITY_TYPE (use TemplateEntity class as a template) //
	/////////////////////////////////////////////////////////////////////////////////////