//------------------------------------------------------------------------------
//
// File Name:	Physics.h
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
#include "Physics.h"
#include "DGL.h"
#include "Stream.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Physics.c.
typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	DGL_Vec2	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	DGL_Vec2	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	DGL_Vec2	velocity;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	//float		inverseMass;

} Physics;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Physics* PhysicsCreate(void)
{
	Physics* physic = calloc(1, sizeof(Physics));

	if (physic)
	{
		return physic;
	}

	return NULL;
}

// Free the memory associated with a Physics component.
// (NOTE: The Physics pointer must be set to NULL.)
// Params:
//	 physics = Pointer to the Physics component pointer.
void PhysicsFree(Physics** physics)
{
	if (*physics)
	{
		(*physics)->oldTranslation.x = 0.0f;
		(*physics)->oldTranslation.y = 0.0f;
		(*physics)->acceleration.x = 0.0f;
		(*physics)->acceleration.y = 0.0f;
		(*physics)->velocity.x = 0.0f;
		(*physics)->velocity.y = 0.0f;
		free(*physics);
		*physics = NULL;
	}
}

// Read the properties of a Physics component from a file.
// [NOTE: Read the acceleration and velocity values using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Physics component.
//	 stream = Pointer to the data stream used for reading.
void PhysicsRead(Physics* physics, Stream stream)
{
	if (stream)
	{
		StreamReadVector2D(stream, &physics->acceleration);
		StreamReadVector2D(stream, &physics->velocity);
	}
	
}

// Get the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetAcceleration(const Physics* physics)
{
	if (physics)
	{
		return &physics->acceleration;
	}

	return NULL;
}

// Get the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetVelocity(const Physics* physics)
{
	if (physics)
	{
		return &physics->velocity;
	}

	return NULL;
}

// Get the old translation (position) of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetOldTranslation(Physics* physics)
{
	if (physics)
	{
		return &physics->oldTranslation;
	}

	return NULL;
}

// Set the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(Physics* physics, const Vector2D* acceleration)
{
	physics->acceleration.x = acceleration->x;
	physics->acceleration.y = acceleration->y;
}

// Set the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(Physics* physics, const Vector2D* velocity)
{
	physics->velocity.x = velocity->x;
	physics->velocity.y = velocity->y;
}

// Update the state of a Physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides and the project instructions.
// (NOTE: This function must verify that the Physics and Transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(Physics* physics, Transform* transform, float dt)
{
	if (physics)
	{
		Vector2DScaleAdd(&physics->velocity, &physics->acceleration, &physics->velocity, dt);


		//TransformGetTranslation(transform) + physics->velocity.x * dt;
		Vector2D translation;
		Vector2DScaleAdd(&translation, &physics->velocity, TransformGetTranslation(transform), dt);
		physics->oldTranslation.x = TransformGetTranslation(transform)->x;
		physics->oldTranslation.y = TransformGetTranslation(transform)->y;
		TransformSetTranslation(transform, &translation);
	}
}

//------------------------------------------------------------------------------

