//------------------------------------------------------------------------------
//
// File Name:	Level1Scene.c
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
#include "Matrix2D.h"
#include "Animation.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------ 
enum MonkeyStates monkeyState = MonkeyInvalid;

typedef struct Level1Scene
{
	// WARNING: The base class must always be included first.
	Scene base;

	// Add any scene-specific variables second.
	int numLives;

	Mesh* mesh;
	Mesh* monkeyMesh;
	Mesh* livesTextMesh;

	SpriteSource* spriteSource;
	SpriteSource* monkeyIdleSpriteSource;
	SpriteSource* monkeyJumpSpriteSource;
	SpriteSource* monkeyWalkSpriteSource;
	SpriteSource* livesTextSpriteSource;

	Entity* entity;
	Entity* monkey;
	Entity* livesText;

} Level1Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float wallDistance = 462.f;
static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const float CheckSquareDistance = (75.0f * 75.0f);
char livesBuffer[16];

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level1SceneLoad(void);
static void Level1SceneInit(void);
static void Level1SceneUpdate(float dt);
static void Level1SceneExit(void);
static void Level1SceneUnload(void);
static void Level1SceneRender(void);
static void Level1SceneMovementController(Entity* entity);
static void Level1SceneSetMonkeyState(Entity* gameObject, MonkeyStates newState);
static void Level1SceneBounceController(Entity* entity);
static bool Level1SceneIsColliding(const Entity* entityA, const Entity* entityB);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level1Scene instance =
{
	// Initialize the base structure:
	{ "Level1", Level1SceneLoad, Level1SceneInit, Level1SceneUpdate, Level1SceneRender, Level1SceneExit, Level1SceneUnload },

	// Initialize any scene-specific variables:

};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* Level1SceneGetInstance(void)
{
	instance.numLives = 0;
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void Level1SceneLoad(void)
{
	//TraceMessage("Level1SceneLoad");

	FILE* fp = StreamOpen("./Data/Level1_Lives.txt");
	if (fp != NULL)
	{
		instance.numLives = StreamReadInt(fp);
		StreamClose(&fp);
	}
	//printf("123\n
}

static void Level1SceneSetMonkeyState(Entity* gameObject, MonkeyStates newState)
{
	if (monkeyState != newState)
	{
		monkeyState = newState;
		Sprite *sprite = EntityGetSprite(gameObject);
		Animation *anim = EntityGetAnimation(gameObject);
		
		switch(newState)
		{
			case MonkeyIdle:
				SpriteSetMesh(sprite, instance.mesh);
				SpriteSetSpriteSource(sprite, instance.monkeyIdleSpriteSource);
				AnimationPlay(anim, 1, 0.f, false);
				break;
			case MonkeyWalk:
				SpriteSetMesh(sprite, instance.monkeyMesh);
				SpriteSetSpriteSource(sprite, instance.monkeyWalkSpriteSource);
				AnimationPlay(anim, 8, 0.05f, true);
				break;
			case MonkeyJump:
				SpriteSetMesh(sprite, instance.mesh);
				SpriteSetSpriteSource(sprite, instance.monkeyJumpSpriteSource);
				AnimationPlay(anim, 1, 0.f, false);
				break;
		}
	}
}

// Initialize the variables used by the scene.
static void Level1SceneInit()
{
	//printf("456\n");

	instance.monkeyMesh = MeshCreateQuad(0.5, 0.5, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	instance.livesTextMesh = MeshCreateQuad(0.5, 0.5, 1.0f / 16, 1.0f / 8, "Mesh16x8");
	instance.mesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
	instance.spriteSource = SpriteSourceCreate();
	instance.monkeyIdleSpriteSource = SpriteSourceCreate();
	instance.monkeyJumpSpriteSource = SpriteSourceCreate();
	instance.monkeyWalkSpriteSource = SpriteSourceCreate();
	instance.livesTextSpriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spriteSource, 1, 1, "./Assets/PlanetTexture.png");
	SpriteSourceLoadTexture(instance.monkeyIdleSpriteSource, 1, 1, "./Assets/MonkeyIdle.png");
	SpriteSourceLoadTexture(instance.monkeyWalkSpriteSource, 3, 3, "./Assets/MonkeyWalk.png");
	SpriteSourceLoadTexture(instance.monkeyJumpSpriteSource, 1, 1, "./Assets/MonkeyJump.png");
	SpriteSourceLoadTexture(instance.livesTextSpriteSource, 16, 8, "./Assets/Roboto_Mono_black.png");
	instance.entity = EntityFactoryBuild("./Data/PlanetBounce.txt");
	instance.monkey = EntityFactoryBuild("./Data/Monkey.txt");
	instance.livesText = EntityFactoryBuild("./Data/MonkeyLivesText.txt");

	if (instance.monkey)
	{
		monkeyState = MonkeyInvalid;
		Level1SceneSetMonkeyState(instance.monkey, MonkeyIdle);
	}

	if (instance.livesText)
	{
		Sprite* sprite = EntityGetSprite(instance.livesText);
		SpriteSetMesh(sprite, instance.livesTextMesh);
		SpriteSetSpriteSource(sprite, instance.livesTextSpriteSource);
		sprintf_s(livesBuffer, _countof(livesBuffer), "Lives: %d", instance.numLives);

		SpriteSetText(sprite, livesBuffer);
	}
	//TraceMessage("Level1SceneInit");

	if (instance.entity)
	{
		Sprite* sprite = EntityGetSprite(instance.entity);
		SpriteSetMesh(sprite, instance.mesh);
		SpriteSetFrame(sprite, 0);
		SpriteSetSpriteSource(sprite, instance.spriteSource);
		//EntityAddSprite(instance.entity, sprite);
	}

	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 1.0f, 1.0f, 1.0f, 1.0f });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}


