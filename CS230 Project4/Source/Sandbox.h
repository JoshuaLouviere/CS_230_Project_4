//------------------------------------------------------------------------------
//
// File Name:	Vector2D.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 1
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
// Helpful Links:
//   https://docs.microsoft.com/en-us/cpp/c-runtime-library/math-constants
//   https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atan-atanf-atanl-atan2-atan2f-atan2l
//   https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/cos-cosf-cosl
//   https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sqrt-sqrtf-sqrtl
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct DGL_Vec2 DGL_Vec2;

//------------------------------------------------------------------------------
// Public Constants:
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

// This function sets the coordinates of the 2D vector (pResult) to 0
void Vector2DZero(DGL_Vec2 *pResult);

// This function sets the coordinates of the 2D vector (pResult) to x & y
void Vector2DSet(DGL_Vec2 *pResult, float x, float y);

// In this function, pResult will be set to the opposite of pVec0 (e.g. x = -x & y = -y)
void Vector2DNeg(DGL_Vec2 *pResult, const DGL_Vec2 *pVec0);

// In this function, pResult will be the sum of pVec0 and pVec1 (x result = x0 + x1)
void Vector2DAdd(DGL_Vec2 *pResult, const DGL_Vec2 *pVec0, const DGL_Vec2 *pVec1);

// In this function, pResult will be the difference between pVec0 and pVec1 (specifically, pVec0 - pVec1)
void Vector2DSub(DGL_Vec2 *pResult, const DGL_Vec2 *pVec0, const DGL_Vec2 *pVec1);

// In this function, pResult will be the unit vector of pVec0
// (NOTE: Care must be taken when pResult = pVec0!)
void Vector2DNormalize(DGL_Vec2 *pResult, const DGL_Vec2 *pVec0);

// In this function, pResult will be the vector pVec0 scaled by the value 'scale'
void Vector2DScale(DGL_Vec2 *pResult, const DGL_Vec2 *pVec0, float scale);

// In this function, pResult will be the vector pVec0 scaled by 'scale' and added to pVec1
void Vector2DScaleAdd(DGL_Vec2 *pResult, const DGL_Vec2 *pVec0, const DGL_Vec2 *pVec1, float scale);

// In this function, pResult will be the vector pVec0 scaled by 'scale' and pVec1 will be subtracted from it
void Vector2DScaleSub(DGL_Vec2 *pResult, const DGL_Vec2 *pVec0, const DGL_Vec2 *pVec1, float scale);

// This function returns the length of the vector pVec0
float Vector2DLength(const DGL_Vec2 *pVec0);

// This function returns the square of pVec0's length.
// NOTE: The square root function must NOT be called by this function.
float Vector2DSquareLength(const DGL_Vec2 *pVec0);

// This function returns the distance between two points.
float Vector2DDistance(const DGL_Vec2 *pVec0, const DGL_Vec2 *pVec1);

// This function returns the distance squared between two points.
// NOTE: The square root function must NOT be called by this function.
float Vector2DSquareDistance(const DGL_Vec2 *pVec0, const DGL_Vec2 *pVec1);

// This function returns the dot product between pVec0 and pVec1
float Vector2DDotProduct(const DGL_Vec2 *pVec0, const DGL_Vec2 *pVec1);

// This function computes the coordinates of the unit vector represented by the angle "angle", which is in Degrees.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Vector2DFromAngleDeg(DGL_Vec2 *pResult, float angle);

// This function computes the coordinates of the unit vector represented by the angle "angle", which is in Radians.
// HINT: x = cos(angle), y = sin(angle).
void Vector2DFromAngleRad(DGL_Vec2 *pResult, float angle);

// This function computes the angle, in radians, of the specified vector.
// HINT: Angle (radians) = atan2f(direction Y, direction X).
// NOTE: If the pointer is NULL, then return 0.0f.
float Vector2DToAngleRad(const DGL_Vec2 * pVec);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

