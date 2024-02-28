//------------------------------------------------------------------------------
//
// File Name:	EntityFactory.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DGL.h"
#include "EntityFactory.h"
#include "Stream.h"
#include "Entity.h"
//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified game object.
// Params:
//	 filename = The name of the file to be deserialized.
// Returns:
//	 If the filename is valid
//	   then return a pointer to a new instance of the specified game object,
//	   else NULL.
Entity* EntityFactoryBuild(const char* filename)
{
	FILE* file = StreamOpen(filename);

	if (file)
	{
		const char* token = StreamReadToken(file);

		if (strncmp(token, "Entity", _countof("Entity")) == 0)
		{
			Entity* ent = EntityCreate();
			EntityRead(ent, file);
			StreamClose(&file);
			return ent;
		}

		StreamClose(&file);
	}

	return NULL;
}

//------------------------------------------------------------------------------