// Update the the variables used by the scene and render objects (temporary).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level1SceneUpdate(float dt)
{

	// NOTE: This call causes the engine to exit immediately.  Make sure to remove
	//   it when you are ready to test out a new scene.

	EntityUpdate(instance.monkey, dt);
	EntityUpdate(instance.entity, dt);
	EntityUpdate(instance.livesText, dt);

	Level1SceneMovementController(instance.monkey);
	Level1SceneBounceController(instance.entity);

	if (Level1SceneIsColliding(instance.monkey, instance.entity))
	{
		instance.numLives--;
		if (instance.numLives <= 0)
		{
			SceneSystemSetNext(Level2SceneGetInstance());
		}
		else
		{
			SceneRestart();
		}
	}

	

	if (DGL_Input_KeyTriggered('0'))
	{
		SceneSystemSetNext(DemoSceneGetInstance());
	}
	else if (DGL_Input_KeyTriggered('1'))
	{
		SceneRestart();
	}
	else if (DGL_Input_KeyTriggered('2'))
	{
		SceneSystemSetNext(Level2SceneGetInstance());
	}
	else if (DGL_Input_KeyTriggered('9'))
	{
		SceneSystemSetNext(SandboxSceneGetInstance());
	}
		
}

static bool Level1SceneIsColliding(const Entity* entityA, const Entity* entityB)
{
	Vector2D* posA = (Vector2D*)TransformGetTranslation(EntityGetTransform(entityA));
	Vector2D* posB = (Vector2D*)TransformGetTranslation(EntityGetTransform(entityB));

	if (Vector2DSquareDistance(posA, posB) < CheckSquareDistance)
	{
		return true;
	}

	return false;
}

static void Level1SceneBounceController(Entity* entity)
{
	Physics* phys = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	if (phys && transform)
	{
		Vector2D* vector = (Vector2D*)PhysicsGetVelocity(phys);
		Vector2D* pos = (Vector2D*)TransformGetTranslation(transform);
		if (pos->x <= -wallDistance)
		{
			pos->x = -wallDistance;
			vector->x *= -1;
		}
		if (pos->x >= wallDistance)
		{
			pos->x = wallDistance;
			vector->x *= -1;
		}
		if (pos->y <= groundHeight)
		{
			pos->y = groundHeight + (groundHeight - pos->y);
			vector->y *= -1;
		}
	}
}

static void Level1SceneMovementController(Entity* entity)
{
	Physics* phys = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	if (phys && transform)
	{
		Vector2D* vector = (Vector2D *)PhysicsGetVelocity(phys);
	
		if (DGL_Input_KeyDown(VK_LEFT))
		{
			vector->x = -moveVelocity;
			if (monkeyState != MonkeyJump)
			{
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else if (DGL_Input_KeyDown(VK_RIGHT))
		{
			vector->x = moveVelocity;
			if (monkeyState != MonkeyJump)
			{
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else
		{
			vector->x = 0;

			if (monkeyState != MonkeyJump)
			{
				Level1SceneSetMonkeyState(entity, MonkeyIdle);
			}
		}

		//printf("%i\n", monkeyState);
		if (DGL_Input_KeyTriggered(VK_UP))
		{
			vector->y = jumpVelocity;
			PhysicsSetAcceleration(phys, &gravityNormal);
			Level1SceneSetMonkeyState(entity, MonkeyJump);
		}
		
		if (TransformGetTranslation(transform)->y < groundHeight)
		{
			vector->y = 0;
			Vector2D* translation = (Vector2D *)TransformGetTranslation(EntityGetTransform(entity));
		
			translation->y = groundHeight;
			PhysicsSetAcceleration(phys, &gravityNone);
			Level1SceneSetMonkeyState(entity, MonkeyIdle);
		}
	}
}

// Render the scene.
void Level1SceneRender(void)
{
	EntityRender(instance.monkey);
	EntityRender(instance.entity);
	EntityRender(instance.livesText);
}

// Exit the scene.
static void Level1SceneExit()
{
	EntityFree(&instance.monkey);
	EntityFree(&instance.livesText);
	EntityFree(&instance.entity);
	SpriteSourceFree(&instance.monkeyJumpSpriteSource);
	SpriteSourceFree(&instance.monkeyIdleSpriteSource);
	SpriteSourceFree(&instance.monkeyWalkSpriteSource);
	SpriteSourceFree(&instance.spriteSource);
	SpriteSourceFree(&instance.livesTextSpriteSource);
	MeshFree(&instance.livesTextMesh);
	MeshFree(&instance.monkeyMesh);
	MeshFree(&instance.mesh);
}

// Unload any resources used by the scene.
static void Level1SceneUnload(void)
{
}

