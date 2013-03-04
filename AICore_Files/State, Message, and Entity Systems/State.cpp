/************************************************************************
*  Program Name:		State.cpp										*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Code file for State classes						*
*						Controls what the AI does in each state			*
*						All sub state classes are singleton design		*
*  Update:																*
*																		*
************************************************************************/

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "State.h"
#include "MessageSystem.h"

//-----------------------------------------------------------------------
// Initialize all state's Instance of themselves
//-----------------------------------------------------------------------
TemplateEntity_InitState* TemplateEntity_InitState::m_pInstance = 0;
TemplateEntity_RoamState* TemplateEntity_RoamState::m_pInstance = 0;
TemplateEntity_AttackState* TemplateEntity_AttackState::m_pInstance = 0;
TemplateEntity_DeadState* TemplateEntity_DeadState::m_pInstance = 0;

		////////////////////////////////////////////////////
		// NOTE: Every state should have its own Instance //
		////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// TemplateEntity initialization state sub class
//-----------------------------------------------------------------------
// Instance function: ensures only a single instance of this class exists
TemplateEntity_InitState* TemplateEntity_InitState::Instance()
{
	if(!m_pInstance)
		m_pInstance = new TemplateEntity_InitState;

	return m_pInstance;
}

// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
bool TemplateEntity_InitState::Execute(TemplateEntity* Entity)
{
	// if a delayed message has been created
	if(m_bDelayedMessageDispatched)
		MessageDispatcher::Instance()->DispatchDelayedMessages(); // check the priorityQ to see if any need to be dispatched

	// Send a MSG_ROAM message to self with no delay
	MessageDispatcher::Instance()->DispatchMessage(0.0f, Entity->GetID(), Entity->GetID(), MSG_ROAM);

	// The only time m_bDelayedMessageDispatched should be true is when DispatchMessage is called with a delay
	// Example: MessageDispatcher::Instance()->DispatchMessage(5.5f, Entity->GetID(), Entity->GetID(), MSG_ROAM);
	//			m_bDelayedMessageDispatched = true;

	// This Execute cannot delete an entity, always return false
	return false;
}

// OnMessage function: runs a set of commands when a message is received
bool TemplateEntity_InitState::OnMessage(TemplateEntity* Entity, const Message& Msg)
{
	// read the message
	switch(Msg.m_iMsg)
	{
	case MSG_ROAM: // if message is MSG_ROAM
		{
			// then change states to StateRoam
			Entity->GetFSM()->ChangeState(TemplateEntity_RoamState::Instance());
		}
		///////////////////////////////////////////////////////////////
		// NOTE: add a case for each possible message to be received //
		///////////////////////////////////////////////////////////////

		// always set delayed message dispatched to false after reading a message
		// because whether it was delayed or not it has now been read
		m_bDelayedMessageDispatched = false;

		// we read a message, return true
		return true;
	}

	// a message was not read, return false
	return false;
}

//-----------------------------------------------------------------------
// TemplateEntity roam state sub class
//-----------------------------------------------------------------------
// Instance function: ensures only a single instance of this class exists
TemplateEntity_RoamState* TemplateEntity_RoamState::Instance()
{
	if(!m_pInstance)
		m_pInstance = new TemplateEntity_RoamState;

	return m_pInstance;
}

// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
bool TemplateEntity_RoamState::Execute(TemplateEntity* Entity)
{
	// if a delayed message has been created
	if(m_bDelayedMessageDispatched)
		MessageDispatcher::Instance()->DispatchDelayedMessages(); // check the priorityQ to see if any need to be dispatched

		///////////////////////////////////
		/* NOTE: RUN STEERING LOGIC HERE //
		///////////////////////////////////

	Entity->SetPosX(fNewPosX);
	Entity->SetPosY(fNewPosY);
	Entity->RotateEntity(fRadians);

	if(player is nearby)
		// Send a MSG_ATTACK message to self with no delay
		MessageDispatcher::Instance()->DispatchMessage(0.0f, Entity->GetID(), Entity->GetID(), MSG_ATTACK);
	*/

	// if entity's health drops to 0
	if(Entity->GetHealth() <= 0)
		// Send a MSG_DEAD message to self with no delay
		MessageDispatcher::Instance()->DispatchMessage(0.0f, Entity->GetID(), Entity->GetID(), MSG_DEAD);

	// Even though this Execute realizes the entity is dead, it still does not actually delete the entity
	// Always return false
	return false;
}

// OnMessage function: runs a set of commands when a message is received
bool TemplateEntity_RoamState::OnMessage(TemplateEntity* Entity, const Message& Msg)
{
	// read the message
	switch(Msg.m_iMsg)
	{
	case MSG_ATTACK: // if message is MSG_ATTACK
		{
			// then change states to StateAttack
			Entity->GetFSM()->ChangeState(TemplateEntity_AttackState::Instance());
		}
	case MSG_DEAD: // if message is MSG_DEAD
		{
			// then change states to StateDead
			Entity->GetFSM()->ChangeState(TemplateEntity_DeadState::Instance());
		}
		///////////////////////////////////////////////////////////////
		// NOTE: add a case for each possible message to be received //
		///////////////////////////////////////////////////////////////

		// always set delayed message dispatched to false after reading a message
		// because whether it was delayed or not it has now been read
		m_bDelayedMessageDispatched = false;

		// we read a message, return true
		return true;
	}

	// a message was not read, return false
	return false;
}

