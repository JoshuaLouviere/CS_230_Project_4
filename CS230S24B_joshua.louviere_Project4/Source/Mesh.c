//------------------------------------------------------------------------------
//
// File Name:	Mesh.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S23
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------



#include "stdafx.h"
#include "DGL.h"
#include "Mesh.h"

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Mesh Mesh;

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Mesh
{
	// The name of the Mesh.  This will be used later in the semester.
	char name[32];

	// Pointer to the DGL_Mesh resource.
	const DGL_Mesh* meshResource;

	// The draw mode to use when rendering the mesh (Usually "DGL_DM_TRIANGLELIST").
	DGL_DrawMode drawMode;
} Mesh;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Mesh object and create a quadrilateral mesh.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: The Mesh name can be stored using strcpy_s(). For example:
//    strcpy_s(mesh->name, _countof(mesh->name), name); )
// (Hint: The DGL_Mesh object must be created using DGL_Graphics_StartMesh,
//    DGL_Graphics_AddTriangle, and DGL_Graphics_EndMesh.)
// Params:
//	 xHalfSize = The X half-size of the mesh.
//	 yHalfSize = The Y half-size of the mesh.
//   uSize = The U size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//   vSize = The V size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//	 name = A name for the mesh.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created Mesh,
//	   else return NULL.
Mesh* MeshCreateQuad(float xHalfSize, float yHalfSize, float uSize, float vSize, const char* name)
{ 
	Mesh* mesh = calloc(1, sizeof(Mesh));
	  		
	// Inform the library that we're about to start adding triangles.
	DGL_Graphics_StartMesh();

	// This shape has 2 triangles
	DGL_Graphics_AddTriangle(
		&(DGL_Vec2) { -xHalfSize, -yHalfSize
	}, & (DGL_Color) { 0.0f, 0.0f, 0.0f, 1.0f }, & (DGL_Vec2) { 0.0f, 0.0f },
		& (DGL_Vec2) {
		xHalfSize, yHalfSize
	}, & (DGL_Color) {
		0.0f, 0.0f, 0.0f, 1.0f
	}, & (DGL_Vec2) {
			uSize, -vSize
		},
			& (DGL_Vec2) {
			xHalfSize, -yHalfSize
		}, & (DGL_Color) {
				0.0f, 0.0f, 0.0f, 1.0f
			}, & (DGL_Vec2) {
				uSize, 0.0f
			});
	DGL_Graphics_AddTriangle(
		&(DGL_Vec2) { -xHalfSize, -yHalfSize
	}, & (DGL_Color) { 0.0f, 0.0f, 0.0f, 1.0f }, & (DGL_Vec2) { 0.0f, 0.0f },
		& (DGL_Vec2) {
		-xHalfSize, yHalfSize
	}, & (DGL_Color) {
		0.0f, 0.0f, 0.0f, 1.0f
	}, & (DGL_Vec2) {
			0.0f, -vSize
		},
			& (DGL_Vec2) {
			xHalfSize, yHalfSize
		}, & (DGL_Color) {
				0.0f, 0.0f, 0.0f, 1.0f
			}, & (DGL_Vec2) {
				uSize, -vSize
			});
	  		

	// Save the mesh (as a list of triangles).
	if (mesh)
	{
		mesh->meshResource = DGL_Graphics_EndMesh();
		strcpy_s(mesh->name, _countof(mesh->name), name);
		return mesh;
	}

	return NULL;
}

// Create a "spaceship" mesh.
// (NOTE: This must be a "unit"-sized triangular mesh as described in the Project 2 instructions.)
// (NOTE: The Mesh object must first be made using calloc().)
// (NOTE: The Mesh name can be stored using strcpy_s().)
// (NOTE: The DGL_Mesh object must be created using DGL_Graphics_StartMesh,
//    DGL_Graphics_AddTriangle, and DGL_Graphics_EndMesh.)
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created Mesh,
//	   else return NULL.
Mesh* MeshCreateSpaceship(void)
{
	Mesh* mesh = calloc(1, sizeof(Mesh));

	DGL_Graphics_StartMesh();
	DGL_Graphics_AddTriangle(
		&(DGL_Vec2) { 0.5f, 0.0f }, & (DGL_Color) { 1.0f, 1.0f, 0.0f, 1.0f }, & (DGL_Vec2) { 0.0f, 0.0f },
		& (DGL_Vec2) {
		-0.5f, -0.5f
	}, & (DGL_Color) {
		1.0f, 0.0f, 0.0f, 1.0f
	}, & (DGL_Vec2) {
			0.0f, 0.0f
		},
			& (DGL_Vec2) {
			-0.5f, 0.5f
		}, & (DGL_Color) {
				1.0f, 0.0f, 0.0f, 1.0f
			}, & (DGL_Vec2) {
				0.0f, 0.0f
			});
	if (mesh)
	{
		mesh->meshResource = DGL_Graphics_EndMesh();
		strcpy_s(mesh->name, _countof("spaceship"), "spaceship");
		return mesh;
	}

	return NULL;
}

// Render a mesh.
// (NOTE: This is done using DGL_Graphics_DrawMesh().)
// Params:
//   mesh = Pointer to a Mesh to be rendered.
void MeshRender(const Mesh* mesh)
{
	if (mesh)
	{
		DGL_Graphics_DrawMesh(mesh->meshResource, mesh->drawMode);
	}
}

// Free the memory associated with a mesh.
// (NOTE: The DGL_Mesh object must be freed using DGL_Graphics_FreeMesh().)
// (NOTE: The Mesh object must be freed using free().
// (NOTE: The Mesh pointer must be set to NULL.)
// Params:
//   mesh = Pointer to the Mesh pointer.
void MeshFree(Mesh** mesh)
{
	if (mesh)
	{
		DGL_Graphics_FreeMesh(&(*mesh)->meshResource);
		free(*mesh);
		*mesh = NULL;
	}
}
//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif