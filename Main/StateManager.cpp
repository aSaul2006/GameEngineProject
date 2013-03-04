/*
	This file defines the StateManager class. When running, we do a game loop, passing control to specific methods to handle each loop's processing, while we can
switch states at any time. The time elapsed since the last processing cycle is passed in, so that the new cycle will know how much real time has passed to account for.
We will be replacing most of the console filler with the use of the cores. At first I thought a whole CoreManager class would be necessary, but on second thought, all we need
are includes for each other core's interface class. A CoreManager class is redundant if the cores are already offering up public methods through an interface to use what we 
need from them.
*/

#include "StdAfx.h" // Standard library... I think
#include "StateManager.h" // Our StateManager class
#include <iostream> // Now we can work with I/O
#include "MainInterface.h"
#include "..\Render\EngineMain.h"

using namespace std; // Standard namespace in standard library

EngineMain* app;

StateManager::StateManager(void)
{
}

StateManager::StateManager(HINSTANCE h) // Constructor initializing to STARTUP state
{
	hInstance = h;
	currentState = STARTUP; // Default our current state to STARTUP

	cout << "StateManager Created\n"; // Write success to console	
}

void StateManager::Run() // Start the game loop
{
	bool running = true; // The game is running

	DWORD startTime = GetTickCount(); // Returns the amount of milliseconds since system startup, effectively the current time
	DWORD elapsedTime = startTime; // No time has passed since the last cycle, on the first cycle

	while (running) //Loop each processing cycle until time to shut down StateManager
	{
		switch (currentState)
		{
			case STARTUP:
				StartUp(elapsedTime);
				break;

			case TITLE:
				Title(elapsedTime);
				break;

			case MAINMENU:
				MainMenu(elapsedTime);
				break;

			case INGAME:
				InGame(elapsedTime);
				break;

			case PAUSE:
				Pause(elapsedTime);

			case SHUTDOWN:
				ShutDown(elapsedTime);
				running = false;
				break;

			default:
				cout << "Error: In unrecognized game state. This is likely caused by a coder adding a new state to the currentState enum without putting a corresponding case in the main game loop.";
				cin.ignore(1);
				running = false;
				break;
		}

		elapsedTime = GetTickCount() - startTime; // Elapsed time in last cycle is the current time, minus the time when we started
	}
}

void StateManager::StartUp(DWORD elapsedTime)
{
	//cout << "Elapsed time since last cycle: " + (int) elapsedTime; //TODO: Fix this. I can't currently figure out how to print it out, simple boxing attempts just lead to memory access violations. It's fine in the debugger though, showing like 16 ms to iterate once through start up.
	cout << "\nStarting up...\n";

	InitRenderingCore(hInstance);

	cout << "Startup complete.\n";

	currentState = TITLE;
}

//Initialize the rendering core. TODO: Currently just a copy of rendering core's main test method
void StateManager::InitRenderingCore(HINSTANCE hInstance)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	app = new EngineMain(hInstance, L"GSP420 CAGE", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	g_d3dApp = app;

    g_d3dApp->initRender();
}

void StateManager::Title(DWORD elapsedTime)
{
	cout << "Title screen displaying.\n";

	//STUB: We need to actually draw the title screen and use the proper I/O

	cout << "Press enter to simulate pressing the button to move on to main menu.";

	cin.ignore(1);

	currentState = MAINMENU;
}

void StateManager::MainMenu(DWORD elapsedTime)
{
	cout << "Main menu displaying.\n";

	//STUB: We need to actually draw the title screen and use the proper I/O

	cout << "Press enter to simulate choosing new game.";

	cin.ignore(1);

	currentState = INGAME;
}

void StateManager::InGame(DWORD elapsedTime)
{
	cout << "Game is running.\n";

	g_d3dApp->render();

	if (app->msg.message == WM_QUIT)
		currentState = SHUTDOWN;

	//cout << "Press enter to simulate opening pause menu.";

	//cin.ignore(1);

	//currentState = PAUSE;
}

void StateManager::Pause(DWORD elapsedTime)
{
	cout << "Game is paused.\n";

	//STUB: We need to draw the pause menu, use the proper I/O.

	cout << "Press any key to simulate choosing to quit to desktop.";

	cin.ignore(1);

	currentState = SHUTDOWN;
}

void StateManager::ShutDown(DWORD elapsedTime)
{
	cout << "Shutting down...\n";

	//STUB: Prepare to shut down the program.

	cout << "Shut down procedures complete. Press any key to close.\n";
	cin.ignore(1); // Wait for key
}

StateManager::~StateManager(void) // Destructs the StateManager class
{
	//TODO: Insert any needed cleanup code here

	cout << "StateManager Destroyed"; //Report success
}
