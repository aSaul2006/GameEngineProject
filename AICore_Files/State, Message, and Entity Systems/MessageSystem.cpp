/************************************************************************
*  Program Name:		MessageSystem.cpp								*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Code file for MessageSystem class				*
*						Allows for the creation and sending of messages	*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "MessageSystem.h"

//-----------------------------------------------------------------------
// Initialize MessageDispatcher Instance of self
//-----------------------------------------------------------------------
MessageDispatcher* MessageDispatcher::m_pInstance = 0;

//-----------------------------------------------------------------------
// Message struct
//-----------------------------------------------------------------------
// Message function: default constructor
Message::Message()
{
	m_iSender = 0;
	m_iReceiver = 0;
	m_iMsg = 0;
	m_fDispatchTime = 0.0f;
}

// Message function: parameterized constructor
Message::Message(int iSend, int iReceive, int iMsg, float fDispatchTime)
{
	m_iSender = iSend;
	m_iReceiver = iReceive;
	m_iMsg = iMsg;
	m_fDispatchTime = fDispatchTime;
}

//-----------------------------------------------------------------------
// Inline utility functions
//-----------------------------------------------------------------------
inline bool operator==(const Message& Msg1, const Message& Msg2)
{
	return (fabs(Msg1.m_fDispatchTime - Msg2.m_fDispatchTime) < g_fThresholdDelay) &&
			(Msg1.m_iSender == Msg2.m_iSender) &&
			(Msg1.m_iReceiver == Msg2.m_iReceiver) &&
			(Msg1.m_iMsg == Msg2.m_iMsg);
}

inline bool operator<(const Message& Msg1, const Message& Msg2)
{
	if(Msg1 == Msg2)
		return false;
	else
		return (Msg1.m_fDispatchTime < Msg2.m_fDispatchTime);
}

//-----------------------------------------------------------------------
// MessageDispatcher class
//-----------------------------------------------------------------------
// Discharge function: sends the message over to the Entity's HandleMessage function
void MessageDispatcher::Discharge(Entity* Receiver, const Message& Msg)
{
	if(!Receiver->HandleMessage(Msg))
		cout << "Message not handled";
}

// Instance function: ensures only a single instance of this class exists
MessageDispatcher* MessageDispatcher::Instance()
{
	if(!m_pInstance)
		m_pInstance = new MessageDispatcher;

	return m_pInstance;
}

// DispatchMessage function: sends the message to Discharge if it has no delay, otherwise puts it in the PriorityQ
void MessageDispatcher::DispatchMessage(float fDelay, int iSender, int iReceiver, int iMsg)
{
	// Get a pointer to the Entity that is receiving the message
	Entity* pReceiver = EntityManager::Instance()->GetEntityFromID(iReceiver);

	// create the message
	Message Msg(iSender, iReceiver, iMsg, fDelay);

	// if no delay, route the message immediately
	if(fDelay <= 0.0f)
		Discharge(pReceiver, Msg);
	else // otherwise add the current time the delay and insert message into PriorityQ
	{
		float fCurrentTime = Timer::Instance()->GetCurrentTime();

		Msg.m_fDispatchTime = fCurrentTime + fDelay;

		m_PriorityQ.insert(Msg);
	}
}

// DispatchDelayedMessages function: loops through PriorityQ until empty or there are no messages to be dispatched at current time
void MessageDispatcher::DispatchDelayedMessages()
{
	// get the current time
	float fCurrentTime = Timer::Instance()->GetCurrentTime();

	while((!m_PriorityQ.empty()) &&
		(m_PriorityQ.begin()->m_fDispatchTime < fCurrentTime) &&
		(m_PriorityQ.begin()->m_fDispatchTime > 0.0f))
	{
		// copy the first message in the priorityQ
		Message Msg = *m_PriorityQ.begin();

		// get the receiving Entity
		Entity* pReceiver = EntityManager::Instance()->GetEntityFromID(Msg.m_iReceiver);

		// send the message to that Entity
		Discharge(pReceiver, Msg);

		// erase the message from the PriorityQ
		m_PriorityQ.erase(m_PriorityQ.begin());
	}
}