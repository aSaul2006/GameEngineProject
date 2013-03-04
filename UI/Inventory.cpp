#include "stdafx.h"
#include "Inventory.h"


std::vector<Inventory::Item> Inventory::ItemList; // needs to be defined in order to make them static
std::vector<Inventory::Icon> Inventory::IconList; // needs to be defined in order to make them static

Inventory::Inventory()
{
	emptyIcon.srcRect.left   = 0;
	emptyIcon.srcRect.top    = 0;
	emptyIcon.srcRect.right  = 0;
	emptyIcon.srcRect.bottom = 0;
	emptyIcon.surface        = NULL;

	emptyItem.destRect.left   = 0;
	emptyItem.destRect.top    = 0;
	emptyItem.destRect.right  = 0;
	emptyItem.destRect.bottom = 0;
	emptyItem.itemName        = TEXT("");
	emptyItem.itemID          = -1;
	emptyItem.itemIcon        = emptyIcon;
}

Inventory::~Inventory()
{
	if( m_pVertexObject != NULL)
	{
		m_pVertexObject->Release();
		m_pVertexObject = NULL;
	}
}

bool Inventory::Item::operator==(const Item& rhs) const
{
	if (this->itemID == rhs.itemID)
		return true;
	else
		return false;
}
bool Inventory::Item::operator!=(const Item& rhs) const
{
	if (this->itemID == rhs.itemID)
		return false;
	else
		return true;
}

HRESULT Inventory::setProperty(std::wstring fn, DWORD color, int n, int l, int r, int g, int x, int y)
{
	m_iHOLocX = x;
	m_iHOLocY = y;
	m_iMaxSlots = n;
	m_iSlotSize = l;
	m_iRowSize = r;
	m_iGapSize = g;

	int tmp_rows = int( std::floor(float( m_iMaxSlots/m_iRowSize )) ); // temporary variable holding number of rows in the inventory
	RECT size = {
		0,
		0,
		( (m_iSlotSize*m_iRowSize)+((m_iRowSize+1)*m_iGapSize) ), // automatically resize the width
		( (m_iSlotSize*tmp_rows  )+((tmp_rows  +1)*m_iGapSize) )  // automatically resize the height
	}; // size of the inventory

	IconList.resize(n+1, emptyIcon); // add one because 0 is reserved for empty icon
	ItemList.resize(n, emptyItem); // fills the inventory with empty slots

	// fill the inventory with empty slots
	for (std::vector<Item>::iterator it = ItemList.begin(); it != ItemList.end(); ++it)
	{
		int i = std::distance(ItemList.begin(), it); // temp variable that stores the current index position
		newIcon(fn, i+1); // add one because 0 is reserved for empty icon
		newItem(fn, i+1, i, -1); // add one because 0 is reserved for empty icon
	}

	//==============================================
	// prepare the drawing code for rendering

	HRESULT hResult;

	// vertecies for the inventory rectangle
	m_Vertices[0].x = m_iHOLocX+(float)size.left;  m_Vertices[0].y = m_iHOLocX+(float)size.top;
	m_Vertices[1].x = m_iHOLocX+(float)size.right; m_Vertices[1].y = m_iHOLocX+(float)size.top;
	m_Vertices[2].x = m_iHOLocX+(float)size.right; m_Vertices[2].y = m_iHOLocX+(float)size.bottom;
	m_Vertices[3].x = m_iHOLocX+(float)size.left;  m_Vertices[3].y = m_iHOLocX+(float)size.bottom;
	m_Vertices[0].z = m_Vertices[1].z = m_Vertices[2].z = m_Vertices[3].z = 0;
	m_Vertices[0].rhw   = m_Vertices[1].rhw   = m_Vertices[2].rhw   = m_Vertices[3].rhw = 1.0f;
	m_Vertices[0].color = m_Vertices[1].color = m_Vertices[2].color = m_Vertices[3].color = color;

	// create the vertex buffer
	m_pVertexObject = NULL;
	void *pVertexBuffer = NULL;

	// allocates a vertex buffer object which we'll use for all buffer operations
	if(FAILED(hResult = g_pD3DDevice->CreateVertexBuffer(
		4*sizeof(CUSTOMVERTEX),       // tells DirectX the required size of the vertex buffer (in bytes).
		0,                            // specifies how the vertex buffer will be used – "0" being the default.
		D3DFVF_XYZRHW|D3DFVF_DIFFUSE, // tells DirectX about the memory layout of each vertex (the FVF format).
		D3DPOOL_DEFAULT,              // says that you don't care where memory is allocated.
		&m_pVertexObject,             // the address of a pointer to be filled with the vertex buffer object location.
		NULL)))                       // specifies a shared handle (don't worry about this).
		return hResult;

	// a memory buffer (pointed to by pVertexBuffer) that we must copy our vertex data into
	if(FAILED(hResult = m_pVertexObject->Lock(0, 4*sizeof(CUSTOMVERTEX), &pVertexBuffer, 0)))
		return hResult;

	memcpy(pVertexBuffer, m_Vertices, 4*sizeof(CUSTOMVERTEX)); // copy the vertex
	m_pVertexObject->Unlock();

	return 0;
}

