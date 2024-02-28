//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
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
#include "Sprite.h"
#include "Stream.h"
#include "Mesh.h"
#include "Matrix2D.h"
#include "DGL.h"
#include "SpriteSource.h"
#include "Transform.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Mesh Mesh;
typedef struct SpriteSource SpriteSource;
typedef struct Sprite Sprite;
typedef struct Transform Transform;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Sprite
{
	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	const SpriteSource* spriteSource;

	// The mesh used to draw the sprite.
	const Mesh* mesh;

	const char* text;

} Sprite;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Sprite component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: You must initialize the 'alpha' member variable to 1.0f.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Sprite* SpriteCreate(void)
{
	Sprite* sprite = calloc(1, sizeof(Sprite));

	if (sprite)
	{
		sprite->alpha = 1.0f;
		return sprite;
	}

	return NULL;
}

// Free the memory associated with a Sprite component.
// (NOTE: The Sprite pointer must be set to NULL.)
// Params:
//	 sprite = Pointer to the Sprite pointer.
void SpriteFree(Sprite** sprite)
{
	if (sprite)
	{
		free(*sprite);
		*sprite = NULL;
	}
		
}

// Assign a text string to a Sprite component.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	 string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 text = Pointer to a zero-terminated array of characters.
void SpriteSetText(Sprite* sprite, const char* text)
{
	if (sprite)
	{
		sprite->text = text;
	}
}

// Read the properties of a Sprite component from a file.
// [NOTE: Read the frameIndex value using StreamReadInt.]
// [NOTE: Read the alpha value using StreamReadFloat.]
// Params:
//	 sprite = Pointer to the Sprite component.
//	 stream = The data stream used for reading.
void SpriteRead(Sprite* sprite, Stream stream)
{
	if (stream)
	{
		sprite->frameIndex = StreamReadInt(stream);
		sprite->alpha = StreamReadFloat(stream);
	}
}

// Render a Sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the Sprite component.
//   transform = Pointer to the Transform component.
void SpriteRender(const Sprite* sprite, Transform* transform)
{
	if (sprite)
	{
		// Prepare to render a textured sprite
		if (sprite->spriteSource)
		{
			DGL_Graphics_SetShaderMode(DGL_SM_TEXTURE);
			// Set texture and texture offsets

			SpriteSourceSetTexture(sprite->spriteSource);
			SpriteSourceSetTextureOffset(sprite->spriteSource, sprite->frameIndex);
		}
		else
		{
			DGL_Graphics_SetShaderMode(DGL_SM_COLOR);
			DGL_Graphics_SetTexture(NULL);
		}

		// Set transparency (range 0.0f – 1.0f)
		DGL_Graphics_SetCB_Alpha(sprite->alpha);
		// Set blend color (RGBA, A = “strength” of blend)
		DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f});
		// Render the mesh (list of triangles)

		// Set position, scale, and rotation for the sprite
		if (sprite->text == NULL)
		{
			Matrix2D bob = *TransformGetMatrix(transform);
			DGL_Graphics_SetCB_TransformMatrix(&bob);
			MeshRender(sprite->mesh);
		}
		else
		{
			Matrix2D bob = *TransformGetMatrix(transform);
			Matrix2D offset;

			Matrix2DTranslate(&offset, TransformGetScale(transform)->x, 0);
			const char* text = sprite->text;
			int index = 0;
			while (*(text + index) != '\0')
			{
				SpriteSourceSetTextureOffset(sprite->spriteSource, *(text + index) - ' ' / 2);
				DGL_Graphics_SetCB_TransformMatrix(&bob);
				MeshRender(sprite->mesh);

				index++;

				Matrix2DConcat(&bob, &offset, &bob);
			}
		}
	}
}

// Get a Sprite's alpha value.
// Params:
//	 sprite = Pointer to the Sprite component.
// Returns:
//	 If the pointer is valid,
//		then return the Sprite's alpha value (a value between 0.0f and 1.0f),
//		else return 0.0f.
float SpriteGetAlpha(const Sprite* sprite)
{
	if (sprite)
	{
		return sprite->alpha;
	}

	return 0.0f;
}


// Set a Sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: The min() and max() macros can be combined to create a clamp function.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   alpha = The Sprite's new alpha value.
void SpriteSetAlpha(Sprite* sprite, float alpha)
{
	if (sprite)
	{
		min(0.0f, alpha);
		max(1.0f, alpha);
		
		sprite->alpha = alpha;
	}
}

// Set a Sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count] before changing the Sprite's frame index!)
// Params:
//	 sprite = Pointer to the Sprite component.
//   frameIndex = New frame index for the Sprite (0 .. frame count).
// ADDITIONAL REQUIREMENTS:
// - This function must make the following function call:
//     TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
void SpriteSetFrame(Sprite* sprite, unsigned int frameIndex)
{
	if (sprite)
	{
		printf("FrameIndex: %i\n", frameIndex);
		sprite->frameIndex = frameIndex;
		TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
	}
}

// Set the Sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the Sprite component.
//   mesh = Pointer to a Mesh object.
void SpriteSetMesh(Sprite* sprite, const Mesh* mesh)
{
	if (sprite)
	{
		sprite->mesh = mesh;
	}
}

// Set a new SpriteSource for the specified Sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a Sprite and cause the Sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the Sprite component.
//	 spriteSource = Pointer to a SpriteSource (this pointer may be NULL).
void SpriteSetSpriteSource(Sprite* sprite, const SpriteSource* spriteSource)
{
	if (sprite)
	{
		sprite->spriteSource = spriteSource;
	}
}

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

