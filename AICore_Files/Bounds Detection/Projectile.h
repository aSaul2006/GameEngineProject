/*****************************************************************
Program Name: Projectile.h
Name: Zach Fetters
Description: This file declares the variables and functions
			 to be used when creating a projectile for use
*****************************************************************/

#include "MovingEntity.h" //include the moving entity class (No copy of file to add *Bryan Sweeney* 2/28/13)
#include <list>
#include "Vector2D.h"

class Entity;
class Game; //the main class for the game

class Projectile : public MovingEntity
{

protected:

	int m_iShooterID; //ID of the entity who fired this projectile
	Vector2D m_vTarget; //the point at which the projectile is aimed
	Game* m_pWorld; //pointer to the games data
	Vector2D m_vOrigin; //the point at which the projectile was fired from
	int m_iDamageDealt; //how much damage the projectile will do
	bool m_bIsDead; //if the projectile is "dead" it should deleted
	bool m_bImpacted; //set to true as soon as the projectile impacts something
	Vector2D m_vImpactPoint; //the point at which the projectile impacts and object

	Entity* GetClosestIntersectingEntity(Vector2D From, Vector2D To)const;

	std::list<Entity*> GetListOfIntersectingEntities(Vector2D From, Vector2D To)const;

public:

	Projectile(Vector2D target, 
			   Game* world, 
			   int ShooterID, 
			   Vector2D origin,
			   Vector2D heading, 
			   int damage, 
			   double scale,
			   double MaxSpeed, 
			   double mass, 
			   double MaxForce) : MovingEntity(origin,
			   scale, 
			   Vector2D(0,0), 
			   MaxSpeed, 
			   heading, 
			   mass, 
			   Vector2D(scale,scale), 
			   0, 
			   MaxForce),
			   m_vTarget(target),
			   m_bIsDead(false), 
			   m_bImpacted(false), 
			   m_pWorld(world), 
			   m_iDamageDealt(damage), 
			   m_vOrigin(origin),
			   m_iShooterID(ShooterID)
	{
		
	}


	virtual void Render() = 0;
	virtual void Update() = 0;

	//get current value of m_bIsDead variable
	bool IsDead()const
	{
		return m_bIsDead;
	}

	//get current value of m_bImpacted variable
	bool HasImpacted()const
	{
		return m_bImpacted;
	}


};