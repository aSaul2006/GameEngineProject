/*****************************************************************************************
*                                                                                        *
*  Program Name:          PhysicsManager.h                                               *
*  Name:                  Bryan Sweeney                                                  *
*  Date:                  February 16, 2013                                              *
*  Description:           Physics Manager source file - allows for access to physics     *
*                         engine by games main() creating and deleting worlds to run     *
*                         Simulations                                                    *
*                                                                                        *
*  Update:                Jennifer Wickensimer, 02/16/2013                               *
*                                                                                        *
*****************************************************************************************/
#pragma once

//---------------------------------------------------------------------------------------
// Include Files
//---------------------------------------------------------------------------------------

#include "../pe/PhysicsEngine.h"


extern void StartPhysics();

class PhysicsManager
{
	friend PhysicsEngine::World;
public:
	struct BodyRegistration
	{
		PhysicsEngine::PhysBody *pBody;
		BodyRegistration * next;
	};

	//the top of the list of registered bodiesa

	BodyRegistration *firstBody;
	typedef std::vector<PhysicsEngine::PhysBody*> Objects; //array to hold added objects
	PhysicsManager(void);
	~PhysicsManager(void);
	void InitManager(); // Initiates Physics Manager and returns a pointer 
	void CreateWorld(PhysicsEngine::World *);//creates world for simulations returning pointer
	void AddObject(PhysicsEngine::PhysBody* pBody, PhysicsEngine::Vector3D Position,
		PhysicsEngine::Quaternion *Orientation, PhysicsEngine::Vector3D Rotation, double Mass,
		double Height, double Width, double Depth, PhysicsEngine::Vector3D Velocity,
		PhysicsEngine::Vector3D Acceleration); //creates a new physbody
	void SetObjectState(PhysicsEngine::PhysBody *pBody, PhysicsEngine::Vector3D, 
		PhysicsEngine::Quaternion, PhysicsEngine::Vector3D, double); // Sets basics of PhysBody Object
	void SetShape(PhysicsEngine::PhysBody *pBody, double, double, double); // set Halfsizes for collision box by multipling Height, Width and Depth by 0.5
	void SetShape(PhysicsEngine::PhysBody *pBody, double);  // radius for collision sphere
	void SetMove(PhysicsEngine::PhysBody *pBody, PhysicsEngine::Vector3D, PhysicsEngine::Vector3D); // set movement of object
	void UpdatePhysics(); // calls physicsIntegration() for each object
	void DeInitPhysics(); //deletes objects added then world


	PhysicsManager *pPhysMan;    // pointer to PhysicsManager

};

