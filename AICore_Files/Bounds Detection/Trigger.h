/*****************************************************************
Program Name: Trigger.h
Name: Zach Fetters
Description: This file declares the variables and functions for a
			 basic trigger and should be inherited by another class
			 to create a specific trigger type
*****************************************************************/

#include "Entity.h" //include basic entity class
#include "TriggerRegion.h"


template <class entityType>
class Trigger : public Entity
{

private:

	//trigger region of the trigger
	TriggerRegion* m_pTriggerRegion;

	//when true the trigger will be deleted
	bool m_bNeedsRemoved;

	//when true the trigger is currently active in the game
	bool m_bIsActive;

	//graph node index to allow for searching of triggers
	int m_iGraphIndex;

protected:

	//set the graph node index
	void SetGraphIndex(int index)
	{

		m_iGraphIndex = index;

	}

	//mark the trigger for removal
	void SetForRemoval()
	{

		m_bNeedsRemoved = true;

	}

	//mark the trigger as inactive
	void SetInactive()
	{

		m_bIsActive = false;

	}

	//mark the trigger as active
	void SetActive()
	{

		m_bIsActive = true;

	}

	//return true if the given entity is overlapping the trigger region
	bool IsTouchingTrigger(Vector2D EntityPosition, double EntityRadius)const;

	//function to create a circular trigger region
	void AddCircularRegion(Vector2D Center, double Radius);

	//function to create a rectangular trigger region
	void AddRectangularRegion(Vector2D TopLeft, Vector2D BottomRight);

public:

	Trigger(unsigned int id):Entity(id),
							 m_bNeedsRemoved(false),
							 m_bIsActive(true),
							 m_iGraphIndex(-1),
							 m_pTriggerRegion(NULL)
	{


	}

	virtual ~Trigger()
	{

		delete m_pTriggerRegion;

	}

	//implemented within sepcific trigger type classes to check for collision with entity
	virtual void Try(entityType*) = 0;

	//implemented within specific trigger type classes to update trigger during each update step
	virtual void Update() = 0;

	//return the graph node index for the trigger
	int GraphIndex()const
	{

		return m_iGraphIndex;

	}

	//check if the trigger needs to be deleted
	bool NeedsRemoved()const
	{

		return m_bNeedsRemoved;

	}

	//check if the tigiger is active
	bool IsActive()
	{

		return m_bIsActive;

	}


};


template <class entityType>
void Trigger<entityType>::AddCircularRegion(Vector2D Center, double Radius)
{

	//if a region already exists delete it before creating a new region
	if(m_pTriggerRegion)
	{

		delete m_pTriggerRegion;

	}

	//create new trigger region
	m_pTriggerRegion = new TriggerRegion_Circle(Center, Radius);

}


template <class entityType>
void Trigger<entityType>::AddRectangularRegion(Vector2D TopLeft, Vector2D BottomRight)
{

	//if a region already exists delete it before creating a new trigger region
	if(m_pTriggerRegion)
	{

		delete m_pTriggerRegion;

	}

	//create new trigger region
	m_pTriggerRegion = new TriggerRegion_Rectangle(TopLEft, BottomRight);

}


template <class entityType>
bool Trigger<entityType>::IsTouchingTrigger(Vector2D EntityPosition, double EntityRadius)const
{

	//if a trigger region exists check if it is touching the given entity
	if(m_pTriggerRegion)
	{

		return m_pTriggerRegion->IsTouching(EntityPosition, EntityRadius);

	}

	return false;

}