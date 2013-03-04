/***********************************************************
*  Program Name: WeightedMovement                          *         
*  Name: John McGarvey                                     *
*  Date: January 20, 2013                                  *
*  Description: Handles Weighted Movement for AI entities  *
*  Update: John McGarvey 1/30/13                           *
***********************************************************/

//--------------------
//Include Files
//--------------------
#include <iostream>
#include <math.h>
#include <time.h>
using namespace std;

//--------------------
//Defined Constant
//--------------------
#define PI 3.14159265

//Main Weighted Movement
class WM_base {
	//--------------
	//Variable Declarations
	//--------------
	bool m_bSeeking;		//Set to true if entity is actively aware of target (false causes wandering instead)
	bool m_bFlee;			//Determines if entity should move away from target instead
	bool m_bPursue;			//Whether entity should account for target's movement while Seeking - Does nothing unless Vertical_Free is also true, and should be false if Vertical_Free is false
	bool m_bSlow;			//Determines if entity should slow down when close to target
	bool m_bVertical_Free;	//Determines whether entity is able to move vertically or limited to ground
	int m_iPositionX;
	int m_iPositionY; 
	double m_dSlowDist;		//Used in conjuction with Slow, determines distance at which entity should begin decelerating
	double m_dVelocity; 
	double m_dDirection;	//Works with Velocity to determine entity's movement *NOTE: Used as an angle if Vertical_Free is true, but should only be set to 0 or 180 if not
	double m_dMax_Velocity; 
	double m_dAcceleration;	//The speed with which the entity can accelerate or decelerate
	double m_dManeuver;		//Used in conjunction with Vertical_Free to allow entity to adjust direction
	
	//------------
	//Private Functions
	//------------
	void update();
	void seek();
	void wander();
	void slow();
	void pursue();
	double calcDist();
	void normalize();

public:
	WM_base();
	};

//Main function call, for calculations and updating position
void WM_base::update()
{
	if (m_bSeeking == true)
	{
		seek();
	}
	else 
	{
		wander();
	}
	
	normalize();
	m_iPositionX = m_iPositionX + (sin(m_dDirection) * m_dVelocity);
	m_iPositionY = m_iPositionY + (cos(m_dDirection) * m_dVelocity);
}

//Returns horizontal distance if AI does not move vertically, or direct distance if it does
double WM_base::calcDist()
{
	double d;
	
	if (m_bVertical_Free == true)
	{
		d = pow((double)(abs(m_iPositionX-player.m_iPositionX)),2.0) + pow((double)(abs(m_iPositionY-player.m_iPositionY)),2.0);
		d = sqrt(d);
	}
	else 
	{
		d = abs(m_iPositionX-player.m_iPositionX);
	}
	
	return d;
}

//Slows the AI's velocity once it is close enough to target
void WM_base::slow()
{
	m_dVelocity = max(m_dVelocity - m_dAcceleration, 0.0);
}

//chooses the appropriate seeking action
void WM_base::seek()
{
	//alters target direction if needed, and player is moving fast enough
	if (m_bPursue == true && player.m_dVelocity > 2.0)
	{
		pursue();
	} 
	
	//slows down if needed, and within correct distance
	else if (m_bSlow == true && calcDist() <= m_dSlowDist)
	{
		slow();
	} 

	//Main Seek Function
	else 
	{
		if (m_bVertical_Free == true)
		{
			double tdirect = atan2(player.m_iPositionY - m_iPositionY, player.m_iPositionX - m_iPositionX) * 180 / PI;
			if (m_dDirection > tdirect)
			{
				max(m_dDirection - m_dManeuver, tdirect);
			}
			else (m_dDirection < tdirect)
			{
				min(m_dDirection + m_dManeuver, tdirect);
			}
		}

		else 
		{
			if (player.m_iPositionX < m_iPositionX)
			{
				m_dDirection = 180;
			}
			if (player.m_iPositionX > m_iPositionX)
			{
				m_dDirection = 0;
			}
		}

		m_dVelocity = m_dVelocity + m_dAcceleration;
	}
}

//Factors in player movement to determine best trajectory
void WM_base::pursue()
{
	if (m_bVertical_Free == true)
	{
		double ata = calcDist()/m_dVelocity;
		double tdirect = atan2(player.m_iPositionY - m_iPositionY, (player.m_iPositionX - m_iPositionX)+(player.m_dVelocity*ata)) * 180 / PI;
			
		if (m_dDirection > tdirect)
		{
			max(m_dDirection - m_dManeuver, tdirect);
		}
		else (m_dDirection < tdirect)
		{
			min(m_dDirection + m_dManeuver, tdirect);
		}
	}
	m_dVelocity = m_dVelocity + m_dAcceleration;
}

//Randomized movement when not seeking target
void WM_base::wander()
{
	if (m_bVertical_Free == true)
	{
		m_dDirection = m_dDirection + ((rand() % m_dManeuver) - (m_dManeuver/2));
	}
	m_dVelocity = m_dVelocity + ((rand() % m_dAcceleration) - (m_dAcceleration/2));
}

//Keeps current Velocity below Maximum Velocity, and sets angles to 0-360 range if needed
void WM_base::normalize()
{
	if (m_dVelocity > m_dMax_Velocity)
	{
		m_dVelocity = m_dMax_Velocity;
	}
	if (m_dDirection > 360)
	{
		m_dDirection -= 360;
	}
	else if (m_dDirection < 0)
	{
		m_dDirection += 360;
	}
}

//Default creation template
WM_base::WM_base()
{
	m_bSeeking = false;
	m_bFlee = false;
	m_bPursue = false;
	m_bSlow = false;
	m_bVertical_Free = false;
	m_iPositionX = 0;
	m_iPositionY = 0; 
	m_dSlowDist = 0.0;
	m_dVelocity = 0.0; 
	m_dDirection = 0.0;
	m_dMax_Velocity = 1.0; 
	m_dAcceleration = 1.0;
	m_dManeuver = 1.0;
}

void main()
{

}