/************************************************************************
*  Program Name:		Timer.cpp										*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Code file for Timer class						*
*						Keeps track of current time for messages		*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#include "Timer.h"

//-----------------------------------------------------------------------
// Initialize Timer Instance of self
//-----------------------------------------------------------------------
Timer* Timer::m_pInstance = 0;

//-----------------------------------------------------------------------
// Timer class
//-----------------------------------------------------------------------
// Timer function: default constructor
Timer::Timer()
{
	m_fStartTime = timeGetTime() * 0.001;
}

// Instance function: ensures only a single instance of this class exists
Timer* Timer::Instance()
{
	if(!m_pInstance)
		m_pInstance = new Timer;

	return m_pInstance;
}

// GetCurrentTime function: returns the amount of time that has elapsed since the timer started
float Timer::GetCurrentTime()
{
	return timeGetTime() * 0.001 - m_fStartTime;
}