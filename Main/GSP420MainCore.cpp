// GSP420MainCore.cpp : Defines the entry point for the console application
//

#include "stdafx.h"
#include "MainInterface.h"
#include <iostream> // Only being used because of reporting errors to console. Delete this line when fixed up better.
//#include "..\Audio\ResourceManager.h"
#include "..\Render\EngineMain.h"

using namespace std;



//Stub method for testing purposes
void Test()
{
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	//Test(hInstance, prevInstance, cmdLine, showCmd);
	//resMan->loadFromXMLFile("Resources.xml");
	MainInterface mainCore = MainInterface(hInstance);
	mainCore.Run();

	return 0;
}

