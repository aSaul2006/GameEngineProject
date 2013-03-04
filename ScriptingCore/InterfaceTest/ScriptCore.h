/*****************************************************************************
 *                                                                           *
 * Program Name:    ScriptCore.h                                             *
 * Name:            David Low                                                *
 * Date:            February 7, 2013                                         *
 * Description:     Include for defining the External interfaces for         *
 *                  Scripting Core                                           *
 * Update:          Brian Warner (2/15/2013)                                 *
 *                                                                           *
 *****************************************************************************/
#ifndef __SCRIPTCORE_H__
#define __SCRIPTCORE_H__

#include <string>
#include <lua.hpp>

class
ScriptCore
{
private:
	// check if given file exists
	bool bFexists( const char *filename );

public:
	int execute( std::string sName );

	void IDE( );
	void IDE( std::string sName );
};

#endif // __SCRIPTCORE_H__
