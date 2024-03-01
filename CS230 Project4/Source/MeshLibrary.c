//------------------------------------------------------------------------------
//
// File Name:	MeshLibrary.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S24
//
// Copyright © 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "MeshLibrary.h"
#include "Mesh.h"
#include "Stream.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Mesh Mesh;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct MeshLibrary
{
	// This variable is not required but could be used for tracking the number
	//   of Mesh objects currently in the list.
	unsigned int meshCount;

	// A list of all currently loaded meshes.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	const Mesh* meshList[50];

} MeshLibrary;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static MeshLibrary meshes;

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void MeshLibraryAdd(Mesh* mesh) {
	if (mesh) {
		meshes.meshList[meshes.meshCount] = mesh;
		meshes.meshCount++;
	}
}

// Initialize the Mesh Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void MeshLibraryInit()
{
	memset(&meshes, 0, sizeof(meshes));
}

// Create a mesh and add it to the mesh manager.
// 1: Use sprintf_s() to construct a path name using meshName
//	   (HINT: The correct path name should be constructed using "Data/&s.txt".)
// 2: Call StreamOpen(), passing the pathname
// 3: If the stream was opened successfully,
//	  a: Call MeshCreate() to create an empty Mesh object.
//    b: Call MeshRead() to construct a mesh using data read from the file
//	  c: Call MeshLibraryAdd(), passing the created mesh
//	  d: Close the stream
//	  e: Return the created mesh
// Params:
//	 meshName = The name of the mesh to be created.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
const Mesh* MeshLibraryBuild(const char* meshName)
{
	char pathName[50];
	sprintf_s(pathName, _countof(pathName), "Data/%s.txt", meshName);
	Stream file = StreamOpen(pathName);
	
	if (file) {
		Mesh* mesh = MeshCreate();
		MeshRead(mesh, file);
		MeshLibraryAdd(mesh);
		StreamClose(&file);
		return mesh;
	}

	return NULL;
}

// Free all Mesh objects in the Mesh Manager.
// (NOTE: You must call MeshFree() to free each Mesh object.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void MeshLibraryFreeAll()
{
	int count = meshes.meshCount;
	for (int i = 0; i < count; i++) {
		MeshFree(&meshes.meshList[i]);
	}
}

//------------------------------------------------------------------------------
