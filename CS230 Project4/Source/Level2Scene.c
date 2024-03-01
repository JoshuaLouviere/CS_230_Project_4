//------------------------------------------------------------------------------
//
// File Name:	Level2Scene.c
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
#include "Level1Scene.h"
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

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------


typedef struct Level2Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	Mesh* mesh;

	Entity* entity;


} Level2Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float spaceshipSpeed = 500.0f;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level2SceneLoad(void);
static void Level2SceneInit(void);
static void Level2SceneUpdate(float dt);
static void Level2SceneExit(void);
static void Level2SceneUnload(void);
static void Level2SceneRender(void);
static void Level2SceneMovementController(Entity* entity);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level2Scene instance =
{
	// Initialize the base structure:
	{ "Level2", Level2SceneLoad, Level2SceneInit, Level2SceneUpdate, Level2SceneRender, Level2SceneExit, Level2SceneUnload },

	// Initialize any scene-specific variables:
	
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* Level2SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void Level2SceneLoad(void)
{
	instance.mesh = MeshCreate();
	MeshBuildSpaceship(instance.mesh);
}

// Initialize the variables used by the scene.
static void Level2SceneInit()
{
	instance.entity = EntityFactoryBuild("SpaceshipHoming");
	EntityFactoryBuild("SpaceshipHoming");

	if (instance.entity != NULL)
	{
		Sprite* sprite = EntityGetSprite(instance.entity);
		SpriteSetMesh(sprite, instance.mesh);
		//EntityAddSprite(instance.entity, sprite);
	}

	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 1.0f });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level2SceneUpdate(float dt)
{

	// NOTE: This call causes the engine to exit immediately.  Make sure to remove
	//   it when you are ready to test out a new scene.
	Level2SceneMovementController(instance.entity);
	EntityUpdate(instance.entity, dt);
	
	Sprite* sprite = EntityGetSprite(instance.entity);
	if (DGL_Input_KeyTriggered('Z'))
	{
		SpriteSetAlpha(sprite, 0.5f);
	}
	else if (DGL_Input_KeyTriggered('X'))
	{
		SpriteSetAlpha(sprite, 1.0f);
	}
	else if (DGL_Input_KeyTriggered('0'))
	{
		SceneSystemSetNext(DemoSceneGetInstance());
	}
	else if (DGL_Input_KeyTriggered('1'))
	{
		SceneSystemSetNext(Level1SceneGetInstance());
	}
	else if (DGL_Input_KeyTriggered('2'))
	{
		SceneRestart();
	}
	else if (DGL_Input_KeyTriggered('9'))
	{
		SceneSystemSetNext(SandboxSceneGetInstance());
	}

		
}

// Render the scene.
void Level2SceneRender(void)
{
	EntityRender(instance.entity);
}

// Exit the scene.
static void Level2SceneExit()
{
	EntityFree(&instance.entity);
}

// Unload any resources used by the scene.
static void Level2SceneUnload(void)
{
	MeshFree(&instance.mesh);
}

static void Level2SceneMovementController(Entity* entity)
{
	Physics* physics = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	if (physics && transform)
	{
		Vector2D* direction = calloc(1, sizeof(Vector2D));
		DGL_Vec2 mousePosition = DGL_Input_GetMousePosition();
		mousePosition = DGL_Camera_ScreenCoordToWorld(&mousePosition);
		Vector2D* translation = (Vector2D*)TransformGetTranslation(transform);
		Vector2DSub(direction, &mousePosition, translation);
		Vector2DNormalize(direction, direction);
		TransformSetRotation(transform, Vector2DToAngleRad(direction));
		Vector2DScale(direction, direction, spaceshipSpeed);
		PhysicsSetVelocity(physics, direction);
	}
}