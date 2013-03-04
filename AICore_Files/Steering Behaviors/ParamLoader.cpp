/************************************************************************
*  Program Name:		ParamLoader.cpp									*
*  Name:				James B. Strickler II							*
*  Date:				January 25, 2013								*
*  Description:			Paramloader class used to parse a 				*
*						parameter file for the steering 				*
*						behavior project								*
*  Update:																*
*																		*
************************************************************************/
#include "ParamLoader.h"

ParamLoader* ParamLoader::Instance()
{
  static ParamLoader instance;

  return &instance;
}