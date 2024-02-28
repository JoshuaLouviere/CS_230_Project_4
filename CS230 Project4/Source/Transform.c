//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DGL.h"
#include "Transform.h"
#include "Stream.h"
#include "Vector2D.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Transform Transform;
typedef struct DGL_Vec2 Vector2D;
typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

float rot = 0;
DGL_Vec2 tra = { 0,0 };
DGL_Vec2 sca = { 0,0 };

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Transform
{
	// The translation (or world position) of an entity.
	DGL_Vec2	translation;

	// The rotation (or orientation) of an entity (in radians).
	float	rotation;

	// The scale (or size) of an entity.
	// (Hint: This should be initialized to (1, 1).)
	DGL_Vec2	scale;

	// True if the transformation matrix needs to be recalculated.
	// (Hint: This should be initialized to true.)
	// (Hint: This should be set to true when the Transform data changes.)
	bool	isDirty;

	// The transformation matrix resulting from concatenating the matrices
	//   representing the translation, rotation, and scale transformations.
	//	 (e.g. matrix = Translation*Rotation*Scale matrices)
	Matrix2D	matrix;

} Transform;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Transform component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: You must initialize the scale values to non-zero values.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Transform* TransformCreate(void)
{
	Transform* transform = calloc(1, sizeof(Transform));

	if (transform)
	{
		transform->scale.x = 1;
		transform->scale.y = 1;
		transform->isDirty = true;
		return transform;
	}

		return NULL;
}

// Free the memory associated with a Transform component.
// (NOTE: The Transform pointer must be set to NULL.)
// Params:
//	 transform = Pointer to the Transform pointer.
void TransformFree(Transform** transform)
{
	if (*transform)
	{
		(*transform)->scale.x = 0.0f;
		(*transform)->scale.y = 0.0f;
		(*transform)->translation.x = 0.0f;
		(*transform)->translation.y = 0.0f;
		(*transform)->rotation = 0.0f;
		free(*transform);
		*transform = NULL;
	}
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
const Matrix2D* TransformGetMatrix(Transform* transform)
{
	if (transform->rotation != rot || &transform->translation != &tra || &transform->scale != &sca)
	{
		transform->isDirty = true;
		rot = transform->rotation;
		tra = transform->translation;
		sca = transform->scale;
	}

	if (transform)
	{
		if (transform->isDirty)
		{
			Matrix2D scale;
			Matrix2D translation;
			Matrix2D rotate;
			Matrix2D matrix;
			Matrix2DScale(&scale, transform->scale.x, transform->scale.y);
			Matrix2DTranslate(&translation, transform->translation.x, transform->translation.y);
			Matrix2DRotRad(&rotate, transform->rotation);
			Matrix2DConcat(&matrix, &rotate, &scale);
			Matrix2DConcat(&transform->matrix, &translation, &matrix);
			//Matrix2DIdentity(&transform->matrix);
			transform->isDirty = false;
		}

		return &transform->matrix;
	}

	return NULL;
}

// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 transform = Pointer to the Transform component.
//	 stream = The data stream used for reading.
void TransformRead(Transform* transform, Stream stream)
{
	if (transform && stream)
	{
		StreamReadVector2D(stream, &transform->translation);
		transform->rotation = StreamReadFloat(stream);
		StreamReadVector2D(stream, &transform->scale);
	}
}

// Get the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's translation,
//		else return a NULL pointer.
const Vector2D* TransformGetTranslation(const Transform* transform)
{
	if (transform)
	{
		return &transform->translation;
	}

	return NULL;
}

// Get the rotation value of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float TransformGetRotation(const Transform* transform)
{
	if (transform)
	{
		return transform->rotation;
	}

	return 0.0f;
}

// Get the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's scale,
//		else return a NULL pointer.
const Vector2D* TransformGetScale(const Transform* transform)
{
	if (transform)
	{
		return &transform->scale;
	}

	return NULL;
}

// Set the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 translation = Pointer to the new translation.
void TransformSetTranslation(Transform* transform, const Vector2D* translation)
{
	transform->translation.x = translation->x;
	transform->translation.y = translation->y;
}

// Set the rotation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 rotation = The rotation value (in radians).
void TransformSetRotation(Transform* transform, float rotation)
{
	transform->rotation = rotation;
}

// Set the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 translation = Pointer to the new scale.
void TransformSetScale(Transform* transform, const Vector2D* scale)
{
	transform->scale.x = scale->x;
	transform->scale.y = scale->y;
}

//------------------------------------------------------------------------------
