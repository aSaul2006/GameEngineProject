/*****************************************************************
Program Name: Trigger_Respawning.h
Name: Zach Fetters
Description: This file is used to define all variables and functions
			 for a basic respawning trigger and should be inherited
			 by another class to create a specific type of trigger
*****************************************************************/

#include "Trigger.h"


template <class entityType>
class Trigger_Respawning : public Trigger<entityType>
{

protected:

	//how many game update steps should pass from deactivation to respawning
	int m_iNumUpdatesBeforeRespawn;

	//how many update steps are left until this trigger respawns
	int m_iNumUpdatesLeftForRespawn;

	//deactivates the trigger for a set number of update steps
	void Deactivate()
	{

		SetInactive();
		m_iNumUpdatesLeftForRespawn = m_iNumUpdatesBeforeRespawn;

	}

public:

	Trigger_Respawning(int id):Trigger<entityType>(id),
								m_iNumUpdatesBeforeRespawn(0),
								m_iNumUpdatesLeftForRespawn(0)
	{

	}

	virtual ~Trigger_Respawning()
	{

	}

	//should be inmplemented by child classes
	virtual void Try(entityType*) = 0;

	//should be called during each game update step
	virtual void Update()
	{

		if((--m_iNumUpdatesLeftForRespawn <= 0) && !IsActive())
		{

			SetActive();

		}

	}

	//set how many steps should pass before this trigger respawns
	void SetRespawnDelay(unsigned int numUpdtaes)
	{

		m_iNumUpdatesBeforeRespawn = numUpdates;

	}


};