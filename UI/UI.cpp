//////////////////////////////////////////////////
// 
// UI Example App
//
//////////////////////////////////////////////////

//======================================
// dependencies
#include "stdafx.h"
#include "Inventory.h"   // INVENTORY_EXAMPLE
#include "SpriteStrip.h" // SPRITESTRIP_EXAMPLE
#include "StatusBar.h"   // STATUSBAR_EXAMPLE
#include "StatusIcon.h"  // STATUSICON_EXAMPLE
#include "TextBox.h"     // TEXTBOX_EXAMPLE

#define SCREEN_WIDTH  640 // the width of the window
#define SCREEN_HEIGHT 480 // the height of the window

// Globals
HINSTANCE hInst;                                // current instance
TCHAR szTitle[] = TEXT("UI Sample");            // The title bar text
TCHAR szWindowClass[] = TEXT("Game Engine");    // the main window class name
HWND hWnd;                                      // global window handle
Inventory inventory; // INVENTORY_EXAMPLE

//======================================
// forward declarations
BOOL    InitWindow(HINSTANCE hInstance, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitDirect3D(HWND hwnd);
void ShutdownDirect3D(void);
void InventorySetup(void); // INVENTORY_EXAMPLE

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	// call our function to init and create our window
	if (!InitWindow(hInstance, nCmdShow))
	{
		MessageBox(NULL, TEXT("Unable to create window"), TEXT("ERROR"), MB_OK);
		return FALSE;
	}

	if (!InitDirect3D(hWnd))
	{
		MessageBox(NULL, TEXT("Unable to init Direct3D"), TEXT("ERROR"), MB_OK);
		return FALSE;
	}

	InventorySetup(); // INVENTORY_EXAMPLE - build the inventory
	//HUD SAMPLE
	//main declarations
	SpriteStrip barSprites(L"_/misc/barca_assets/bar.png", 4);
	SpriteStrip CreditSprites(L"_/misc/barca_assets/credits.png", 8);

	//healthbar
	float playerHealth = 100.0;
	float maxPlayerHealth = playerHealth;
	int healthBarImageID = 0;
	StatusBar playerHealthBar(L"Health", 5, 5, healthBarImageID, playerHealth, maxPlayerHealth, barSprites );
	/*
	//when health is modified
	if(playerHealth >= 50.0){
		healthBarImageID = 0;
	}else{
		if(playerHealth >= 25.0){
			healthBarImageID = 1;
		}else{
			healthBarImageID = 2;
		}
	}
	playerHealthBar.changeImage(healthBarImageID);
	playerHealthBar.modifyValue(playerHealth, barSprites);
	*/
	//stamina bar
	float playerStamina = 100.0;
	float maxPlayerStamina = playerStamina;
	StatusBar playerStaminaBar(L"Stamina", 5, 26, 4, playerStamina, maxPlayerStamina, barSprites );
	
	/*
	//when stamina is modified
	playerStaminaBar.modifyValue(playerStamina, barSprites);
	*/
	
	//credits

	bool showCredits = true;
	int li = 0;
	int crSlide = 0;
	StatusIcon credits(L"credits", SCREEN_WIDTH/4, SCREEN_HEIGHT/4, crSlide, CreditSprites);
	//END HUD SAMPLE

	// Main message loop:
	// Enter the message loop
	memset(&msg, 0, sizeof(MSG));
	while( msg.message!=WM_QUIT )
	{
		// check for messages
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		// this is called when no messages are pending
		else
		{
			// clear background to a white offset
			g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(240, 240, 240), 1.0f, 0);

			if(SUCCEEDED(g_pD3DDevice->BeginScene()))
			{

				inventory.drawElement(); // INVENTORY_EXAMPLE
				//HUD SAMPLE
				playerHealthBar.drawElement(barSprites);
				playerStaminaBar.drawElement(barSprites);
				if(showCredits == true){
					credits.drawElement(CreditSprites);
					li++;
					if (li > 150){
						crSlide++;
						credits.changeImage(crSlide, CreditSprites);

						li = 0;
					}
					if(crSlide > 7){
						li = 0;
						crSlide = 0;
						showCredits = false;
					}else{
						showCredits = true;
					}
				}
				//END HUD SAMPLE

				g_pD3DDevice->EndScene();
			}

			// Present the backbuffer contents to the display
			g_pD3DDevice->Present( NULL, NULL, NULL, NULL );
		}
	}

	// release and shutdown Direct3D
	ShutdownDirect3D();

	return (int) msg.wParam;
}

//////////////////////////////////////////////////
//
// PURPOSE: Saves instance handle and creates main window and
//            registers the window class.
//
// COMMENTS:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
//////////////////////////////////////////////////
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= 0;

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//////////////////////////////////////////////////
//
// PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT  - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_KEYDOWN:
		int fwKeys;
		LPARAM keyData;
		fwKeys = (int)wParam;      // virtual-key code 
		keyData = lParam;          // key data 

		switch(fwKeys)
		{
		case VK_SPACE:
			inventory.swapItem(1, 2); // INVENTORY_EXAMPLE
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

BOOL InitDirect3D(HWND hwnd)
{
	if( NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) )
		return FALSE;

	D3DPRESENT_PARAMETERS D3Dpp; // holds properties about how an application should behave
	memset(&D3Dpp, 0, sizeof(D3DPRESENT_PARAMETERS));

	D3Dpp.Windowed = TRUE;                     // run in windowed mode
	D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  // basically it tells the device driver to select the most efficient way of handling swap chains
	D3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	D3Dpp.BackBufferCount  = 1;
	D3Dpp.BackBufferHeight = SCREEN_HEIGHT;
	D3Dpp.BackBufferWidth  = SCREEN_WIDTH;
	D3Dpp.EnableAutoDepthStencil = TRUE;       
	D3Dpp.AutoDepthStencilFormat = D3DFMT_D16; 
	D3Dpp.hDeviceWindow    = hwnd;

	if( FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3Dpp, &g_pD3DDevice)) )
		if( FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3Dpp, &g_pD3DDevice)) )
			return FALSE;

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	return TRUE;
}

void ShutdownDirect3D()
{
	if( g_pD3DDevice != NULL) 
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if( g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

// INVENTORY_EXAMPLE
void InventorySetup()
{
	const std::wstring SLOTS_IMAGE = TEXT("item.png");            // filename of the image to use for the background of a slot
	const std::wstring BACKGROUND_IMAGE = TEXT("background.png"); // filename of the image to use for the background of the inventory
	const signed int MAX_SLOTS = 10;				              // the number of slots the inventory can hold
	const signed int SLOT_SIZE = 32;				              // the width or height of the slot
	const signed int ROW_SIZE =  5;				                  // the number of slots per row
	const signed int GAP_SIZE = 10;                               // size of gap between items
	int posX(200), posY(200);                                     // position of the inventory object
	const DWORD COLOR = 0x000000;                                 // background color for inventory

	// set the properties of the inventory - the second argument can be a filename or a color
	inventory.setProperty(SLOTS_IMAGE, BACKGROUND_IMAGE, MAX_SLOTS, SLOT_SIZE, ROW_SIZE, GAP_SIZE, posX, posY);

	// add some icons to the list
	inventory.newIcon(TEXT("item1.png"), 1);
	inventory.newIcon(TEXT("item2.png"), 2);
	inventory.newIcon(TEXT("item3.png"), 3);

	// add some items to the list
	inventory.newItem(TEXT("Item 1"), 1);
	inventory.newItem(TEXT("Item 2"), 2);
	inventory.newItem(TEXT("Item 3"), 3);
}
// !INVENTORY_EXAMPLE
