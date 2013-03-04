#include "StdAfx.h"
#include "MainInterface.h"
#include "StateManager.h"
#include <iostream> //For reporting errors to console
#include <sstream> //Lets us output variable strings through the console

using namespace std;

MainInterface::MainInterface(HINSTANCE h)
{
	stateManager = StateManager(h);
}

//Run the StateManager and game loop
void MainInterface::Run(void)
{
	try //If any error we haven't handled further in occurs, we want to properly log what the error was to a text file or such.
	{
		stateManager.Run();
	}
	catch (...)
	{
		//TODO: Properly log whatever occurred.
		cout << "An error occurred, but we haven't yet figured out how to properly log it. Press enter to throw the exception to the debugger.";
		throw;
	}
}

//Outputs the given string to the console window.
void MainInterface::OutputToConsole(string s)
{
	cout << s;
}

MainInterface::~MainInterface(void)
{
}
