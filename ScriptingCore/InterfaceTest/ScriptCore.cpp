/*****************************************************************************
 *                                                                           *
 * Program Name:    ScriptCore.cpp                                           *
 * Name:            David Low                                                *
 * Date:            February 7, 2013                                         *
 * Description:     External interfaces for Scripting Core                   *
 * Update:          Brian Warner (2/15/2013)                                 *
 *                  - debugging, add private funcction bFexists              *
 *                  David Low (2/20/2013)                                    *
 *                  - conform to a coding standard                           *
 *                  - reorganize based on integration model of engine        *
 *****************************************************************************/

#include <string>
#include <fstream>

#if defined(_WIN32)
#include <windows.h>
#include <WinBase.h>
#endif

#include "ScriptCore.h"

// some handy typedefs
typedef lua_State* voidfunc ();
typedef int *varfunc (lua_State *L,...);					// quick and dirty using the varargs
typedef void varfuncvoid (lua_State *L,...);

// some variables to use to setup the IDE
namespace LibFuncs{
	static voidfunc *luaL_newstate;
	static varfunc *luaL_loadbuffer;
	static varfunc *luaL_loadbufferx;
	static varfunc *luaL_openlibs;
	static varfunc *lua_pcall;
	static varfunc *lua_pcallk;
	static varfunc *lua_pushcclosure;
	static varfunc *lua_setfield;
	static varfunc *lua_tolstring;
	static varfunc *luaL_loadfilex;
	static varfuncvoid *lua_createtable;
	static varfuncvoid *lua_pushstring;
	static varfuncvoid *lua_rawseti;
	static varfuncvoid *lua_getglobal;
	static varfuncvoid *lua_close;
	static varfunc *luaL_dofile;
	static varfunc *luaL_loadfile;
	static varfunc *luaL_loadstring;
}

static int
luafunc_mbox( lua_State *L )
{
	const char *title = (const char*)LibFuncs::lua_tolstring( L, 1, NULL );
	const char *msg = (const char*)LibFuncs::lua_tolstring( L, 2, NULL );

	MessageBox( NULL, msg, title, MB_OK | MB_ICONERROR );

	return ( 0 );
}

static const char *luacode =
	"local msg = _ERRMSG; _ERRMSG = nil "
	"local arg = _ARG or {}; _ARG = nil "
	"xpcall("
	"function() "
	"(loadfile 'src/main.lua')(unpack(arg)) end,"
	"function(err) msg('Uncaught lua script exception',debug.traceback(err)) end)";

// check if a given file exists
bool
ScriptCore::bFexists( const char *filename )
{
	std::ifstream ifile( filename );
	return( (NULL == ifile) ? false : true );
}

// start IDE
void
ScriptCore::IDE( )
{
	ScriptCore::IDE( "" );
}

void
ScriptCore::IDE( std::string sName )
{
	char *pcName = new char[sName.size() + 1];

	pcName[sName.size()] = '\0';
	memcpy( pcName, sName.c_str(), sName.size() );

	HINSTANCE tLib;

#if defined(_WIN32)
	tLib = LoadLibrary( "LUA-library.dll" );
#endif

	if ( NULL != tLib ) {
	    LibFuncs::luaL_newstate    = (voidfunc*)GetProcAddress( tLib, "luaL_newstate" );
		LibFuncs::luaL_loadbuffer  = (varfunc*)GetProcAddress( tLib, "luaL_loadbuffer" );
		LibFuncs::luaL_loadbufferx = (varfunc*)GetProcAddress( tLib, "luaL_loadbufferx" );
		LibFuncs::luaL_openlibs    = (varfunc*)GetProcAddress( tLib, "luaL_openlibs" );
		LibFuncs::lua_pcall        = (varfunc*)GetProcAddress( tLib, "lua_pcall" );
		LibFuncs::lua_pcallk       = (varfunc*)GetProcAddress( tLib, "lua_pcallk" );
		LibFuncs::lua_tolstring    = (varfunc*)GetProcAddress( tLib, "lua_tolstring" );
		LibFuncs::lua_setfield     = (varfunc*)GetProcAddress( tLib, "lua_setfield" );
		LibFuncs::lua_pushcclosure = (varfunc*)GetProcAddress( tLib, "lua_pushcclosure" );
		LibFuncs::lua_createtable  = (varfuncvoid*)GetProcAddress( tLib, "lua_createtable" );
		LibFuncs::lua_pushstring   = (varfuncvoid*)GetProcAddress( tLib, "lua_pushstring" );
		LibFuncs::lua_rawseti      = (varfuncvoid*)GetProcAddress( tLib, "lua_rawseti" );
		LibFuncs::lua_getglobal    = (varfuncvoid*)GetProcAddress( tLib, "lua_getglobal" );
		LibFuncs::lua_close        = (varfuncvoid*)GetProcAddress( tLib, "lua_close" );
		LibFuncs::luaL_loadfilex   = (varfunc*)GetProcAddress( tLib, "luaL_loadfilex" );
		LibFuncs::luaL_dofile      = (varfunc*)GetProcAddress( tLib, "luaL_dofile" );
		LibFuncs::luaL_loadstring  = (varfunc*)GetProcAddress( tLib, "luaL_loadstring" );

	    // If the function address is valid, call the function.
	    if ( LibFuncs::luaL_newstate && LibFuncs::luaL_loadbufferx && LibFuncs::luaL_openlibs
			&& LibFuncs::lua_pcallk	&& LibFuncs::lua_pushcclosure && LibFuncs::lua_setfield
			&& LibFuncs::lua_tolstring && LibFuncs::lua_createtable && LibFuncs::lua_pushstring
			&& LibFuncs::lua_rawseti ) {

			// start LUA
			lua_State *L = LibFuncs::luaL_newstate();

			if ( NULL != L ) {
				LibFuncs::lua_createtable( L, 1, 0 );
		        LibFuncs::lua_pushstring( L, sName.c_str() );
		        LibFuncs::lua_rawseti( L, -2, 1 );

				LibFuncs::lua_getglobal( L,"_ARG" );
		        LibFuncs::luaL_openlibs( L );
				LibFuncs::lua_pushcclosure( L, luafunc_mbox, 0 );

				LibFuncs::lua_getglobal( L, "_ERRMSG" );
				// open IDE
				if (0 == LibFuncs::luaL_loadbuffer( L, luacode, strlen( luacode ), "Initializer" ) ) {
					LibFuncs::lua_pcall( L, 0, 0, 0 );
				}
				else {
					MessageBox( NULL,
						TEXT( "An unexpected error occured while loading the lua chunk." ),
						TEXT( "Failed to start IDE" ),
						MB_OK | MB_ICONERROR );
				}
			}
			else {
				MessageBox( NULL,
					TEXT( "Couldn't initialize a luastate" ),
					TEXT( "Failed to start IDE" ),
					MB_OK | MB_ICONERROR );
			}
			LibFuncs::lua_close(L);
		}
		else {
			MessageBox( NULL,
				TEXT( "Could not load all functions that are supposed to be located in the LUA-library.dll" ),
				TEXT( "Failed to start IDE" ),
				MB_OK | MB_ICONERROR );
		}

	    // Free the DLL module.
		FreeLibrary( tLib );
	}
	else {
		MessageBox( NULL,
			TEXT( "The LUA-library.dll could not be found or loaded, please check the working directory of the application."),
			TEXT( "Failed to initialize IDE"),
			MB_OK | MB_ICONERROR );
	}

	return;
}

