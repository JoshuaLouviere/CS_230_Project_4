//------------------------------------------------------------------------------
//
// File Name:	Stream.c
// Author(s):	Joshua Louviere (joshua.louviere)
// Project:		Project 1
// Course:		CS230S23
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
// Helpful Links:
//   https://learn.microsoft.com/en-us/dotnet/standard/io/file-path-formats
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stream.h"
#include "Trace.h"
#include "DGL.h"
#include "Vector2D.h"

// Open a file for reading.
// (NOTE: Use fopen_s() with the "rt" option.)
// (NOTE: You must generate the following trace message if the file was not opened successfully:
//	  TraceMessage("Error: StreamOpen could not open file %s; %s", filePath, errorMsg);
//	  Where errorMsg is a char array containing the error message obtained using strerror_s().)
// Params:
//	 filePath = The file to be opened for reading.
// Returns:
//	 NULL if filePath is NULL or the file could not be opened.
//	 Pointer to a FILE object if the file was opened successfully.
Stream StreamOpen(const char* filePath)
{
	FILE *file;

	errno_t error = fopen_s(&file, filePath, "rt");
	
	if (filePath == NULL)
	{
		char errorMsg[80];
		strerror_s(errorMsg, 80, error);
		TraceMessage("Error: StreamOpen could not open file %s; %s", filePath, errorMsg);
		return NULL;
	}

	return file;
}

// Read a single integer from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 0 if the stream was not opened succesfully;
//	 otherwise, an integer value read from the file.
int StreamReadInt(Stream stream)
{
		int check = 0;
	if (stream != NULL)
	{
		fscanf_s(stream, "%i", &check);
		return check;
	}
	return 0;
}

// Read a single float from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for a float.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a float value read from the file,
//	   else return 0.
float StreamReadFloat(Stream stream)
{
	float check = 0.0f;

	if (stream)
	{
		fscanf_s(stream, "%f", &check);
		return check;
	}

	return 0;
}

// Read the data for a Vector2D from a stream.
// (NOTE: Verify that the stream and vector pointer are valid first.)
// (HINT: Use StreamReadFloat() to read the x and y values, in sequence.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream and vector pointer are both valid,
//	   then fill the vector with two float values (x & y),
//	   else do nothing (optionally, write an error message to the trace log).
void StreamReadVector2D(Stream stream, DGL_Vec2* vector)
{
	if (stream && vector)
	{
		vector->x = StreamReadFloat(stream);
		vector->y = StreamReadFloat(stream);
	}
}

// Close an opened stream.
// (NOTE: Do not attempt to close the stream if the pointer is null.)
// (PRO TIP: Avoid dangling pointers by setting the FILE pointer to NULL.)
// Params:
//	 stream = The file stream to be closed.
void StreamClose(Stream* stream)
{
	if (stream != NULL)
	{
		fclose(*stream);
	}
}

// Read a token (a single word) from a file.
// Suggested steps:
//	 - Set the first value in tokenBuffer to 0
//	 - If the stream was opened successfully,
//	   - Read a string ("%s") into tokenBuffer using fscanf_s()
//	 - Return tokenBuffer
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 Pointer to tokenBuffer
const char* StreamReadToken(Stream stream)
{
	static char tokenBuffer[1024];
	tokenBuffer[0] = 0;

	if (stream)
	{
		fscanf_s(stream, "%s", tokenBuffer, (unsigned int)_countof(tokenBuffer));
		return tokenBuffer;
	}

	return NULL;
}

// Read a single boolean value from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a boolean value read from the file as an integer,
//	   else return 0.
bool StreamReadBoolean(Stream stream)
{
	if (stream)
	{
		int check = 0;
		fscanf_s(stream, "%i", &check);
		return check;
	}

	return 0;
}
