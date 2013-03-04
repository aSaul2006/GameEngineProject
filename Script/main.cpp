/*****************************************************************************
 *                                                                           *
 * Program Name:    main.cpp                                                 *
 * Name:            Brian Warner                                             *
 * Date:            February 15, 2013                                        *
 * Description:     Main program for ScriptCore's external interface         *
 * Update:          David Low (2/20/2013)                                    *
 *                  - further testing and code cleanup                       *
 *****************************************************************************/
#include <iostream>
#include <string>

#include "ScriptCore.h"

// help message
void help()
{
	printf( "Please enter a lua script to execute, type 'help' to redisplay this message, or \n" );
	printf( "type 'exit' to terminate.\n\n" );
	printf( "You may enter the file name if it is in the current directory, or type a full\n" );
	printf( "path or a relative file path using '..\\'. \n" );
	printf( "You must add the extension if the script file has one.\n\n");
}

int main()
{
	// local declarations
	std::string sInput;	// user input
	bool bExit = false;	// I/O while loop bool
	int iRC;
	ScriptCore *oSCore = new ScriptCore; // ScriptCore object

//	oSCore->IDE("");		//initialize IDE

//	printf( "The IDE has been initialized. Welcome to ScriptCore!\n\n" );
	help();

	// start user I/O loop
	while (!bExit) {
		printf( "File name or file path: " );
		// get user input
		std::cin >> sInput;
		printf( "\n" );

		if ("exit" == sInput) {
			// exit IDE
			bExit = true;
		}
		else if ("help" == sInput) {
			// print help message
			printf( "\n" );
			help();
		}
		else {
			// run script
			printf( "\n\nScript run attempted.\n" );
			iRC = oSCore->execute( sInput );
			if (iRC != 0) {
				printf( " ... Failure.\n\n" );
			}
			else {
				printf( " ... Success.\n\n" );
			}
		}
	}

	delete oSCore;  // terminate core object

	return( 0 );
}
