/*****************************************************************
Program Name: Trigger_LimitedLifetime.h
Name: Zach Fetters
Description: This file is used to define the variables and functions
			 for a basic trigger with a set lifetime and should be
			 inherited by another class to create a specific trigger
*****************************************************************/

#include "Trigger.h"

template <class entityType>
class Trigger_LimitedLifetime : public Trigger<entityType>
{

protected:

	//the number of game update steps this trigger will last
	int m_iLifetime;

public:

	Trigger_LimitedLifetime(int Lifetime):Trigger<entityType>(Entity::/*get next available entity*/),
										  m_iLifetime(Lifetime)
	{


	}

	virtual ~Trigger_LimitedLifetime()
	{

	}

	//should be called from within child classes update methods
	virtual void Update()
	{

		//if the triggers lifetime expires mark it for removal
		if(--m_iLifetime <= 0)
		{

			SetForRemoval();

		}

	}

	//should be implemented by child classes
	virtual void Try(entityType*) = 0;

};