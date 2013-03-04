//////////////////////////////////////////////////
//
// UIInterface.h: A class interface for the UI
//
//////////////////////////////////////////////////

//======================================
// include guard
#ifndef UIINTERFACE_H
#define UIINTERFACE_H

//======================================
// dependencies
//#include "MainInterface.h" add when needed
#include "Inventory.h"
#include "SpriteStrip.h"
#include "TextBox.h"
#include "StatusIcon.h"
#include "StatusBar.h"

//#include <string>

//======================================
// the code

#pragma once
class UIInterface
{
	//MainInterface* render; add when needed
	Inventory m_Inventory; // inventory object
public:
	UIInterface(void);
	~UIInterface(void);

	//======================================================
	// Inventory functions
	//======================================================

	/* -----------------------------------------------------
	Purpose: Sets the properties of the inventory
	Parameter 1: Filename of an image to use as the default/empty slot for the inventory
	Parameter 2: Color to use for the background of the inventory
	Parameter 3: The max number of slots available in the inventory
	Parameter 4: The height or width of a slot (slots are square)
	Parameter 5: The number of slots in a row
	Parameter 6: Size of a gap to show between slots
	Parameter 7: X-Coordinate on the screen of the inventory
	Parameter 8: Y-Coordinate on the screen of the inventory
	------------------------------------------------------*/
	void Inventory_Build(std::wstring, DWORD, signed int, int, int, int, int, int);

	/* -----------------------------------------------------
	Purpose: Sets the properties of the inventory
	Parameter 1: Filename of an image to use as the default/empty slot for the inventory
	Parameter 2: Filename of an image to use as the inventory background
	Parameter 3: The max number of slots available in the inventory
	Parameter 4: The height or width of a slot (slots are square)
	Parameter 5: The number of slots in a row
	Parameter 6: Size of a gap to show between slots
	Parameter 7: X-Coordinate on the screen of the inventory
	Parameter 8: Y-Coordinate on the screen of the inventory
	------------------------------------------------------*/
	void Inventory_Build(std::wstring, std::wstring, signed int, int, int, int, int, int);

	/* -----------------------------------------------------
	Purpose: Creates a new icon to use with an item afterwards
	Parameter 1: Filename of an image
	Parameter 2: Unique number to represent the icon (unique only within the inventory)
	------------------------------------------------------*/
	void Inventory_CreateIcon(std::wstring, int);

	/* -----------------------------------------------------
	Purpose: Adds an item to the inventory
	Parameter 1: Name of the item
	Parameter 2: The number (ID) of an icon to use for the item
	------------------------------------------------------*/
	void Inventory_AddItem(std::wstring, int);

	/* -----------------------------------------------------
	Purpose: Adds an item to the inventory
	Parameter 1: Name of the item
	Parameter 2: The number (ID) of an icon to use for the item
	Parameter 3: The location in the inventory to place the item (0-index)
	Parameter 4: Unique number to represent the item (unique only within the inventory)
	------------------------------------------------------*/
	void Inventory_AddItem(std::wstring, int, signed int, int);

	/* -----------------------------------------------------
	Purpose: Removes an item from the inventory
	Parameter 1: The slot number (location) of the item (1-index)
	------------------------------------------------------*/
	void Inventory_RemoveItem(signed int);

	/* -----------------------------------------------------
	Purpose: Removes all instances of an item from the inventory
	Parameter 1: The name of an item (ID is not used because same items have unique IDs)
	------------------------------------------------------*/
	void Inventory_RemoveItem(std::wstring);

	/* -----------------------------------------------------
	Purpose: Swaps the items of two slots; if one slot is empty, it moves the item to the slot
	Parameter 1: Location of the first item (0-index)
	Parameter 2: Location of the second item (0-index)
	------------------------------------------------------*/
	void Inventory_SwapSlots(signed int, signed int);

	/* -----------------------------------------------------
	Purpose: Draws the inventory on the screen
	------------------------------------------------------*/
	void Inventory_Draw();

	//======================================================
	// HUD functions
	//======================================================

	/*-----------------------------------------------------------------
	Purpose: creates a statusbar
	-------------------------------------------------------------------*/
	StatusBar CreateStatusBar(std::wstring, int, int, int, float, float, SpriteStrip );

	/*-----------------------------------------------------------------
	Purpose: creates a statusicon
	-------------------------------------------------------------------*/
	StatusIcon CreateStatusIcon(std::wstring, int, int, int, SpriteStrip );

	/*-----------------------------------------------------------------
	Purpose: creates a textbox
	-------------------------------------------------------------------*/
	TextBox CreateTextBox(std::wstring, int, int, DWORD, std::wstring );

	/*-----------------------------------------------------------------
	Purpose: Draws the status bar on the screen
	-------------------------------------------------------------------*/
	void drawStatusBar(StatusBar, float, SpriteStrip, bool );

	/*-----------------------------------------------------------------
	Purpose: Draws the status icon or can be used as 2D animation if needed
	-------------------------------------------------------------------*/
	void drawStatusIcon(StatusIcon, int, SpriteStrip, bool);

	/*-----------------------------------------------------------------
	Purpose: Draws a text box
	-------------------------------------------------------------------*/
	void drawTextBox(TextBox, std::wstring, DWORD, bool, bool);


	//======================================================
	// Control functions
	//======================================================

	void keyPress();


	//======================================================
	// Menu functions
	//======================================================


};

#endif // !UIINTERFACE_H
