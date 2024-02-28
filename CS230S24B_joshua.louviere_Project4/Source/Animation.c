//------------------------------------------------------------------------------
//
// File Name:	Animation.h
// Author(s):	Doug Schilling (dschilling) Joshua Louviere (joshua.louviere)
// Project:		Project 3
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "Stream.h"
#include "Entity.h"
#include "Sprite.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Entity Entity;
typedef struct Animation Animation;
typedef struct Sprite Sprite;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Animation
{
	// Pointer to the parent Entity associated with the Animation component.
	Entity* parent;

	// The current frame being displayed.X
	unsigned int frameIndex;

	// The maximum number of frames in the sequence.X
	unsigned int frameCount;

	// The time remaining for the current frame.X
	float frameDelay;

	// The amount of time to display each successive frame.X
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.X
	bool isRunning;

	// True if the animation loops back to the beginning.X
	bool isLooping;

	// True if the end of the animation sequence has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)X
	bool isDone;

} Animation;

static void AnimationAdvanceFrame(Animation* animation);

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Animation component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Animation* AnimationCreate(void)
{
	Animation* anim = calloc(1, sizeof(Animation));
	
	if (anim)
	{
		return anim;
	}

	return NULL;
}

// Free the memory associated with an Animation component.
// (NOTE: The Animation pointer must be set to NULL.)
// Params:
//	 animation = Pointer to the Animation pointer.
void AnimationFree(Animation** animation)
{
	if (animation)
	{
		free(*animation);
		*animation = NULL;
	}
}

// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = The data stream used for reading.
void AnimationRead(Animation* animation, Stream stream)
{
	if (animation && stream)
	{
		animation->frameIndex = StreamReadInt(stream);
		animation->frameCount = StreamReadInt(stream);
		animation->frameDelay = StreamReadFloat(stream);
		animation->frameDuration = StreamReadFloat(stream);
		animation->isRunning = StreamReadInt(stream);
		animation->frameCount = StreamReadInt(stream);
	}
}

// Set the parent Entity for an Animation component.
// Params:
//	 animation = Pointer to the Animation component.
//	 parent = Pointer to the parent Entity.
void AnimationSetParent(Animation* animation, Entity* parent)
{
	if (animation && parent)
	{
		animation->parent = parent;
	}
}

// Play a simple animation sequence [0 .. frameCount - 1].
// (Hint: This function must initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the Animation component.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to display each frame (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void AnimationPlay(Animation* animation, int frameCount, float frameDuration, bool isLooping)
{
	if (animation)
	{
		animation->frameCount = frameCount;
		animation->frameDuration = frameDuration;
		animation->isLooping = isLooping;
		animation->frameDelay = frameDuration;
		animation->isRunning = true;
		animation->isDone = false;
		animation->frameIndex = 0;
		Sprite* sprite = EntityGetSprite(animation->parent);
		SpriteSetFrame(sprite, animation->frameIndex);
	}
}

static void AnimationAdvanceFrame(Animation* animation)
{
	if (animation)
	{
		animation->frameIndex++;

		if (animation->frameIndex >= animation->frameCount)
		{
			if (animation->isLooping == true)
			{
				animation->frameIndex = 0;

			}
			else
			{
				animation->frameIndex = animation->frameCount - 1;
				animation->isRunning = false;
			}

			animation->isDone = true;
		}

		if (animation->isRunning == true)
		{
			Sprite* sprite = EntityGetSprite(animation->parent);
			SpriteSetFrame(sprite, animation->frameIndex);
			animation->frameDelay += animation->frameDuration;
		}
		else
		{
			animation->frameDelay = 0;
		}
	}
}

// Update the animation.
// Params:
//	 animation = Pointer to the Animation component.
//	 dt = Change in time (in seconds) since the last game loop.
void AnimationUpdate(Animation* animation, float dt)
{
	if (animation)
	{
		animation->isDone = false;

		if (animation->isRunning)
		{
			animation->frameDelay -= dt;
		
			if (animation->frameDelay <= 0)
			{
				AnimationAdvanceFrame(animation);
			}
		}
	}
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the Animation component.
// Returns:
//	 If the Animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool AnimationIsDone(const Animation* animation)
{
	if (animation)
	{
		return animation->isDone;
	}

	return false;
}
