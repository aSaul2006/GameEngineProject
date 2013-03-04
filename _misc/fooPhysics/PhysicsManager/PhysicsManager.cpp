/*****************************************************************************************
*                                                                                        *
*  Program Name:          PhysicsManager.cpp                                             *
*  Name:                  Bryan Sweeney                                                  *
*  Date:                  February 16, 2013                                              *
*  Description:           Physics Manager source file - allows for access to physics     *
*                         engine by games main() creating and deleting worlds to run     *
*                         Simulations                                                    *
*                                                                                        *
*  Update:                Jennifer Wickensimer, 02/16/2013                               *
*                                                                                        *
*****************************************************************************************/
#include "PhysicsManager.h"
#include <vector>


PhysicsManager::PhysicsManager(void)
{
	
}


PhysicsManager::~PhysicsManager(void)
{
}

void PhysicsManager::InitManager()
{
	// returns the pointer to PhysicsWorld created in createWorld()
	void CreateWorld(PhysicsEngine::World *pWorld);
}

void PhysicsManager::CreateWorld(PhysicsEngine::World *)
{
	PhysicsEngine::World *pWorld;
}

void PhysicsManager::AddObject(PhysicsEngine::PhysBody *pBody, PhysicsEngine::Vector3D Position, 
	PhysicsEngine::Quaternion *Orientation, PhysicsEngine::Vector3D Rotation, double Mass, 
	double Height, double Width, double Depth, PhysicsEngine::Vector3D Velocity,
	PhysicsEngine::Vector3D Acceleration)
{
	// create new object
	pBody = new PhysicsEngine::PhysBody;
	// Set attributes of object
	SetObjectState(pBody, Position, *Orientation, Rotation, Mass);
	// check to see if box or sphere
	if (double Radius=0)
	{
		SetShape(pBody, Height, Width, Depth); // Is Box so setshape to box
	}
	else
	{
		SetShape(pBody, Radius); // is sphere so setshape to sphere
	}
	
	SetMove(pBody, Velocity, Acceleration); //Add movement to physBody object
	
}

void PhysicsManager::SetObjectState(PhysicsEngine::PhysBody *pBody,
	                                PhysicsEngine::Vector3D Position, 
	                                PhysicsEngine::Quaternion Orientation, 
									PhysicsEngine::Vector3D Rotation, double Mass)
{
	pBody->setBodyPosition(Position);
	pBody->setBodyOrientationQ(Orientation);
	pBody->setObjectRotation(Rotation);
	pBody->setBodyMass(Mass);
}

void PhysicsManager::SetShape(PhysicsEngine::PhysBody *pBody,
	                          double Height, double Width, double Depth)
{
	
	PhysicsEngine::Vector3D vec;
	vec.x=Width*0.5; vec.y=Height*0.5; vec.z=Depth*0.5;
	PhysicsEngine::CollisionBox box;
	box.m_v3HalfSizeBox = vec;
}

void PhysicsManager::SetShape(PhysicsEngine::PhysBody *pBody, double Radius)
{
	PhysicsEngine::CollisionSphere sphere;
	sphere.m_dSphereRadius = Radius;
}

void PhysicsManager::SetMove(PhysicsEngine::PhysBody *pBody,
	                         PhysicsEngine::Vector3D Velocity, 
	                         PhysicsEngine::Vector3D Acceleration)

{
	pBody->setObjectVelocity(Velocity);
	pBody->setBodyAcceleration(Acceleration);
}

void StartPhysics()
{
    PhysicsManager *pPhysMan; // creates a pointer to PhysicsManager	pPhysMan = new PhysicsManager*; // creates a pointer to PhysicsManager
	void InitManager();
}

void PhysicsManager::UpdatePhysics()
{
	//cycle through vector containing pointers to pBodies and update
	BodyRegistration *reg = firstBody;
	while (reg)
	{
		//remove all accumlated forces
		reg->pBody->emptyAccumulators();
		reg->pBody->calculateObjectData();

		//go to the next reg
		reg = reg->next;

	}
}
	
void PhysicsManager::DeInitPhysics()
{
	//cycle through vector containing pointers to pBodies and delete
	/*for (unsigned int i=0; i < Objects.size(); i++)
	{
		delete Objects[i];
	};

	~World();    
	PhysicsManager::~PhysicsManager();*/
	BodyRegistration *reg = firstBody;
	while (reg)
	{
		//remove all accumlated forces
		delete reg->pBody;
	
		//go to the next reg
		reg = reg->next;

	}
	
	//delete PhysicsEngine::World pWorld;
}