HRESULT Inventory::setProperty(std::wstring fn, std::wstring bk, int n, int l, int r, int g, int x, int y)
{
	m_iHOLocX = x;
	m_iHOLocY = y;
	m_iMaxSlots = n;
	m_iSlotSize = l;
	m_iRowSize = r;
	m_iGapSize = g;

	int tmp_rows = int( std::floor(float( m_iMaxSlots/m_iRowSize )) ); // temporary variable holding number of rows in the inventory
	RECT size = {
		0,
		0,
		( (m_iSlotSize*m_iRowSize)+((m_iRowSize+1)*m_iGapSize) ), // automatically resize the width
		( (m_iSlotSize*tmp_rows  )+((tmp_rows  +1)*m_iGapSize) )  // automatically resize the height
	}; // size of the inventory

	IconList.resize(n+1, emptyIcon); // add one because 0 is reserved for empty icon
	ItemList.resize(n, emptyItem); // fills the inventory with empty slots

	// fill the inventory with empty slots
	for (std::vector<Item>::iterator it = ItemList.begin(); it != ItemList.end(); ++it)
	{
		int i = std::distance(ItemList.begin(), it); // temp variable that stores the current index position
		newIcon(fn, i+1); // add one because 0 is reserved for empty icon
		newItem(fn, i+1, i, -1); // add one because 0 is reserved for empty icon
	}

	//==============================================
	// prepare the drawing code for rendering

	HRESULT            hResult;
	IDirect3DSurface9* tmpSurface = NULL;
	D3DXIMAGE_INFO     tmpInfo; // holds infomation about an image

	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(bk.c_str(), &tmpInfo);
	if (FAILED(hResult))
		return hResult;

	// create a temporary surface
	hResult = g_pD3DDevice->CreateOffscreenPlainSurface(
		tmpInfo.Width,       // width of the surface
		tmpInfo.Height,      // height of the surface
		D3DFMT_A8R8G8B8,     // format of the structure
		D3DPOOL_DEFAULT,     // surface pool type
		&tmpSurface,         // Pointer to the IDirect3DSurface9 interface created.
		NULL                 // Reserved. Set this parameter to NULL.
		);
	if (FAILED(hResult))
		return hResult;

	// adds the image to the surface
	hResult = D3DXLoadSurfaceFromFile(tmpSurface, NULL, NULL, bk.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return hResult;

	// setup the rectangle to read image
	m_IProp.srcRect.left = 0;
	m_IProp.srcRect.top = 0;
	m_IProp.srcRect.right = tmpInfo.Width;
	m_IProp.srcRect.bottom = tmpInfo.Height;

	// setup the rectangle to draw the iamge
	m_IProp.destRect.left = long( m_iHOLocX+(float)size.left );
	m_IProp.destRect.top = long( m_iHOLocX+(float)size.top );
	m_IProp.destRect.right = long( m_iHOLocX+(float)size.right );
	m_IProp.destRect.bottom = long( m_iHOLocX+(float)size.bottom );

	// save the image for later use
	m_IProp.surface = tmpSurface;

	return 0;
}

HRESULT Inventory::newIcon(std::wstring fileName, int iconID)
{
	HRESULT            hResult;
	IDirect3DSurface9* tmpSurface = NULL;
	D3DXIMAGE_INFO     tmpInfo; // holds infomation about an image

	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(fileName.c_str(), &tmpInfo);
	if (FAILED(hResult))
		return hResult;

	// create a temporary surface
	hResult = g_pD3DDevice->CreateOffscreenPlainSurface(
		tmpInfo.Width,       // width of the surface
		tmpInfo.Height,      // height of the surface
		D3DFMT_A8R8G8B8,     // format of the structure
		D3DPOOL_DEFAULT,     // surface pool type
		&tmpSurface,         // Pointer to the IDirect3DSurface9 interface created.
		NULL                 // Reserved. Set this parameter to NULL.
		);
	if (FAILED(hResult))
		return hResult;

	// adds the image to the surface
	hResult = D3DXLoadSurfaceFromFile(tmpSurface, NULL, NULL, fileName.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return hResult;

	IconList.at(iconID).srcRect.left = 0;
	IconList.at(iconID).srcRect.top = 0;
	IconList.at(iconID).srcRect.right = tmpInfo.Width;
	IconList.at(iconID).srcRect.bottom = tmpInfo.Height;
	IconList.at(iconID).surface = tmpSurface;

	return 0;
}

BOOL Inventory::newItem(std::wstring itemName, int iconID)
{
	int col;        // column in inventory that an item is in
	int row;        // row in inventory that an item is in
	int colGap = m_iGapSize; // gap between the columns
	int rowGap = m_iGapSize; // gap between the rows

	// loop through the container to find an empty slot, then insert the new item in that position
	for (std::vector<Item>::iterator it = ItemList.begin(); it != ItemList.end(); ++it)
	{
		int i = std::distance(ItemList.begin(), it); // temp variable that stores the current index position

		// if an empty slot is found, insert the item in that position
		if (ItemList.at(i) == emptyItem)
		{
			Item tmp = {itemName, i, IconList.at(iconID)}; // create a temporary item

			++i;                                                // set index relative to 1, for correct calculations
			col = 1 + ( (i-1)%m_iRowSize );                     // find which column the item is in
			row = int( 1+std::floor(float((i-1)/m_iRowSize)) ); // find which row the item is in
			--i;                                                // make the index relative to 0 again

			if (col == 1)
				tmp.destRect.left =   long( m_iHOLocX+colGap );
			else
			{
				colGap = col * colGap;
				tmp.destRect.left =   long( m_iHOLocX+((col-1)*m_iSlotSize)+colGap );
			}

			if (row == 1)
				tmp.destRect.top =    long( m_iHOLocY+rowGap );
			else
			{
				rowGap = row * rowGap;
				tmp.destRect.top =    long( m_iHOLocY+((row-1)*m_iSlotSize)+rowGap );
			}
			tmp.destRect.right =  long( tmp.destRect.left+m_iSlotSize );
			tmp.destRect.bottom = long( tmp.destRect.top+m_iSlotSize );

			ItemList.at(i) = tmp; // add the item to the container

			return TRUE; // inserted an item successfully
		}
	}

	return FALSE; // error occured
}

void Inventory::newItem(std::wstring itemName, int iconID, signed int itemLoc, int itemID)
{
	// check if the item location is valid and if it's empty then insert the item in that position
	if (isValid(itemLoc) && ItemList.at(itemLoc) == emptyItem)
	{
		Item tmp = {itemName, itemID, IconList.at(iconID)}; // create a temporary item
		int colGap = m_iGapSize; // gap between the columns
		int rowGap = m_iGapSize; // gap between the rows

		++itemLoc;                                                    // set index relative to 1, for correct calculations
		int col = 1 + ( (itemLoc-1)%m_iRowSize );                     // find which column the item is in
		int row = int( 1+std::floor(float((itemLoc-1)/m_iRowSize)) ); // find which row the item is in
		--itemLoc;                                                    // make the index relative to 0 again

		if (col == 1)
			tmp.destRect.left =   long( m_iHOLocX+colGap );
		else
		{
			colGap = col * colGap;
			tmp.destRect.left =   long( m_iHOLocX+((col-1)*m_iSlotSize)+colGap );
		}

		if (row == 1)
			tmp.destRect.top =    long( m_iHOLocY+rowGap );
		else
		{
			rowGap = row * rowGap;
			tmp.destRect.top =    long( m_iHOLocY+((row-1)*m_iSlotSize)+rowGap );
		}
		tmp.destRect.right =  long( tmp.destRect.left+m_iSlotSize );
		tmp.destRect.bottom = long( tmp.destRect.top+m_iSlotSize );

		ItemList.at(itemLoc) = tmp; // add the item to the container
	}
}

void Inventory::removeItem(signed int itmLoc)
{
	// check if the item location is valid and if it's empty then insert the item in that position
	if (isValid(itmLoc) && ItemList.at(itmLoc) != emptyItem)
	{
		int tmpX = 1 + ( (itmLoc-1)%m_iRowSize );                     // find which column the item is in
		int tmpY = int( 1+std::floor(float((itmLoc-1)/m_iRowSize)) ); // find which row the item is in
		--itmLoc;                                                     // make the index relative to 0 again

		emptyItem.destRect.left =   long( m_iHOLocX+(tmpX*m_iSlotSize) );
		emptyItem.destRect.top =    long( m_iHOLocY+(tmpY*m_iSlotSize) );
		emptyItem.destRect.right =  long( emptyItem.destRect.left+m_iSlotSize );
		emptyItem.destRect.bottom = long( emptyItem.destRect.top+m_iSlotSize );

		ItemList.at(itmLoc) = emptyItem;
	}
}

void Inventory::removeItem(std::wstring itemName)
{
	// loop through the container and remove all instances of an item
	for (std::vector<Item>::iterator it = ItemList.begin(); it != ItemList.end(); ++it)
	{
		int i = std::distance(ItemList.begin(), it); // temp variable that stores the current index position

		// if the item exists, empty the element in ItemList
		if (isExist(itemName, i))
		{
			++i;                                                     // set index relative to 1, for correct calculations
			int tmpX = 1 + ( (i-1)%m_iRowSize );                     // find which column the item is in
			int tmpY = int( 1+std::floor(float((i-1)/m_iRowSize)) ); // find which row the item is in
			--i;                                                     // make the index relative to 0 again

			emptyItem.destRect.left =   long( m_iHOLocX+(tmpX*m_iSlotSize) );
			emptyItem.destRect.top =    long( m_iHOLocY+(tmpY*m_iSlotSize) );
			emptyItem.destRect.right =  long( emptyItem.destRect.left+m_iSlotSize );
			emptyItem.destRect.bottom = long( emptyItem.destRect.top+m_iSlotSize );

			ItemList.at(i) = emptyItem;
		}
	}
}

void Inventory::swapItem(signed int itemLoc1, signed int itemLoc2)
{
	if (isValid(itemLoc1) && isValid(itemLoc2)) // check that both positions are valid
	{
		Item tmp = emptyItem; // temporary item to hold data between swaps
		--itemLoc1;           // set the location relative to 0
		--itemLoc2;           // set the location relative to 0

		if (!isEmpty(itemLoc1))
		{ // check if the first item is empty, if so, skip to decrease redundancy
			tmp = ItemList.at(itemLoc1);                   // save the first item in a temporary item
			ItemList.at(itemLoc1) = ItemList.at(itemLoc2); // overwrite the first item with the second item
			ItemList.at(itemLoc1).destRect = tmp.destRect; // keep the drawing destination the same
			tmp.destRect = ItemList.at(itemLoc2).destRect; // keep the drawing destination the same
			ItemList.at(itemLoc2) = tmp;                   // overwrite the second item with the first/temporary item
		}
		else if (!isEmpty(itemLoc2))
		{ // check if the second item is empty, if so, skip to decrease redundancy
			tmp = ItemList.at(itemLoc2);                   // save the second item in a temporary item
			ItemList.at(itemLoc2) = emptyItem;             // empty the second item directly since we know n is empty
			ItemList.at(itemLoc2).destRect = tmp.destRect; // keep the drawing destination the same
			tmp.destRect = ItemList.at(itemLoc1).destRect; // keep the drawing destination the same
			ItemList.at(itemLoc1) = tmp;                   // overwrite the second item with the first/temporary item
		}
		else // both positions were empty, don't do anything
			return;
	}
}

void Inventory::drawElement()
{
	// get the backbuffer
	g_pD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &g_pBackBuffer);

	if (m_IProp.surface != NULL)
	{
		// draw the inventory background with an image
		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		g_pD3DDevice->StretchRect(m_IProp.surface, &m_IProp.srcRect, g_pBackBuffer, &m_IProp.destRect, D3DTEXF_NONE);
	}
	else if (m_pVertexObject != NULL)
	{
		// draw the inventory background with a color
		g_pD3DDevice->SetStreamSource(0, m_pVertexObject, 0, sizeof(CUSTOMVERTEX));
		g_pD3DDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}

	// loop through the container and draw the slots
	for (std::vector<Item>::iterator it = ItemList.begin(); it != ItemList.end(); ++it)
	{
		int i = std::distance(ItemList.begin(), it); // temp variable that stores the current index position
		g_pD3DDevice->StretchRect(ItemList.at(i).itemIcon.surface, &ItemList.at(i).itemIcon.srcRect, g_pBackBuffer, &ItemList.at(i).destRect, D3DTEXF_NONE);
	}
}

bool Inventory::isValid(signed int n)
{
	// check if the item ID exceeds the max number of slots in the inventory
	if (n > m_iMaxSlots || n < 0)
		return false; // error - ID exceeds inventory limit
	return true;
}

bool Inventory::isExist(Item tmp)
{
	// make sure that the item doesn't already exists at a given location in the inventory
	if (ItemList.at(tmp.itemID) == tmp)
		return true;
	return false;
}

bool Inventory::isExist(std::wstring s, signed int n)
{
	// make sure that the item doesn't already exists at a given location in the inventory
	if (ItemList.at(n).itemName == s)
		return true;
	return false;
}

bool Inventory::isEmpty(signed int n)
{
	// check if the element at the given location is empty
	if (ItemList.at(n) == emptyItem)
		return true;
	return false;
}
