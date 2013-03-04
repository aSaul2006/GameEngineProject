/************************************************************************
*  Program Name:		State.h											*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Header file for State classes					*
*						Controls what the AI does in each state			*
*						All state sub classes use singleton design		*
*  Update:																*
*																		*
************************************************************************/

#pragma once

//-----------------------------------------------------------------------
// Predeclaration classes
//-----------------------------------------------------------------------
struct Message;
class PlayerEntity;
class TemplateEntity;
/////////////////////////////////////////////////////
// NOTE: Add a predeclaration for each ENTITY_TYPE //
/////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Template class
//-----------------------------------------------------------------------
template <class entity_class>

//-----------------------------------------------------------------------
// State base class
//-----------------------------------------------------------------------
class State
{
public:
	bool m_bDelayedMessageDispatched;	// stores whether or not a delayed message has been created

	// ~State function: default destructor
	virtual ~State() {};
	// Enter function: runs a set of commands when a state is entered
	virtual void Enter(entity_class* Entity) = 0;
	// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
	virtual bool Execute(entity_class* Entity) = 0;
	// Exit function: runs a set of commands when a state is exited
	virtual void Exit(entity_class* Entity) = 0;
	// OnMessage function: runs a set of commands when a message is received
	virtual bool OnMessage(entity_class* Entity, const Message& msg) = 0;
};

		///////////////////////////////////////////////////////////////////////////////
		// NOTE: THESE ARE JUST GENERIC STATES TO USE AS TEMPLATES FOR CUSTOM STATES //
		///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// TemplateEntity initialization state sub class
//-----------------------------------------------------------------------
class TemplateEntity_InitState : public State<TemplateEntity>
{
private:
	// TemplateEntity_InitState function: default constructor
	TemplateEntity_InitState() {m_bDelayedMessageDispatched = false;};
	// TemplateEntity_InitState function: copy constructor
	TemplateEntity_InitState(TemplateEntity_InitState const&) {};
	// operator= overload so instance can't be copied
	TemplateEntity_InitState& operator=(TemplateEntity_InitState const&) {};

	static TemplateEntity_InitState* m_pInstance;			// static pointer to own class

public:
	// Instance function: ensures only a single instance of this class exists
	static TemplateEntity_InitState* Instance();

	// Enter function: runs a set of commands when a state is entered
	virtual void Enter(TemplateEntity* Entity) {};
	// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
	virtual bool Execute(TemplateEntity* Entity);
	// Exit function: runs a set of commands when a state is exited
	virtual void Exit(TemplateEntity* Entity) {};
	// OnMessage function: runs a set of commands when a message is received
	virtual bool OnMessage(TemplateEntity* Entity, const Message& msg);

	// The only thing the initialization state does is change the state to something else
	// The initialization state is useful because the Enter function will not be ran on the first state assigned
	// The Enter function isn't commonly used, but it can be helpful

	// An example of when the Enter and Exit functions will be useful is for when an entity switches to an attack state
	// Upon entering the attack state that entity can draw his weapons and when exiting that state he can sheathe his weapons
};

//-----------------------------------------------------------------------
// TemplateEntity roam state sub class
//-----------------------------------------------------------------------
class TemplateEntity_RoamState : public State<TemplateEntity>
{
private:
	// TemplateEntity_RoamState function: default constructor
	TemplateEntity_RoamState() {m_bDelayedMessageDispatched = false;};
	// TemplateEntity_RoamState function: copy constructor
	TemplateEntity_RoamState(TemplateEntity_RoamState const&) {};
	// operator= overload so instance can't be copied
	TemplateEntity_RoamState& operator=(TemplateEntity_RoamState const&) {};

	static TemplateEntity_RoamState* m_pInstance;			// static pointer to own class

public:
	// Instance function: ensures only a single instance of this class exists
	static TemplateEntity_RoamState* Instance();

	// Enter function: runs a set of commands when a state is entered
	virtual void Enter(TemplateEntity* Entity) {};
	// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
	virtual bool Execute(TemplateEntity* Entity);
	// Exit function: runs a set of commands when a state is exited
	virtual void Exit(TemplateEntity* Entity) {};
	// OnMessage function: runs a set of commands when a message is received
	virtual bool OnMessage(TemplateEntity* Entity, const Message& msg);
};

//-----------------------------------------------------------------------
// TemplateEntity attack state sub class
//-----------------------------------------------------------------------
class TemplateEntity_AttackState : public State<TemplateEntity>
{
private:
	// TemplateEntity_AttackState function: default constructor
	TemplateEntity_AttackState() {m_bDelayedMessageDispatched = false;};
	// TemplateEntity_AttackState function: copy constructor
	TemplateEntity_AttackState(TemplateEntity_AttackState const&) {};
	// operator= overload so instance can't be copied
	TemplateEntity_AttackState& operator=(TemplateEntity_AttackState const&) {};

	static TemplateEntity_AttackState* m_pInstance;			// static pointer to own class

public:
	// Instance function: ensures only a single instance of this class exists
	static TemplateEntity_AttackState* Instance();

	// Enter function: runs a set of commands when a state is entered
	virtual void Enter(TemplateEntity* Entity) {};
	// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
	virtual bool Execute(TemplateEntity* Entity);
	// Exit function: runs a set of commands when a state is exited
	virtual void Exit(TemplateEntity* Entity) {};
	// OnMessage function: runs a set of commands when a message is received
	virtual bool OnMessage(TemplateEntity* Entity, const Message& msg);
};

//-----------------------------------------------------------------------
// TemplateEntity dead state sub class
//-----------------------------------------------------------------------
class TemplateEntity_DeadState : public State<TemplateEntity>
{
private:
	// TemplateEntity_DeadState function: default constructor
	TemplateEntity_DeadState() {m_bDelayedMessageDispatched = false;};
	// TemplateEntity_DeadState function: copy constructor
	TemplateEntity_DeadState(TemplateEntity_DeadState const&) {};
	// operator= overload so instance can't be copied
	TemplateEntity_DeadState& operator=(TemplateEntity_DeadState const&) {};

	static TemplateEntity_DeadState* m_pInstance;			// static pointer to own class

public:
	// Instance function: ensures only a single instance of this class exists
	static TemplateEntity_DeadState* Instance();

	// Enter function: runs a set of commands when a state is entered
	virtual void Enter(TemplateEntity* Entity) {};
	// Execute function: runs a set of commands every frame, returns true if entity was deleted/killed during this update call
	virtual bool Execute(TemplateEntity* Entity);
	// Exit function: runs a set of commands when a state is exited
	virtual void Exit(TemplateEntity* Entity);
	// OnMessage function: runs a set of commands when a message is received
	virtual bool OnMessage(TemplateEntity* Entity, const Message& msg);
};