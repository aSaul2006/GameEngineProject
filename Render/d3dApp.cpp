//=============================================================================
// d3dApp.h by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "d3dApp.h"

D3DApp* g_d3dApp              = 0;
IDirect3DDevice9* g_d3dDevice = 0;
ID3DXEffect* g_FX			  = 0;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if( g_d3dApp != 0 )
		return g_d3dApp->msgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance, std::wstring winCaption, D3DDEVTYPE devType, DWORD requestedVP)
{
	m_MainWndCaption = winCaption;
	m_DevType        = devType;
	m_dRequestedVP    = requestedVP;
	
	m_hAppInst   = hInstance;
	m_hMainWnd   = 0;
	m_d3dObject  = 0;
	m_bAppPaused  = false;
	ZeroMemory(&m_d3dPP, sizeof(m_d3dPP));

	initMainWindow();
	initDirect3D();
}

D3DApp::~D3DApp()
{
	ReleaseCOM(m_d3dObject);
	ReleaseCOM(g_d3dDevice);
}

HINSTANCE D3DApp::getAppInst()
{
	return m_hAppInst;
}

HWND D3DApp::getMainWnd()
{
	return m_hMainWnd;
}

void D3DApp::initMainWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hAppInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Default to a window with a client area rectangle of 800x600.

	RECT R = {0, 0, 800, 600};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	m_hMainWnd = CreateWindow(L"D3DWndClassName", m_MainWndCaption.c_str(), 
		WS_OVERLAPPEDWINDOW, 100, 100, R.right, R.bottom, 
		0, 0, m_hAppInst, 0); 

	if( !m_hMainWnd )
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);
}

void D3DApp::initDirect3D()
{
	// Step 1: Create the IDirect3D9 object.

    m_d3dObject = Direct3DCreate9(D3D_SDK_VERSION);
	if( !m_d3dObject )
	{
		MessageBox(0, L"Direct3DCreate9 FAILED", 0, 0);
		PostQuitMessage(0);
	}


	// Step 2: Verify hardware support for specified formats in windowed and full screen modes.
	
	D3DDISPLAYMODE mode;
	m_d3dObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	HR(m_d3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, m_DevType, mode.Format, mode.Format, true));
	HR(m_d3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, m_DevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	// Step 3: Check for requested vertex processing and pure device.

	D3DCAPS9 caps;
	HR(m_d3dObject->GetDeviceCaps(D3DADAPTER_DEFAULT, m_DevType, &caps));

	DWORD devBehaviorFlags = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		devBehaviorFlags |= m_dRequestedVP;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// If pure device and HW T&L supported
	if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
		devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
			devBehaviorFlags |= D3DCREATE_PUREDEVICE;

	// Step 4: Fill out the D3DPRESENT_PARAMETERS structure.

	m_d3dPP.BackBufferWidth            = 0; 
	m_d3dPP.BackBufferHeight           = 0;
	m_d3dPP.BackBufferFormat           = D3DFMT_UNKNOWN;
	m_d3dPP.BackBufferCount            = 1;
	m_d3dPP.MultiSampleType            = D3DMULTISAMPLE_NONE;
	m_d3dPP.MultiSampleQuality         = 0;
	m_d3dPP.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	m_d3dPP.hDeviceWindow              = m_hMainWnd;
	m_d3dPP.Windowed                   = true;
	m_d3dPP.EnableAutoDepthStencil     = true; 
	m_d3dPP.AutoDepthStencilFormat     = D3DFMT_D24S8;
	m_d3dPP.Flags                      = 0;
	m_d3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dPP.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;


	// Step 5: Create the device.

	HR(m_d3dObject->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		m_DevType,           // device type
		m_hMainWnd,          // window associated with device
		devBehaviorFlags,   // vertex processing
	    &m_d3dPP,            // present parameters
	    &g_d3dDevice));      // return created device
}

int D3DApp::run()
{
	MSG  msg;
    msg.message = WM_NULL;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			// If the application is paused then free some CPU cycles to other 
			// applications and then continue on to the next frame.
			if( m_bAppPaused )
			{
				Sleep(20);
				continue;
			}

			if( !isDeviceLost() )
			{
				__int64 currTimeStamp = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
				float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;

				updateScene(dt);
				drawScene();

				// Prepare for next iteration: The current time stamp becomes
				// the previous time stamp for the next iteration.
				prevTimeStamp = currTimeStamp;
			}
        }
    }
	return (int)msg.wParam;
}

void D3DApp::render()
{
	// If there are Window messages then process them.
	if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
	{
        TranslateMessage( &msg );
        DispatchMessage( &msg );
	}
	// Otherwise, do animation/game stuff.
	else
    {	
		// If the application is paused then free some CPU cycles to other 
		// applications and then continue on to the next frame.
		if( m_bAppPaused )
		{
			Sleep(20);
			return;
		}

		if( !isDeviceLost() )
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;

			updateScene(dt);
			drawScene();

			// Prepare for next iteration: The current time stamp becomes
			// the previous time stamp for the next iteration.
			prevTimeStamp = currTimeStamp;
		}
    }
}

