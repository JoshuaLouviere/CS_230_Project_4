//------------------------------------------------------------------------------
//
// File Name:	Entity.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S24
//
// Copyright � 2024 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Entity.h"
#include "Stream.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "Animation.h"
#include "Behavior.h"
#include "BehaviorSpaceship.h"
#include "BehaviorBullet.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Entity
{
	// The name of the entity.
	// A buffer is used to allow each entity to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
	//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
	//	 calling this function!  Instead, use the _countof() macro to get the
	//	 size of the "name" array.]
	char name[32];

	// Pointer to an attached physics component.
	Physics* physics;

	// Pointer to an attached sprite component.
	Sprite* sprite;

	// Pointer to an attached transform component.
	Transform* transform;

	//Pointer to an attached animation componenet.
	Animation* animation;

	// Clarify state of destruction
	bool isDestroyed;

	// Behaviors
	Behavior* behavior;
} Entity;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Entity.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Entity* EntityCreate(void)
{
	Entity* entity = calloc(1, sizeof(Entity));

	if (entity != NULL)
	{
		return entity;
	}

	return NULL;
}

// Free the memory associated with an Entity.
// (NOTE: All attached components must be freed using the corresponding Free() functions.)
// (NOTE: The Entity pointer must be set to NULL.)
// Params:
//	 entity = Pointer to the Entity pointer.
void EntityFree(Entity** entity)
{
	if (*entity)
	{
		// free all parts first then the container
		if ((*entity)->physics) {
			PhysicsFree(&((*entity)->physics));
			(*entity)->physics = NULL;
		}

		TransformFree(&((*entity)->transform));
		(*entity)->transform = NULL;

		if ((*entity)->animation) {
			AnimationFree(&((*entity)->animation));
			(*entity)->animation = NULL;
		}
		
		BehaviorFree(&((*entity)->behavior));
		(*entity)->behavior = NULL;

		SpriteFree(&((*entity)->sprite));
		(*entity)->sprite = NULL;

		free(*entity);
		*entity = NULL;
	}
}

// Attach an Animation component to an Entity.
// (NOTE: This function must set the animation component's parent pointer by
//	  calling the AnimationSetParent() function.)
// Params:
//	 entity = Pointer to the Entity.
//   animation = Pointer to the Animation component to be attached.
void EntityAddAnimation(Entity* entity, Animation* animation)
{
	if (entity && animation)
	{
		entity->animation = animation;
		AnimationSetParent(animation, entity);
	}
}

// Read (and construct) the components associated with a entity.
// [NOTE: See project instructions for implementation instructions.]
// Params:
//	 entity = Pointer to the Entity.
//	 stream = The data stream used for reading.
void EntityRead(Entity* entity, Stream stream)
{
	if (entity && stream)
	{
		const char* token = StreamReadToken(stream);
		EntitySetName(entity, token);

		while (true)
		{
			token = StreamReadToken(stream);
			// printf("%s\n", token);
			if (strncmp(token, "Transform", _countof("Transform")) == 0)
			{
				Transform* transform = TransformCreate();
				TransformRead(transform, stream);
				entity->transform = transform;
			}
			else if (strncmp(token, "Physics", _countof("Physics")) == 0)
			{
				Physics* physics = PhysicsCreate();
				PhysicsRead(physics, stream);
				entity->physics = physics;
			}
			else if (strncmp(token, "Sprite", _countof("Sprite")) == 0)
			{
				Sprite* sprite = SpriteCreate();
				SpriteRead(sprite, stream);
				entity->sprite = sprite;
			}
			else if (strncmp(token, "Animation", _countof("Animation")) == 0)
			{
				Animation* animation = AnimationCreate();
				AnimationRead(animation, stream);
				entity->animation = animation;
			}
			else if (strncmp(token, "BehaviorSpaceship", _countof("BehaviorSpaceship")) == 0) 
			{
				Behavior* beh = BehaviorSpaceshipCreate();
				BehaviorRead(beh, stream);
				EntityAddBehavior(entity, beh);
			}
			else if (strncmp(token, "BehaviorBullet", _countof("BehaviorBullet")) == 0)
			{
				Behavior* beh = BehaviorBulletCreate();
				BehaviorRead(beh, stream);
				EntityAddBehavior(entity, beh);
			}
			else if (strncmp(token, "", _countof("")) == 0)
			{
				return;
			}
		}

	}
}

// Flag an Entity for destruction.
// (Note: This is to avoid Entities being destroyed while they are being processed.)
// Params:
//	 entity = Pointer to the Entity to be flagged for destruction.
// Returns:
//	 If 'entity' is valid,
//	   then set the 'isDestroyed' flag,
//	   else do nothing.
void EntityDestroy(Entity* entity)
{
	if (entity) {
		entity->isDestroyed = true;
	}
}

// Check whether an Entity has been flagged for destruction.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return the value in the "isDestroyed" flag,
//		else return false.
bool EntityIsDestroyed(const Entity* entity)
{
	if (entity) {
		return entity->isDestroyed;
	}

	return false;
}

