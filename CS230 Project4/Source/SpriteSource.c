//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.h
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

#include "stdafx.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "Stream.h"
#include "DGL.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct DGL_Texture DGL_Texture;
typedef struct SpriteSource SpriteSource;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct SpriteSource
{
	// The dimensions of the sprite sheet.
	// (Hint: These should be initialized to (1, 1).)
	int	numRows;
	int	numCols;

	// Pointer to a texture resource created using the DigiPen Graphics Library (DGL).
	const DGL_Texture* texture;

} SpriteSource;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new SpriteSource object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: numRows and numCols should be initialized to 1.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpriteSource* SpriteSourceCreate()
{
	SpriteSource *source = calloc(1, sizeof(SpriteSource));

	if (source)
	{
		source->numCols = 1;
		source->numRows = 1;
		return source;
	}

	return NULL;

}

// Free the memory associated with a SpriteSource object.
// (NOTE: The SpriteSource pointer must be set to NULL.)
// Params:
//	 spriteSource = Pointer to the SpriteSource pointer.
void SpriteSourceFree(SpriteSource** spriteSource)
{
	if (*spriteSource)
	{
		free(*spriteSource);
		*spriteSource = NULL;
	}
}

// Load a texture from a file (may be an Col x Row sprite sheet).
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 textureName = The name of the texture to be loaded.
void SpriteSourceLoadTexture(SpriteSource* spriteSource, int numCols, int numRows, const char* textureName)
{
	FILE* file = StreamOpen(textureName);

	if (spriteSource)
	{
		spriteSource->numCols = numCols;
		spriteSource->numRows = numRows;
		spriteSource->texture = DGL_Graphics_LoadTexture(textureName);
	}

	if (file)
	{
		StreamClose(&file);
	}
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
// Returns:
//	 If the SpriteSource pointer is valid,
//		then return the calculated frame count (numCols * numRows),
//		else return 0.
unsigned SpriteSourceGetFrameCount(const SpriteSource* spriteSource)
{
	if (spriteSource)
	{
		return spriteSource->numCols * spriteSource->numRows;
	}

	return 0;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
//	 frameIndex = A frame index within a sprite sheet.
//   u = Pointer to a float to contain the U coordinate. 
//   v = Pointer to a float to contain the V coordinate. 
void SpriteSourceGetUV(const SpriteSource* spriteSource, unsigned int frameIndex, float* u, float* v)
{
	if (spriteSource && u && v)
	{
		float uSize = 1.0f / spriteSource->numCols;
		float vSize = 1.0f / spriteSource->numRows;
		*u = uSize * (frameIndex % spriteSource->numCols);
		*v = vSize * (frameIndex * spriteSource->numCols);
	}
}

// Sets a SpriteSource texture for rendering.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
void SpriteSourceSetTexture(const SpriteSource* spriteSource)
{
	if (spriteSource->texture)
	{
		DGL_Graphics_SetTexture(spriteSource->texture);
	}
}

// Sets the texture UV offsets for rendering.
// Params:
//	 spriteSource = Pointer to the SpriteSource object.
void SpriteSourceSetTextureOffset(const SpriteSource* spriteSource, unsigned frameIndex)
{
	if (spriteSource)
	{
		float uSize = 1.0f / spriteSource->numCols;
		float vSize = 1.0f / spriteSource->numRows;
		float uOff = uSize * (frameIndex % spriteSource->numCols);
		float vOff = vSize * (frameIndex / spriteSource->numCols);
		
		DGL_Vec2 Vec;
		Vector2DSet(&Vec, uOff, vOff);
	
		DGL_Graphics_SetCB_TextureOffset(&Vec);
	}
}

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

