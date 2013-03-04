/************************************************************************
*  Program Name:         Keyboard_Input.h                                    *         
*  Name:                 Paul Scott                                                                                        
*  Date:                 January 25th, 2013  								*
*  Description:          A class that initializes a Direct Input object and device
*  					 as well as handling keyboard input
*  													                                                                                      *
*  																		*
*  														               *
************************************************************************/
//---------------------------------------------
//Include Files
//---------------------------------------------
#include "stdafx.h"
#define DIRECTINPUT_VERSION 0x0800
#define KEYDOWN(name, key) (name[key] & 0x80)
#include <dinput.h>


//---------------------------------------------
//The class: Input
//---------------------------------------------
class Input
{
private: 
	LPDIRECTINPUT8        g_lpDirect_Input_Object;      //the direct input object
	LPDIRECTINPUTDEVICE8  g_lpDirect_Input_Device;      //the direct input device
	char                  m_cBuffer[256];               //array to hold the state of each key					

public:
	Input();											//constructor
	~Input();											//destructor
	int Direct_Input_Initialize(HINSTANCE);	
	void Release();
	void KeyPress();									//checks m_cBuffer to see what keys were pressed
};
