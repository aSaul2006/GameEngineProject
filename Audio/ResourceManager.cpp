/************************************************************************
*  Program Name:         ResourceManager Core                                               
*  Name:                 Aaron Saul                                                                                        
*  Date:                 January 22nd 2013  
*  Description:          loads data from XML file using tinyXML parser.                                                                                                                                                                    *
*  Update:                                            
*                                                     
************************************************************************/
#define SAFE_DELETE(a) { delete (a); (a) = NULL; }
#include "ResourceManager.h"
#include "AudioManager.h"
#include "../Render/d3dUtil.h"

AudioManager* g_audioManager;
cRenderManager* g_renderManager = new cRenderManager();



//-------------------------------------------------------------------------------------
//=====================================================================================
//--------------------Load Resource from XML File -------------------------------------
//=====================================================================================

bool cResourceManager::loadFromXMLFile(std::string Filename)
{
	TiXmlDocument doc(Filename.c_str());

	if(doc.LoadFile())
	{
		//find topmost resources node in XML file
		TiXmlNode* ResourceTree = doc.FirstChild("resources");

		if(ResourceTree)
		{
			//Enumerate resource objects
			for(TiXmlNode* child = ResourceTree->FirstChild();child; child = child->NextSibling())
			{
				TiXmlElement *Element = child->ToElement();

				if(Element)
				{
					cResource *Resource = NULL;
					cAudioResource* aResource = NULL;

					for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib = ElementAttrib->Next())
					{
						//examine our resource object
						std::string AttribName = ElementAttrib->Name();
						std::string AttribValue = ElementAttrib->Value();

						//detect resource type. Graphic, Texture, Audio
						if(AttribName=="type")
						{
							//Allow managers to implement derived versions of cResource.
							//Managers will create resources and pass a cResource point to be
							//added to resource list.  
							if(AttribValue=="graphic")
							{
								//call the render manager to instantiate an instance of render resource
								Resource = g_renderManager->load3DFromXML(Element);
							}

							if(AttribValue=="texture")
							{
								Resource = AudioManager::GetInstance()->loadTResourceFromXML(Element);
								//Resource = g_TextureManager.loadResourceFromXML(Element);  //CTextureResource
							}

							if(AttribValue=="audio")
							{	
								Resource = AudioManager::GetInstance()->loadResourceFromXML(Element);
							}
						}

						if(Resource)
						{
							if(AttribName=="UID")
							{
								Resource->m_ResourceID = atoi(AttribValue.c_str());
							}

							if(AttribName=="filename")
							{
								Resource->m_FileName = AttribValue;
							}

							if(AttribName=="scenescope")
							{
								Resource->m_Scope = atoi(AttribValue.c_str());
							}
						}
					}

					if(Resource)
					{
						//Resources are added to map here
						m_Resources[Resource->m_Scope].push_back(Resource);
						m_ResourceCount++;
					}
					//if(aResource)
					//{
					//	//Resources are added to map here
					//	m_Resources[aResource->m_Scope].push_back(aResource);
					//	m_ResourceCount++;
					//}
				}
			}
			return true;
		}
	}
	return false;
}


//-----------------------------------------------------------------------------------------
//=========================================================================================
//--Search the cResource collection for resource of a given ID and return pointer to ------
//--this resource if found, if not return a null pointer-----------------------------------
//=========================================================================================

cResource* cResourceManager::findResourcebyID(unsigned int UID)
{
	std::map<unsigned int, std::list<cResource*>>::iterator it;

	//search through scopes
	for(it=m_Resources.begin(); it != m_Resources.end(); it++)
	{
		if(!(*it).second.empty())
		{
			std::list<cResource*>::iterator list_it;

			//Search through resources of scope
			for(list_it=(*it).second.begin();list_it != (*it).second.end(); list_it++)
			{
				//if matches ID
				if((*list_it)->m_ResourceID == UID)
					return (*list_it);
			}
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------
//=========================================================================================
//----Clear method removes all cResources from collection maintained by resource manager --
//=========================================================================================

void cResourceManager::clear()
{
	std::map<unsigned int, std::list<cResource*>>::iterator it;

	//Search through scopes
	for(it=m_Resources.begin();it != m_Resources.end();it++)
	{
		if(!(*it).second.empty())
		{
			std::list<cResource*>::iterator list_it;

			//Search through resources of the scope
			for(list_it=(*it).second.begin();list_it != (*it).second.end();list_it++)
			{
				//delete resource object
				(*list_it)->unload();
				SAFE_DELETE(*list_it);
			}

			(*it).second.clear();
		}

		m_Resources.clear();
	}
}


//---------------------------------------------------------------------------------------------
//=============================================================================================
//----set current scope should be called for every scene change in the game.  it accepts ------
//----one parameter indicating the unique ID for the new scene which resource are to be loaded-
//=============================================================================================

void cResourceManager::setCurrentScope(unsigned int Scope)
{
	//unload old scope, if not global scope in this case global scope is Zero
	if(m_CurrentScope != 0)
	{
		std::list<cResource*>::iterator list_it;

		for(list_it = m_Resources[m_CurrentScope].begin();list_it != m_Resources[m_CurrentScope].end(); list_it++)
		{
			(*list_it)->unload();
		}
	}

	m_CurrentScope = Scope;

	//Load new scope
	std::list<cResource*>::iterator list_it;

	for(list_it = m_Resources[m_CurrentScope].begin(); list_it != m_Resources[m_CurrentScope].end(); list_it++)
	{
		(*list_it)->load();
	}

}

