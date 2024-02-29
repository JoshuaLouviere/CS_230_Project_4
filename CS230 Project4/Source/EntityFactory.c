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
#include "EntityContainer.h"
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

static EntityContainer* archetypes = NULL;

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
Entity* EntityFactoryBuild(const char* entityName)
{
	if (entityName == NULL) {
		return NULL;
	} 

	char pathName[50] = "";
	sprintf_s(pathName, _countof(pathName), "./Data/%s.txt", entityName);
	// printf("%s\n", pathName);
	FILE* file = StreamOpen(pathName);

	if (archetypes == NULL) {
		archetypes = EntityContainerCreate();
	}

	if (EntityContainerFindByName(archetypes, entityName) == NULL) {
		printf("Success\n");
		if (file)
		{
			const char* token = StreamReadToken(file);

			if (strncmp(token, "Entity", _countof("Entity")) == 0)
			{
				Entity* ent = EntityCreate();
				EntityRead(ent, file);
				EntityContainerAddEntity(archetypes, ent);

				StreamClose(&file);
				return ent;
			}

			StreamClose(&file);
		}
	}
	

	return NULL;
}

// Free all archetype Entities.
// (Hint: If the "archetypes" container exists, then the EntityContainerFreeAll
//    function must be called.)
void EntityFactoryFreeAll() 
{
	//if ()
}
//------------------------------------------------------------------------------
