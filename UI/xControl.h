#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// No MFC
#define WIN32_LEAN_AND_MEAN

// We need the Windows Header and the XInput Header
#include <windows.h>
#include <XInput.h>

// Now, the XInput Library
// NOTE: COMMENT THIS OUT IF YOU ARE NOT USING
// A COMPILER THAT SUPPORTS THIS METHOD OF LINKING LIBRARIES
#pragma comment(lib, "XInput.lib")
class xControl
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
public:
	xControl(int playerNumber)
	{
		// Set the Controller Number
		_controllerNum = playerNumber - 1;
	}

	//Current state of controller
	XINPUT_STATE GetState()
	{
		// Zeroise the state
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		// Get the state
		XInputGetState(_controllerNum, &_controllerState);
		return _controllerState;
	}

	//Check if Controller Exists
	bool IsConnected()
	{
		// Zeroise the state
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		// Get the state
		DWORD Result = XInputGetState(_controllerNum, &_controllerState);

		if(Result == ERROR_SUCCESS)
			return true;
		else
			return false;
	}

	void Vibrate(int leftVal = 0, int rightVal = 0)
	{
		// Create a Vibraton State
		XINPUT_VIBRATION Vibration;

		// Zeroise the Vibration
		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

		// Set the Vibration Values
		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;

		// Vibrate the controller
		XInputSetState(_controllerNum, &Vibration);
	}

	xControl(void);
	~xControl(void);
};
#endif

