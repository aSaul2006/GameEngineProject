/************************************************************************
*  Program Name:		StateMachine.h									*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Header file for StateMachine class				*
*						Runs the State logic							*
*  Update:																*
*																		*
************************************************************************/

#pragma once

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "State.h"

//-----------------------------------------------------------------------
// Template class
//-----------------------------------------------------------------------
template <class entity_class>
// Template causes linker errors when used in a .cpp file (I'm not really sure why),
// therefore implementation for StateMachine functions is done in the header class

//-----------------------------------------------------------------------
// StateMachine class
//-----------------------------------------------------------------------
class StateMachine
{
private:
	entity_class* m_pOwner;					// pointer to the Entity that owns this instance

	State<entity_class>* m_pCurrentState;	// pointer to the current state the Owner is in

public:
	// StateMachine function: parameterized constructor
	StateMachine(entity_class* Self, State<entity_class>* InitState) : m_pOwner(Self), m_pCurrentState(InitState) {};

	// ChangeState function: runs current State's Exit function, changes to new State, and runs new State's Enter function
	void ChangeState(State<entity_class>* NewState)
	{
		// make sure both states are valid
		assert(m_pCurrentState && NewState);

		m_pCurrentState->Exit(m_pOwner);

		m_pCurrentState = NewState;

		m_pCurrentState->Enter(m_pOwner);
	};

	// Update function: run current state's Execute function, returns true if entity was deleted/killed during this update call
	bool Update() const
	{
		// only update if a current state exists
		if(m_pCurrentState)
			return m_pCurrentState->Execute(m_pOwner);

		// if no current state exists, then always return false
		return false;
	};

	// HandleMessage function: sends a message to the State's OnMessage function to be handled
	bool HandleMessage(const Message& Msg) const
	{
		if(m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, Msg))
			return true;

		return false;
	};
};