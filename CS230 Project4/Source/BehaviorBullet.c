//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.h
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
#include "Behavior.h"
#include "BehaviorSpaceship.h"
#include "BehaviorBullet.h"
#include "Vector2D.h"
#include "Physics.h"
#include "DGL.h"
#include "Transform.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Scene.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Behavior Behavior;

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

static const float spaceshipSpeedMax = 500.0f;

static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt);

static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt)
{
	/*o	If the behavior timer > 0,
			Decrement the behavior timer by ‘dt’.
			If the behavior timer <= 0,
		•	Call EntityDestroy(), passing the behavior’s ‘parent’ pointer.*/
	if (behavior->timer > 0) {
		behavior->timer -= dt;
		if (behavior->timer <= 0) {
			behavior->timer = 0;
			EntityDestroy(behavior->parent);
		}
	}
}

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorBulletCreate(void)
{
	Behavior* beh = calloc(1, sizeof(Behavior));

	if (beh) {
		beh->onInit = BehaviorBulletInit;
		beh->onUpdate = BehaviorBulletUpdate;
		beh->onExit = BehaviorBulletExit;
		beh->stateCurr = cBulletInvalid;
		beh->stateNext = cBulletInvalid;
	}

	return beh;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorBulletInit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletUpdate(Behavior* behavior, float dt)
{
	/*o	Add a switch statement with the expression, (behavior->stateCurr)
		o	Add a case for cBulletIdle.
			Call BehaviorBulletUpdateLifeTimer.*/
	switch (behavior->stateCurr) {
		case cBulletIdle:
			BehaviorBulletUpdateLifeTimer(behavior, dt);
			break;
	}

}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
