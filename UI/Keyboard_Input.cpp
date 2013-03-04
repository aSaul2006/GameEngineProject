/************************************************************************
*  Program Name:         Keyboard_Input.h                                    *         
*  Name:                 Paul Scott                                                                                        
*  Date:                 January 25th, 2013  								*
*  Description:          A class that initializes a Direct Input object and device
*  					 as well as handling keyboard input
*  					 
*  													                                                                                      *
*  																		*
*  														               *
************************************************************************/
#include "stdafx.h"
#include "Keyboard_Input.h"


//***************************
//Constructor
//**************************
//Simple constructor
Input::Input()
{
	g_lpDirect_Input_Device = NULL;

}

//************************
//Destructor
//************************
//Destructor. Also releases the Direct Input Object (but not the device)
Input::~Input()
{
	//releasing the Direct input object
	g_lpDirect_Input_Object->Release();
	g_lpDirect_Input_Object = NULL;
}

//*****************************
//Initializing Direct Input
//****************************
//This section of code creates the Direct Input Object and Device
int Input::Direct_Input_Initialize(HINSTANCE hInstance)
{  
	HRESULT hr_Test = NULL;
	HWND hWindow = NULL;

	//Create the Direct Input object
	hr_Test = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDirect_Input_Object, NULL);
	if FAILED(hr_Test)		//Test for failure
		return FALSE;		

	//Create the Direct Input Device
	hr_Test = g_lpDirect_Input_Object->CreateDevice(GUID_SysKeyboard, &g_lpDirect_Input_Device, NULL);
	if FAILED(hr_Test)
		return FALSE;

	//Set the data format
	hr_Test = g_lpDirect_Input_Device->SetDataFormat(&c_dfDIKeyboard);
	if FAILED(hr_Test)
		return FALSE;

	//Set the cooperative level
	hr_Test = g_lpDirect_Input_Device->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if FAILED(hr_Test)
		return FALSE;

	//Gain access to the device
	hr_Test = g_lpDirect_Input_Device->Acquire();
	if FAILED(hr_Test)
		return FALSE;

	return true;
}
//---------------------------
//Input::Release()
//--------------------------
//Releases the Direct Input device (but not the object)
void Input::Release()
{
	//unacquiring device, then release
	g_lpDirect_Input_Device->Unacquire();
	g_lpDirect_Input_Device->Release();
	g_lpDirect_Input_Device = NULL;
}

//--------------------------
//Input::KeyPress()
//--------------------------
//First, checks if we lost the device, if so then it reacquires it. Next it
//checks the keyboard state, and does something based on which keys were pressed
//Must be in the main game loop
void Input::KeyPress()
{


	HRESULT hr=g_lpDirect_Input_Device->GetDeviceState( sizeof(m_cBuffer), m_cBuffer );

	if (FAILED(hr))
	{
		// If input is lost then acquire and keep trying until we get it back
		hr=g_lpDirect_Input_Device->Acquire();
		while( hr == DIERR_INPUTLOST )
		{         
			hr = g_lpDirect_Input_Device->Acquire();
		}
		// Could be we failed for some other reason
		if (FAILED(hr))
			return;
			
	

	}



	//check what keys are currently being pressed
	//Checks keypress every cycle. Needs to send appropriate call depending on game state. 
	//I.e. if menu is up, needs to control menu. If Menu is down, needs to control character. 
	g_lpDirect_Input_Device->GetDeviceState( sizeof(m_cBuffer), (LPVOID) &m_cBuffer );


	if (KEYDOWN(m_cBuffer, DIK_LEFT))
	{
		//do something with the left arrow
	}

	if (KEYDOWN(m_cBuffer, DIK_UP))
	{
		//do something with the  arrow
	}

	if (KEYDOWN(m_cBuffer, DIK_RIGHT))
	{
		//do something with the left arrow
	}

	if (KEYDOWN(m_cBuffer, DIK_LEFT))
	{
		//do something with the down arrow
	}


	//-----------------------------
	/*All additional active keys require the following code
	if (KEYDOWN(m_cBuffer, [THE KEY BEING PRESSED]))
	{
	//do something with the [THE KEY BEING PRESSED]
	}

	*/


}