//-----------------------------------------------------------------------
// TemplateEntity attack state sub class
//-----------------------------------------------------------------------
// Instance function: ensures only a single instance of this class exists
TemplateEntity_AttackState* TemplateEntity_AttackState::Instance()
{
	if(!m_pInstance)
		m_pInstance = new TemplateEntity_AttackState;

	return m_pInstance;
}

// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
bool TemplateEntity_AttackState::Execute(TemplateEntity* Entity)
{
	// if a delayed message has been created
	if(m_bDelayedMessageDispatched)
		MessageDispatcher::Instance()->DispatchDelayedMessages(); // check the priorityQ to see if any need to be dispatched

		///////////////////////////////////
		/* NOTE: RUN STEERING LOGIC HERE //
		///////////////////////////////////

	Entity->SetPosX(fNewPosX);
	Entity->SetPosY(fNewPosY);
	Entity->RotateEntity(fRadians);

	if(player is too far away)
		// Send a MSG_ROAM message to self with no delay
		MessageDispatcher::Instance()->DispatchMessage(0.0f, Entity->GetID(), Entity->GetID(), MSG_ROAM);
	*/

	// if entity's health drops to 0
	if(Entity->GetHealth() <= 0)
		// Send a MSG_DEAD message to self with no delay
		MessageDispatcher::Instance()->DispatchMessage(0.0f, Entity->GetID(), Entity->GetID(), MSG_DEAD);

	// Even though this Execute realizes the entity is dead, it still does not actually delete the entity
	// Always return false
	return false;
}

// OnMessage function: runs a set of commands when a message is received
bool TemplateEntity_AttackState::OnMessage(TemplateEntity* Entity, const Message& Msg)
{
	// read the message
	switch(Msg.m_iMsg)
	{
	case MSG_ROAM: // if message is MSG_ROAM
		{
			// then change states to StateRoam
			Entity->GetFSM()->ChangeState(TemplateEntity_RoamState::Instance());
		}
	case MSG_DEAD: // if message is MSG_DEAD
		{
			// then change states to StateDead
			Entity->GetFSM()->ChangeState(TemplateEntity_DeadState::Instance());
		}
		///////////////////////////////////////////////////////////////
		// NOTE: add a case for each possible message to be received //
		///////////////////////////////////////////////////////////////

		// always set delayed message dispatched to false after reading a message
		// because whether it was delayed or not it has now been read
		m_bDelayedMessageDispatched = false;

		// we read a message, return true
		return true;
	}

	// a message was not read, return false
	return false;
}

//-----------------------------------------------------------------------
// TemplateEntity dead state sub class
//-----------------------------------------------------------------------
// Instance function: ensures only a single instance of this class exists
TemplateEntity_DeadState* TemplateEntity_DeadState::Instance()
{
	if(!m_pInstance)
		m_pInstance = new TemplateEntity_DeadState;

	return m_pInstance;
}

// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
bool TemplateEntity_DeadState::Execute(TemplateEntity* Entity)
{
	// if a delayed message has been created
	if(m_bDelayedMessageDispatched)
		MessageDispatcher::Instance()->DispatchDelayedMessages(); // check the priorityQ to see if any need to be dispatched

	// Dead state does nothing except send a message out so its Exit function is called to delete the entity
	// Send a MSG_DELETE message to self with no delay
	MessageDispatcher::Instance()->DispatchMessage(0.0f, Entity->GetID(), Entity->GetID(), MSG_DELETE);

	// If this Execute has been called, the entity is being deleted
	// Always return true
	return true;
}

// Exit function: runs a set of commands when a state is exited
void TemplateEntity_DeadState::Exit(TemplateEntity* Entity)
{
	EntityManager::Instance()->RemoveEntity(Entity);
}

// OnMessage function: runs a set of commands when a message is received
bool TemplateEntity_DeadState::OnMessage(TemplateEntity* Entity, const Message& Msg)
{
	// read the message
	switch(Msg.m_iMsg)
	{
	case MSG_DELETE: // if message is MSG_DELETE
		{
			// then change states to anything so its Exit function is called
			Entity->GetFSM()->ChangeState(TemplateEntity_InitState::Instance());
		}
		///////////////////////////////////////////////////////////////
		// NOTE: add a case for each possible message to be received //
		///////////////////////////////////////////////////////////////

		// always set delayed message dispatched to false after reading a message
		// because whether it was delayed or not it has now been read
		m_bDelayedMessageDispatched = false;

		// we read a message, return true
		return true;
	}

	// a message was not read, return false
	return false;
}