#pragma once // Prevents the header file from being included more than once

#include "windows.h" // The Windows libraries

class StateManager
{
	enum gameState {STARTUP, TITLE, MAINMENU, INGAME, PAUSE, SHUTDOWN}; // Specifies which game state we're in TODO: Add more states as needed
	gameState currentState; // The current state we're in
	HINSTANCE hInstance; // Current HINSTANCE, whatever that means. Used for rendering core

	void StartUp(DWORD elapsedTime);
	void Title(DWORD elapsedTime);
	void MainMenu(DWORD elapsedTime);
	void InGame (DWORD elapsedTime);
	void Pause(DWORD elapsedTime);
	void ShutDown(DWORD elapsedTime);

	void InitRenderingCore(HINSTANCE);

public:


	StateManager(void); //Default Constructor
	StateManager(HINSTANCE); // Constructor
	void Run();
	~StateManager(void); // Destructor
};

