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
#include "Vector2D.h"
#include "Physics.h"
#include "DGL.h"
#include "Transform.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Scene.h"

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
	/*If ‘VK_LEFT’ is pressed,
	Set the physics component’s rotation velocity = spaceshipTurnRateMax
	Else If ‘VK_RIGHT’ is pressed,
	Set the physics component’s rotation velocity = -spaceshipTurnRateMax
	Else
	Set the physics component’s rotation velocity = 0*/
	Physics* phys = EntityGetPhysics(behavior->parent);
	if (DGL_Input_KeyDown(VK_LEFT)) {
		PhysicsSetRotationalVelocity(phys, spaceshipTurnRateMax);
	}
	else if (DGL_Input_KeyDown(VK_RIGHT)) {
		PhysicsSetRotationalVelocity(phys, -spaceshipTurnRateMax);
	}
	else {
		PhysicsSetRotationalVelocity(phys, 0);
	}
	UNREFERENCED_PARAMETER(dt);
}

static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(behavior);
	/*o	Get the transform and physics components from the behavior’s parent Entity.
		o	Verify that the pointers are valid.
		o	Get the transform component’s ‘rotation’.
		o	Get a unit vector in direction of ‘rotation’.
			Hint : There is a Vector2D function for this
		o	Get the physics component’s ‘velocity’.
		o	Calculate the new velocity :
			velocity = velocity + direction of rotation * spaceshipAcceleration * dt
			Note : Try using the Vector2DScaleAdd function for this
		o	Calculate the ‘speed’ of this new velocity.
			Hint : speed = length(new velocity)
		o	If the speed > spaceshipSpeedMax, then limit the speed :
		velocity = velocity * (spaceshipMaxSpeed / speed)
		o	Set the physics component’s new velocity.*/
	Transform* tra = EntityGetTransform(behavior->parent);
	Physics* phy = EntityGetPhysics(behavior->parent);
	if (phy && tra) {
		float rot = TransformGetRotation(tra);

		Vector2D* dir = calloc(1, sizeof(Vector2D));
		Vector2DFromAngleRad(dir, rot);

		Vector2D* vel = (Vector2D *)PhysicsGetVelocity(phy);
		Vector2DScaleAdd(vel, dir, vel, spaceshipAcceleration * dt);

		PhysicsSetVelocity(phy, vel);
	}



}
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt)
{
	/*o	If the behavior timer > 0,
			Decrement the behavior timer by ‘dt’.
			If the behavior timer < 0,
		•	Set the behavior timer = 0
	o	If spacebar(‘ ‘) is pressed,
			If behavior timer <= 0
		•	Call BehaviorSpaceshipSpawnBullet
		•	Set behavior timer = spaceshipWeaponCooldownTime*/
	if (behavior->timer > 0) {
		behavior->timer -= dt;
		if (behavior->timer <= 0) {
			behavior->timer = 0;
		}
	}
	
	if (DGL_Input_KeyDown(VK_SPACE)) {
		if (behavior->timer <= 0) {
			BehaviorSpaceshipSpawnBullet(behavior);
			behavior->timer = spaceshipWeaponCooldownTime;
			printf("Shoot\n");
		}
	}
}
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior)
{
	/*o	Call EntityFactoryBuild to build a new ‘Bullet’ Entity.
			NOTE : The ‘entityName’ parameter is case-sensitive!
		o	If the Bullet was cloned successfully
			Get the spaceship’s position and rotation.
			Set the cloned bullet’s position and rotation.
			Get a unit vector in direction of the spaceship’s ‘rotation’.
			Set the bullet’s velocity = direction * spaceshipWeaponBulletSpeed
			Add the cloned bullet to the Entity manager’s active list.*/
  	Entity* ent = EntityFactoryBuild("Bullet");
	if (ent) {
		Transform* tran = EntityGetTransform(behavior->parent);
		float rot = TransformGetRotation(tran);
		const Vector2D* pos = TransformGetTranslation(tran);
		Transform* bulTran = EntityGetTransform(ent);
		TransformSetTranslation(bulTran, pos);
		TransformSetRotation(bulTran, rot);

		Vector2D* dir = calloc(1, sizeof(Vector2D));
		Vector2DFromAngleRad(dir, rot);

		Physics* bulPhy = EntityGetPhysics(ent);
		//bulPhy->oldTranslation = (Vector2D*){ behavior};
		Vector2D* vel = calloc(1, sizeof(Vector2D));
		Vector2DScale(vel, dir, spaceshipWeaponBulletSpeed);
		PhysicsSetVelocity(bulPhy, vel);
		SceneAddEntity(ent);
	}
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

