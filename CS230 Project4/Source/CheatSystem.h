//------------------------------------------------------------------------------
//
// File Name:	CheatSystem.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S24
//
// Copyright � 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {	// Assume C declarations for C++.
#endif

	//------------------------------------------------------------------------------
	// Forward References:
	//------------------------------------------------------------------------------

	typedef struct BaseSystem BaseSystem;

	//------------------------------------------------------------------------------
	// Public Constants:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Typedefs:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	// An example of the structure to be defined in CheatSystem.c.
#if 0
	typedef struct
	{
		char key;
		const Scene* (*getInstance)(void);

	} CheatSystemKeyBinding;
#endif

	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Get the instance of the Stub System.
	BaseSystem* CheatSystemGetInstance(void);

	/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       // End of extern "C" {
#endif