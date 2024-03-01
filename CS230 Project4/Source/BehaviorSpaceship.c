//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.h
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
#include "DGL.h"

//------------------------------------------------------------------------------

#define PI 3.14f

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

static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.034f;
static const float spaceshipWeaponBulletSpeed = 750.0f;

static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior);

static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(behavior);
}
static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(behavior);

}
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(behavior);

}
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);

}

// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorSpaceshipCreate(void)
{
	Behavior* beh = calloc(1, sizeof(Behavior));
	beh->onInit = BehaviorSpaceshipInit;
	beh->onUpdate = BehaviorSpaceshipUpdate;
	beh->onExit = BehaviorSpaceshipExit;
	beh->stateCurr = cSpaceshipInvalid;
	beh->stateNext = cSpaceshipInvalid;
	return beh;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorSpaceshipInit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipUpdate(Behavior* behavior, float dt)
{
	dt = dt;
	behavior = behavior;
	if (DGL_Input_KeyDown(VK_UP)) {
		printf("WOW\n");
	}

	switch (behavior->stateCurr) {
		case cSpaceshipIdle:
			BehaviorSpaceshipUpdateRotation(behavior, dt);
			BehaviorSpaceshipUpdateWeapon(behavior, dt);
			if (DGL_Input_KeyDown(VK_UP)) {
				behavior->stateNext = cSpaceshipThrust;
			}
			break;
		case cSpaceshipThrust:
			BehaviorSpaceshipUpdateRotation(behavior, dt);
			BehaviorSpaceshipUpdateWeapon(behavior, dt);
			BehaviorSpaceshipUpdateVelocity(behavior, dt);
			if (DGL_Input_KeyReleased(VK_UP)) {
				behavior->stateNext = cSpaceshipIdle;
			}
			break;
	}

	
	BehaviorSpaceshipSpawnBullet(behavior);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipExit(Behavior* behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------

