//------------------------------------------------------------------------------
//
// File Name:	SandboxScene.c
// Author(s):	Joshua Louviere (joshua.louviere)
// Project:		Project 0
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "StubScene.h"
#include "Trace.h"
#include "Stream.h"
#include "DGL.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SandboxScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.

} SandboxScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void SandboxSceneLoad(void);
static void SandboxSceneInit(void);
static void SandboxSceneUpdate(float dt);
static void SandboxSceneExit(void);
static void SandboxSceneUnload(void);
static void SandboxSceneRender(void);
static void SandboxSceneTraceFloat(const char* text, float f);
static void SandboxSceneTraceVector(const char* text, const DGL_Vec2* v);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static SandboxScene instance =
{
	// Initialize the base structure:
	{ "Sandbox", SandboxSceneLoad, SandboxSceneInit, SandboxSceneUpdate, SandboxSceneRender, SandboxSceneExit, SandboxSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* SandboxSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void SandboxSceneTraceFloat(const char* text, float f)
{
	TraceMessage("Vector Test: %s = %f", text, f);
}

static void SandboxSceneTraceVector(const char* text, const DGL_Vec2* v)
{
	TraceMessage("Vector Test: %s = [%f, %f]",text,v->x,v->y);
}

// Load any resources used by the scene.
static void SandboxSceneLoad(void)
{
}

// Initialize the variables used by the scene.
static void SandboxSceneInit()
{
}

// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void SandboxSceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	// NOTE: This call causes the engine to exit immediately.  Make sure to remove
	//   it when you are ready to test out a new scene.
	FILE* file;
	file = StreamOpen("Data/VectorTests.txt");

	if (file != NULL)
	{
		DGL_Vec2* vec = malloc(sizeof(DGL_Vec2));
		Vector2DZero(vec);
		SandboxSceneTraceVector("Vector2DZero", vec);

		Vector2DSet(vec, 1.0f, 1.5f);
		SandboxSceneTraceVector("Vector2DSet", vec);

		Vector2DNeg(vec, vec);
		SandboxSceneTraceVector("Vector2DNeg", vec);

		Vector2DAdd(vec, vec, vec);
		SandboxSceneTraceVector("Vector2DAdd", vec);

		Vector2DSub(vec, vec, vec);
		SandboxSceneTraceVector("Vector2DSub", vec);

		StreamReadVector2D(file, vec);
		SandboxSceneTraceVector("StreamReadVector2D", vec);

		Vector2DNormalize(vec, vec);
		SandboxSceneTraceVector("Vector2DNormalize", vec);

		float scale = StreamReadFloat(file);
		SandboxSceneTraceFloat("StreamReadFloat", scale);

		Vector2DScale(vec, vec, scale);
		SandboxSceneTraceVector("Vector2DScale", vec);

		Vector2DScaleAdd(vec, vec, vec, scale);
		SandboxSceneTraceVector("Vector2DScaleAdd", vec);

		Vector2DScaleSub(vec, vec, vec, scale);
		SandboxSceneTraceVector("Vector2DScaleSub", vec);

		float length = Vector2DLength(vec);
		SandboxSceneTraceFloat("Vector2DLength", length);

		length = Vector2DSquareLength(vec);
		SandboxSceneTraceFloat("Vector2DSquareLength", length);

		DGL_Vec2* vec1 = malloc(sizeof(DGL_Vec2));
		DGL_Vec2* vec2 = malloc(sizeof(DGL_Vec2));

		StreamReadVector2D(file, vec1);
		SandboxSceneTraceVector("StreamReadVector2D", vec1);

		StreamReadVector2D(file, vec2);
		SandboxSceneTraceVector("StreamReadVector2D", vec2);

		float Temp = Vector2DDistance(vec1, vec2);
		SandboxSceneTraceFloat("Vector2DDistance", Temp);

		Temp = Vector2DSquareDistance(vec1, vec2);
		SandboxSceneTraceFloat("Vector2DSquareDistance", Temp);

		Temp = Vector2DDotProduct(vec1, vec2);
		SandboxSceneTraceFloat("Vector2DDotProduct", Temp);
		
		DGL_Vec2* vec3 = malloc(sizeof(DGL_Vec2));

		float angle = StreamReadFloat(file);
		SandboxSceneTraceFloat("StreamReadFloat", angle);

		Vector2DFromAngleDeg(vec, angle);
		SandboxSceneTraceVector("Vector2DFromAngleDeg", vec);

		angle = StreamReadFloat(file);
		SandboxSceneTraceFloat("StreamReadFloat", angle);

		Vector2DFromAngleRad(vec, angle);
		SandboxSceneTraceVector("Vector2DFromAngleRad", vec);

		angle = Vector2DToAngleRad(vec);
		SandboxSceneTraceFloat("Vector2DToAngleRad", angle);

		StreamClose(&file);

		free(vec);
		free(vec1);
		free(vec2);
		free(vec3);
	}
	
	SceneSystemSetNext(NULL);
	
}

// Render the scene.
void SandboxSceneRender(void)
{
}

// Exit the scene.
static void SandboxSceneExit()
{
}

// Unload any resources used by the scene.
static void SandboxSceneUnload(void)
{
}

