//------------------------------------------------------------------------------
//
// File Name:	AsteroidsScene.c
// Author(s):	Joshua Louviere (joshua.louviere)
// Project:		Project 0
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stream.h"
#include "Scene.h"
#include "Trace.h"
#include "SceneSystem.h"
#include "AsteroidsScene.h"
#include "Level2Scene.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Entity.h"
#include "Vector2D.h"
#include "EntityFactory.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "DGL.h"
#include "DemoScene.h"
#include "SandboxScene.h"
#include "Matrix2D.h"
#include "Animation.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------ 

typedef struct AsteroidsScene
{
	// WARNING: The base class must always be included first.
	Scene base;

	// Add any scene-specific variables second.

} AsteroidsScene;

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

static void AsteroidsSceneLoad(void);
static void AsteroidsSceneInit(void);
static void AsteroidsSceneUpdate(float dt);
static void AsteroidsSceneUnload(void);
static void AsteroidsSceneRender(void);
static void AsteroidsSceneExit(void);


//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static AsteroidsScene instance =
{
	// Initialize the base structure:
	{ "Asteroids", AsteroidsSceneLoad, AsteroidsSceneInit, AsteroidsSceneUpdate, AsteroidsSceneRender, AsteroidsSceneExit, AsteroidsSceneUnload },

	// Initialize any scene-specific variables:

};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* AsteroidsSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void AsteroidsSceneLoad(void)
{
	
	//printf("123\n
}

// Initialize the variables used by the scene.
static void AsteroidsSceneInit()
{
	// Background / Blend mode
	float col = 0.0f;
	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { col, col, col, 1.0f });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}


// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void AsteroidsSceneUpdate(float dt)
{
	dt = dt;
}

// Render the scene.
void AsteroidsSceneRender(void)
{
	
}

// Exit the scene.
static void AsteroidsSceneExit()
{
	
}

// Unload any resources used by the scene.
static void AsteroidsSceneUnload(void)
{
}
