//////////////////////////////////////////////////
//
// Inventory.h: A class for an inventory window which also
// handles items inside said inventory.
//
//////////////////////////////////////////////////

//======================================
// include guard
#ifndef UI_INVENTORY_H
#define UI_INVENTORY_H

//======================================
// dependencies
#include "HUDObject.h"
#include <string>
#include <vector>

//======================================
// the code

class Inventory :
	public HUDObject
{
	// structure containing inventory properties
	struct {
		RECT srcRect;				// holds the location of this sprite in the source bitmap
		RECT destRect;              // holds the location to draw on the screen
		IDirect3DSurface9* surface; // holds the image itself} m_IProp;
	} m_IProp;

	// structure for an icon
	struct Icon
	{
		RECT srcRect;				// holds the location of this sprite in the source bitmap
		IDirect3DSurface9* surface; // holds the image itself
	};

	// structure for an item
	struct Item
	{
		std::wstring itemName; // the name of an item in the inventory
		int itemID; // the id of an item
		Icon itemIcon; // the icon for the item
		RECT destRect; // location of the icon for the item
		bool operator==(const Item&) const;
		bool operator!=(const Item&) const;
	};

	signed int m_iMaxSlots; // number of slots in the inventory
	int m_iSlotSize; // the width or height of a slot
	int m_iRowSize; // number of slots per row
	int m_iGapSize; // size of gap between items
	LPDIRECT3DVERTEXBUFFER9 m_pVertexObject;
	CUSTOMVERTEX m_Vertices[4];
	Item emptyItem; // used as a placeholder for an element in ItemList
	Icon emptyIcon; // icon for the placeholder

	bool isValid(signed int); // check an item ID to see if it's valid
	bool isExist(Item); // check if the ID of an item is in use
	bool isExist(std::wstring, signed int); // check if the name of an item is in use at a given location
	bool isEmpty(signed int); // check if the given position is empty
public:
	Inventory(void);
	~Inventory(void);

	static std::vector<Item> ItemList; // container with all of the items
	static std::vector<Icon> IconList; // container with all of the icons

	// set the inventory properties
	// (filename for empty slots image, background color for the inventory, max slots in inventory, height or width of a slot, number of slots per row, size of gap between items, x-coord, y-coord)
	HRESULT setProperty(std::wstring, DWORD, signed int, int, int, int, int, int);
	// (filename for empty slots image, filename for inventory background, max slots in inventory, height or width of a slot, number of slots per row, size of gap between items, x-coord, y-coord)
	HRESULT setProperty(std::wstring, std::wstring, signed int, int, int, int, int, int);

	// create an icon for inventory use
	// (filename, image index number)
	HRESULT newIcon(std::wstring, int);

	// create an item for the inventory in first empty slot it finds
	// (item name, image index number)
	BOOL newItem(std::wstring, int);

	// create an item for the inventory; index starts at 0
	// (item name, image index number, location in inventory, id of item)
	void newItem(std::wstring, int, signed int, int);

	// removes an item matching the given locaiton
	// (location in inventory relative to 1)
	void removeItem(signed int);

	// removes all items matching the string from the inventory
	// (item name)
	void removeItem(std::wstring);

	// swaps/moves an item with/to another item/position; index starts at 0
	// (location in inventory, location in inventory)
	void swapItem(signed int, signed int);

	// draw the inventory
	void drawElement();
};

#endif // !UI_INVENTORY_H
