/************************************************************************
*  Program Name:		Timer.h											*
*  Name:				Thomas Culp										*
*  Date:				January 25, 2013								*
*  Description:			Header file for Timer class						*
*						Keeps track of current time for messages		*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/

#pragma once

//-----------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------
#pragma comment(lib, "winmm.lib")
#include <windows.h>

//-----------------------------------------------------------------------
// Timer class
//-----------------------------------------------------------------------
class Timer
{
private:
	float m_fStartTime;		// stores the start time of the program

	// Timer function: default constructor
	Timer();
	// Timer function: copy constructor
	Timer(Timer const&) {};
	// operator= overload so instance can't be copied
	Timer& operator=(Timer const&) {};

	static Timer* m_pInstance;		// static pointer to own class

public:
	// Instance function: ensures only a single instance of this class exists
	static Timer* Instance();

	// GetCurrentTime function: returns the amount of time that has elapsed since the timer started
	float GetCurrentTime();
};