void D3DApp::initRender()
{
	msg.message = WM_NULL;
	cntsPerSec = 0;
	secsPerCnt = 1.0f / (float)cntsPerSec;
	prevTimeStamp = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);
}

LRESULT D3DApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Is the application in a minimized or maximized state?
	static bool minOrMaxed = false;

	RECT clientRect = {0, 0, 0, 0};
	switch( msg )
	{

	// WM_ACTIVE is sent when the window is activated or deactivated.
	// We pause the game when the main window is deactivated and 
	// unpause it when it becomes active.
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
			m_bAppPaused = true;
		else
			m_bAppPaused = false;
		return 0;


	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		if( g_d3dDevice )
		{
			m_d3dPP.BackBufferWidth  = LOWORD(lParam);
			m_d3dPP.BackBufferHeight = HIWORD(lParam);

			if( wParam == SIZE_MINIMIZED )
			{
				m_bAppPaused = true;
				minOrMaxed = true;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				m_bAppPaused = false;
				minOrMaxed = true;
				onLostDevice();
				HR(g_d3dDevice->Reset(&m_d3dPP));
				onResetDevice();
			}
			// Restored is any resize that is not a minimize or maximize.
			// For example, restoring the window to its default size
			// after a minimize or maximize, or from dragging the resize
			// bars.
			else if( wParam == SIZE_RESTORED )
			{
				m_bAppPaused = false;

				// Are we restoring from a mimimized or maximized state, 
				// and are in windowed mode?  Do not execute this code if 
				// we are restoring to full screen mode.
				if( minOrMaxed && m_d3dPP.Windowed )
				{
					onLostDevice();
					HR(g_d3dDevice->Reset(&m_d3dPP));
					onResetDevice();
				}
				else
				{
					// No, which implies the user is resizing by dragging
					// the resize bars.  However, we do not reset the device
					// here because as the user continuously drags the resize
					// bars, a stream of WM_SIZE messages is sent to the window,
					// and it would be pointless (and slow) to reset for each
					// WM_SIZE message received from dragging the resize bars.
					// So instead, we reset after the user is done resizing the
					// window and releases the resize bars, which sends a
					// WM_EXITSIZEMOVE message.
				}
				minOrMaxed = false;
			}
		}
		return 0;


	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		GetClientRect(m_hMainWnd, &clientRect);
		m_d3dPP.BackBufferWidth  = clientRect.right;
		m_d3dPP.BackBufferHeight = clientRect.bottom;
		onLostDevice();
		HR(g_d3dDevice->Reset(&m_d3dPP));
		onResetDevice();

		return 0;

	// WM_CLOSE is sent when the user presses the 'X' button in the
	// caption bar menu.
	case WM_CLOSE:
		DestroyWindow(m_hMainWnd);
		return 0;

	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			enableFullScreenMode(false);
		else if( wParam == 'F' )
			enableFullScreenMode(true);
		return 0;
	}
	return DefWindowProc(m_hMainWnd, msg, wParam, lParam);
}

void D3DApp::enableFullScreenMode(bool enable)
{
	// Switch to fullscreen mode.
	if( enable )
	{
		// Are we already in fullscreen mode?
		if( !m_d3dPP.Windowed ) 
			return;

		int width  = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		m_d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		m_d3dPP.BackBufferWidth  = width;
		m_d3dPP.BackBufferHeight = height;
		m_d3dPP.Windowed         = false;

		// Change the window style to a more fullscreen friendly style.
		SetWindowLongPtr(m_hMainWnd, GWL_STYLE, WS_POPUP);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(m_hMainWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);	
	}
	// Switch to windowed mode.
	else
	{
		// Are we already in windowed mode?
		if( m_d3dPP.Windowed ) 
			return;

		RECT R = {0, 0, 800, 600};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		m_d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
		m_d3dPP.BackBufferWidth  = 800;
		m_d3dPP.BackBufferHeight = 600;
		m_d3dPP.Windowed         = true;
	
		// Change the window style to a more windowed friendly style.
		SetWindowLongPtr(m_hMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(m_hMainWnd, HWND_TOP, 100, 100, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	// Reset the device with the changes.
	onLostDevice();
	HR(g_d3dDevice->Reset(&m_d3dPP));
	onResetDevice();
}

bool D3DApp::isDeviceLost()
{
	// Get the state of the graphics device.
	HRESULT hr = g_d3dDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset yet then
	// sleep for a bit and we'll try again on the next 
	// message loop cycle.
	if( hr == D3DERR_DEVICELOST )
	{
		Sleep(20);
		return true;
	}
	// Driver error, exit.
	else if( hr == D3DERR_DRIVERINTERNALERROR )
	{
		MessageBox(0, L"Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	// The device is lost but we can reset and restore it.
	else if( hr == D3DERR_DEVICENOTRESET )
	{
		onLostDevice();
		HR(g_d3dDevice->Reset(&m_d3dPP));
		onResetDevice();
		return false;
	}
	else
		return false;
}