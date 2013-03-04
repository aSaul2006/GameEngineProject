/************************************************************************
*  Program Name:		ResourceManager.h   							*
*  Name:				          										*
*  Date:				January 25, 2013								*
*  Description:			Code file for ResourceManager class				*
*						                                        		*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/
#pragma once

#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <iostream>
#include <list>
#include <map>
#include <string>

//Tiny XML header, needs tinyXML library
#include "tiny\tinyxml.h"
#include "fmod\include\fmod.h"
#include "fmod\include\fmod.hpp"
#include "fmod\include\fmod_codec.h"
#include "fmod\include\fmod_dsp.h"
#include "fmod\include\fmod_errors.h"
#include "fmod\include\fmod_memoryinfo.h"
#include "fmod\include\fmod_output.h"


//Enum to indicate resource type
typedef enum{
	RESOURCE_NULL = 0,
	RESOURCE_GRAPHIC = 1,
	RESOURCE_TEXTURE = 2,
	RESOURCE_AUDIO = 3,
}RESOURCE_TYPE;

//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Resource Interface class----------------------------
//===============================================================================
class cResource 
{
private:
protected:
public:
	unsigned int m_ResourceID;
	unsigned int m_Scope;
	std::string m_FileName;
	RESOURCE_TYPE m_Type;

	//To be overloaded by derived classes
	virtual ~cResource(){};
	virtual void load(){};
	virtual void unload(){};
	

	inline cResource()
	{
		m_ResourceID = m_Scope = 0;
		m_Type = RESOURCE_NULL;
	}
};

//--------------------------------------------------------------------------------
//================================================================================
//------------------------Resource Manager class----------------------------------
//================================================================================
class cResourceManager 
{
private:
protected:

	//current resource scope
	unsigned int m_CurrentScope;

	//Total number of resources, both loaded and unloaded
	unsigned int m_ResourceCount;

public:
	
	//Map of form <scope, resource_list>
	//each scope element will contain a list of resource for that scope
	std::map<unsigned int, std::list<cResource*>> m_Resources;

	//Finds resource by ID. will return null if specified resource not found
	cResource* findResourcebyID(unsigned int UID);

	//clear all resource and scopes regardless
	void clear();

	//loads resources from XML file
	bool loadFromXMLFile(std::string Filename);

	//set the current scope depending on scene currently loaded
	void setCurrentScope(unsigned int Scope);

	const unsigned int getResourceCount(){return m_ResourceCount;}

	inline cResourceManager()
	{
		m_CurrentScope = m_ResourceCount = 0;
	}
};

#endif  // RESOURCEMANAGER_H_INCLUDED