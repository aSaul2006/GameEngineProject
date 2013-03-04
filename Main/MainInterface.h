#pragma once

#include "WorldObject.h"
#include "StateManager.h"
#include <string>

//Physics variables
const float gravity = -9.81f; //In m/s. See about getting this from physics core
const float friction = -0.1f; //Apparently these have public scope.
const float drag = -0.05f; //Yeah, I'm just making these up for now.

class MainInterface
{
	StateManager stateManager; //Our state manager

public:

	//-----Variables and objects-----
	WorldObject worldObjects[100]; //Array of the different entities in the game world.	

	MainInterface(HINSTANCE);

	void Run(void);

	static void OutputToConsole(std::string s);

	~MainInterface(void);
};

