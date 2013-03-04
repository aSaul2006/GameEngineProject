#include "UIInterface.h"


UIInterface::UIInterface(void)
{
}


UIInterface::~UIInterface(void)
{
}

//======================================================
// Inventory functions
//======================================================

void UIInterface::Inventory_Build(std::wstring slotBackground, DWORD inventoryBackground, signed int maxSlots, int slotSize, int rowSize, int gapSize, int posX, int posY)
{
	m_Inventory.setProperty(slotBackground, inventoryBackground, maxSlots, slotSize, rowSize, gapSize, posX, posY);
}

void UIInterface::Inventory_Build(std::wstring slotBackground, std::wstring inventoryBackground, signed int maxSlots, int slotSize, int rowSize, int gapSize, int posX, int posY)
{
	m_Inventory.setProperty(slotBackground, inventoryBackground, maxSlots, slotSize, rowSize, gapSize, posX, posY);
}

void UIInterface::Inventory_CreateIcon(std::wstring iconImage, int iconID)
{
	m_Inventory.newIcon(iconImage, iconID);
}

void UIInterface::Inventory_AddItem(std::wstring itemName, int iconID)
{
	m_Inventory.newItem(itemName, iconID);
}

void UIInterface::Inventory_AddItem(std::wstring itemName, int iconID, signed int itemLocation, int itemID)
{
	m_Inventory.newItem(itemName, iconID, itemLocation, itemID);
}

void UIInterface::Inventory_RemoveItem(signed int itemLocation)
{
	m_Inventory.removeItem(itemLocation);
}

void UIInterface::Inventory_RemoveItem(std::wstring itemName)
{
	m_Inventory.removeItem(itemName);
}

void UIInterface::Inventory_SwapSlots(signed int itemLocation1, signed int itemLocation2)
{
	m_Inventory.swapItem(itemLocation1, itemLocation2);
}

void UIInterface::Inventory_Draw()
{
	m_Inventory.drawElement();
}


//======================================================
// HUD functions
//======================================================

StatusBar UIInterface::CreateStatusBar(std::wstring name, int x, int y, int id, float value, float maxvalue, SpriteStrip imagelocation)
{
	return StatusBar(name, x, y, id, value, maxvalue, imagelocation);
}

StatusIcon UIInterface::CreateStatusIcon(std::wstring name, int x, int y, int id, SpriteStrip imagelocation )
{
	return StatusIcon(name, x, y, id, imagelocation);
}

TextBox UIInterface::CreateTextBox(std::wstring name, int x, int y, DWORD color, std::wstring message)
{
	return TextBox(name, x, y, color, message);
}

void UIInterface::drawStatusBar(StatusBar sb, float val, SpriteStrip imageLocation, bool valwasmodified)
{
	if(valwasmodified == true){
		sb.modifyValue(val, imageLocation);
	}
	sb.drawElement(imageLocation);
}

void UIInterface::drawStatusIcon(StatusIcon si, int id, SpriteStrip imageLocation, bool changeimage)
{
	if (changeimage == true){
		si.changeImage(id, imageLocation);
	}
	si.drawElement(imageLocation);
}

void UIInterface::drawTextBox(TextBox tb, std::wstring message, DWORD color, bool isnewmessage, bool isnewcolor)
{
	if(isnewmessage == true){
		tb.updateText(message);
	}
	if(isnewcolor == true){
		tb.modifyColor(color);
	}
	tb.drawElement();
}


//======================================================
// Control functions
//======================================================

void UIInterface::keyPress()
{

}




//======================================================
// Menu functions
//================