// execute LUA script
int
ScriptCore::execute( std::string sName )
{
	int iRC = -1;				// script return code

	// initialize lua library
	HINSTANCE tLib;

#if defined(_WIN32)
	tLib = LoadLibrary( "LUA-library.dll" );
#endif

	if ( NULL != tLib ) {
	    LibFuncs::luaL_newstate    = (voidfunc*)GetProcAddress( tLib, "luaL_newstate" );
		LibFuncs::luaL_loadbuffer  = (varfunc*)GetProcAddress( tLib, "luaL_loadbuffer" );
		LibFuncs::luaL_loadbufferx = (varfunc*)GetProcAddress( tLib, "luaL_loadbufferx" );
		LibFuncs::luaL_openlibs    = (varfunc*)GetProcAddress( tLib, "luaL_openlibs" );
		LibFuncs::lua_pcall        = (varfunc*)GetProcAddress( tLib, "lua_pcall" );
		LibFuncs::lua_pcallk       = (varfunc*)GetProcAddress( tLib, "lua_pcallk" );
		LibFuncs::lua_tolstring    = (varfunc*)GetProcAddress( tLib, "lua_tolstring" );
		LibFuncs::lua_setfield     = (varfunc*)GetProcAddress( tLib, "lua_setfield" );
		LibFuncs::lua_pushcclosure = (varfunc*)GetProcAddress( tLib, "lua_pushcclosure" );
		LibFuncs::lua_createtable  = (varfuncvoid*)GetProcAddress( tLib, "lua_createtable" );
		LibFuncs::lua_pushstring   = (varfuncvoid*)GetProcAddress( tLib, "lua_pushstring" );
		LibFuncs::lua_rawseti      = (varfuncvoid*)GetProcAddress( tLib, "lua_rawseti" );
		LibFuncs::lua_getglobal    = (varfuncvoid*)GetProcAddress( tLib, "lua_getglobal" );
		LibFuncs::lua_close        = (varfuncvoid*)GetProcAddress( tLib, "lua_close" );
		LibFuncs::luaL_loadfilex   = (varfunc*)GetProcAddress( tLib, "luaL_loadfilex" );
		LibFuncs::luaL_dofile      = (varfunc*)GetProcAddress( tLib, "luaL_dofile" );
		LibFuncs::luaL_loadstring  = (varfunc*)GetProcAddress( tLib, "luaL_loadstring" );

	    // If the function address is valid, call the function.
	    if ( LibFuncs::luaL_newstate && LibFuncs::luaL_loadbufferx && LibFuncs::luaL_openlibs
			&& LibFuncs::lua_pcallk	&& LibFuncs::lua_pushcclosure && LibFuncs::lua_setfield
			&& LibFuncs::lua_tolstring && LibFuncs::lua_createtable && LibFuncs::lua_pushstring
			&& LibFuncs::lua_rawseti ) {

			// start LUA
			lua_State *L = LibFuncs::luaL_newstate();

			// open lua library
			LibFuncs::luaL_openlibs( L );

			// verify file exists before attempting to execute
			if (bFexists( sName.c_str() )) {
				iRC = luaL_dofile( L, sName.c_str() );

				// when execute error, print error message
				if (0 != iRC) {
					printf( "ERROR: File found, but script could not run\n" );
				}
			}
			else {
				// file does not exist
				printf( "ERROR: File not found\n" );
			}

			// close lua library
			LibFuncs::lua_close( L );
		}
	    // Free the DLL module.
		FreeLibrary( tLib );
	}
	else {
		MessageBox( NULL,
			TEXT( "The LUA-library.dll could not be found or loaded, please check the working directory of the application."),
			TEXT( "Failed to initialize IDE"),
			MB_OK | MB_ICONERROR );
	}

	return( iRC );
}
