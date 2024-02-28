//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "DGL.h"
#include "Vector2D.h"
#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

// Frequently, vector and matrix structures are declared publicly.  While an
// interface is typically provided with these structures, these are two common
// exceptions to the coding best practice of "hiding information".

typedef struct DGL_Vec2 Vector2D;
typedef struct DGL_Mat4 Matrix2D;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Macros:
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

// This function sets the matrix equal to the identity matrix.
void Matrix2DIdentity(Matrix2D* pResult)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				pResult->m[i][j] = 1;
			}
			else
			{
				pResult->m[i][j] = 0;
			}
		}
	}
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2DTranspose(Matrix2D* pResult, const Matrix2D* pMtx)
{
	Matrix2D bob;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			bob.m[i][j] = pMtx->m[j][i];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pResult->m[i][j] = bob.m[i][j];
		}
	}
}

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2DConcat(Matrix2D* pResult, const Matrix2D* pMtx0, const Matrix2D* pMtx1)
{
	Matrix2D bob;
	float temp = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				temp += pMtx0->m[i][k] * pMtx1->m[k][j];
			}
			bob.m[i][j] = temp;
			temp = 0;
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pResult->m[i][j] = bob.m[i][j];
		}
	}
}

// This function creates a translation matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
	Matrix2DIdentity(pResult);

	pResult->m[0][3] = x;
	pResult->m[1][3] = y;
}

// This function creates a scaling matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
	Matrix2DIdentity(pResult);

	pResult->m[0][0] = x;
	pResult->m[1][1] = y;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// (Hint: This function must initialize all matrix values.)
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Matrix2DRotDeg(Matrix2D* pResult, float angle)
{
	Matrix2DIdentity(pResult);
	float Radians = (angle * (float)M_PI) / 180.0f;
	pResult->m[0][0] = cosf(Radians);
	pResult->m[0][1] = -sinf(Radians);
	pResult->m[1][0] = sinf(Radians);
	pResult->m[1][1] = cosf(Radians);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// (Hint: This function must initialize all matrix values.)
void Matrix2DRotRad(Matrix2D* pResult, float angle)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = cosf(angle);
	pResult->m[0][1] = -sinf(angle);
	pResult->m[1][0] = sinf(angle);
	pResult->m[1][1] = cosf(angle);
}


// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Result = Mtx * Vec.
// (NOTE: Care must be taken when pResult = pVec.)
// (NOTE: Remember to treat the implied w component as 1.)
void Matrix2DMultVec(Vector2D* pResult, const Matrix2D* pMtx, const Vector2D* pVec)
{
	Vector2D bob;

	bob.x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][3];
	bob.y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][3];

	pResult->x = bob.x;
	pResult->y = bob.y;
}

//------------------------------------------------------------------------------