// Compare the Entity's name with the specified name.
// Params:
//	 entity = Pointer to the Entity.
//   name = Pointer to the name to be checked.
// Returns:
//	 If the Entity pointer is valid and the two names match,
//		then return true,
//		else return false.
bool EntityIsNamed(const Entity* entity, const char* name)
{
	if (entity) {
		return strncmp(entity->name, name, _countof(entity->name)) == 0;
	}

	return false;
}

// Attach a Behavior component to an Entity.
// (NOTE: This function must set the Behavior component's parent pointer by
//	  calling the BehaviorSetParent() function.)
// Params:
//	 entity = Pointer to the Entity.
//   behavior = Pointer to the Behavior component to be attached.
void EntityAddBehavior(Entity* entity, Behavior* behavior)
{
	if (entity && behavior) {
		entity->behavior = behavior;
		BehaviorSetParent(behavior, entity);
	}
}

// Get the Behavior component attached to an Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Behavior component,
//		else return NULL.
Behavior* EntityGetBehavior(const Entity* entity)
{
	if (entity) {
		return entity->behavior;
	}

	return NULL;
}

// Dynamically allocate a clone of an existing Entity.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// (WARNING: You should use the EntityAdd* functions when attaching cloned
//    components to the cloned Entity.  This will ensure that the 'parent'
//    variable is set properly.)
// Params:
//	 other = Pointer to the Entity to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned Entity,
//	   else return NULL.
Entity* EntityClone(const Entity* other)
{
	if (other) {
		//Entity* shallow = &other;
		Entity* deep = EntityCreate();
		EntitySetName(deep, other->name);
		EntityAddSprite(deep, SpriteClone(other->sprite));
		EntityAddAnimation(deep, AnimationClone(other->animation));
		EntityAddBehavior(deep, BehaviorClone(other->behavior));
		deep->isDestroyed = false;
		EntityAddTransform(deep, TransformClone(other->transform));
		EntityAddPhysics(deep, PhysicsClone(other->physics));
		if (deep) {
			return deep;
		}
	}

	return NULL;
}

// Attach a Physics component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   physics = Pointer to the Physics component to be attached.
void EntityAddPhysics(Entity* entity, Physics* physics)
{
	entity->physics = physics;
}

// Attach a sprite component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   sprite = Pointer to the Sprite component to be attached.
void EntityAddSprite(Entity* entity, Sprite* sprite)
{
	entity->sprite = sprite;
}

// Attach a transform component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   transform = Pointer to the Transform component to be attached.
void EntityAddTransform(Entity* entity, Transform* transform)
{
	entity->transform = transform;
}

// Set the Entity's name.
// [NOTE: Verify that both pointers are valid before setting the name.]
// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
//	 calling this function!  Instead, use the _countof() macro to get the
//	 size of the "name" array.]
// Params:
//	 entity = Pointer to the Entity.
//	 name = Pointer to the Entity's new name.
void EntitySetName(Entity* entity, const char* name)
{
	strcpy_s(entity->name, _countof(entity->name), name);

}

// Get the Entity's name.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the Entity's name,
//		else return NULL.
const char* EntityGetName(const Entity* entity)
{
	if (entity)
	{
		return entity->name;
	}

	return NULL;
}

// Get the Physics component attached to an Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached physics component,
//		else return NULL.
Physics* EntityGetPhysics(const Entity* entity)
{
	if (entity)
	{
		return entity->physics;
	}

	return NULL;
}

// Get the Animation component attached to an Entity.
// Params:
//	 entity = Pointer to the Entity .
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Animation component,
//		else return NULL.
Animation* EntityGetAnimation(const Entity* entity)
{
	if (entity)
	{
		return entity->animation;
	}

	return NULL;
}

// Get the Sprite component attached to a Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Sprite component,
//		else return NULL.
Sprite* EntityGetSprite(const Entity* entity)
{
	if (entity)
	{
		return entity->sprite;
	}

	return NULL;
}

// Get the Transform component attached to a Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Transform component,
//		else return NULL.
Transform* EntityGetTransform(const Entity* entity)
{
	if (entity)
	{
		return entity->transform;
	}

	return NULL;
}

// Update any components attached to the Entity.
// (Hint: You will need to call PhysicsUpdate().)
// (NOTE: You must first check for a valid pointer before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
//	 dt = Change in time (in seconds) since the last game loop.
void EntityUpdate(Entity* entity, float dt)
{
	if (entity)
	{
		BehaviorUpdate(entity->behavior, dt);
		PhysicsUpdate(entity->physics, entity->transform, dt);
		AnimationUpdate(entity->animation, dt);
	}
}

// Render any visible components attached to the Entity.
// (Hint: You will need to call SpriteRender(), passing the Sprite and Transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
void EntityRender(Entity* entity)
{
	if (entity)
	{
		SpriteRender(entity->sprite, entity->transform);
	}
}

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

