/*
	WinMain() is the entry point of a Windows program
	(instead of the familiar main() in a console program)
*/

// Includes
//=========

#include <Engine/Logging/Logging.h>
#include "cMyFinalGame.h"

// Entry Point
//============

int WINAPI WinMain( HINSTANCE i_thisInstanceOfTheApplication, HINSTANCE, char* i_commandLineArguments, int i_initialWindowDisplayState )
{
	eae6320::Logging::OutputMessage("MyFinalGame project has started.");
	return eae6320::Application::Run<eae6320::MyFinalGame>( i_thisInstanceOfTheApplication, i_commandLineArguments, i_initialWindowDisplayState );
}