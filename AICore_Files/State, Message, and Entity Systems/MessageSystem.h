/************************************************************************
*  Program Name:		MessageSystem.h									*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Header file for MessageDispatcher class			*
*						Allows for the creation and sending of messages	*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

#pragma once

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include <iostream>
#include <set>
#include "EntityGenerator.h"
#include "Timer.h"

//-----------------------------------------------------------------------
// Predeclaration class
//-----------------------------------------------------------------------
class Entity;

//-----------------------------------------------------------------------
// Message type identifiers
//-----------------------------------------------------------------------
enum MESSAGE_TYPES {MSG_ROAM, MSG_ATTACK, MSG_DEAD, MSG_DELETE};

		//////////////////////////////////////////////////////
		// NOTE: all message types need to be included here //
		//////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Message struct
//-----------------------------------------------------------------------
struct Message
{
	int m_iSender;			// stores the ID of the entity that sent this message

	int m_iReceiver;		// stores the ID of the entity that is to receive this message

	int m_iMsg;				// stores the message itself

	float m_fDispatchTime;	// stores the time to wait before the message is sent

	// Message function: default constructor
	Message();
	// Message function: parameterized constructor
	Message(int iSend, int iReceive, int iMsg, float iDispatchTime);
};

//-----------------------------------------------------------------------
// Inline utility variable and functions
//-----------------------------------------------------------------------
const float g_fThresholdDelay = 0.25; // global variable storing a threshold for the delay for when comparing (==) messages

// operator== overload to allow messages to be compared
inline bool operator==(const Message& Msg1, const Message& Msg2);

// operator< overload to allow messages to be properly ordered in the PriorityQ
inline bool operator<(const Message& Msg1, const Message& Msg2);

//-----------------------------------------------------------------------
// MessageDispatcher class
//-----------------------------------------------------------------------
class MessageDispatcher
{
private:
	std::set<Message> m_PriorityQ;	// priority queue to store messages and send them by dispatch time

	// Discharge function: sends the message over to the Entity's HandleMessage function
	void Discharge(Entity* Receiver, const Message& Msg);

	// MessageDispatcher function: default constructor
	MessageDispatcher() {};
	// MessageDispatcher function: copy constructor
	MessageDispatcher(MessageDispatcher const&) {};
	// operator= overload so instance can't be copied
	MessageDispatcher& operator=(MessageDispatcher const&) {};

	static MessageDispatcher* m_pInstance;		// static pointer to own class

public:
	// Instance function: ensures only a single instance of this class exists
	static MessageDispatcher* Instance();

	// DispatchMessage function: sends the message to Discharge if it has no delay, otherwise puts it in the PriorityQ
	void DispatchMessage(float fDelay, int iSender, int iReceiver, int iMsg);
	// DispatchDelayedMessages function: loops through PriorityQ until empty or there are no messages to be dispatched at current time
	void DispatchDelayedMessages();
};