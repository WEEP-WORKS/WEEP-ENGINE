#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != nullptr )        \
	    {                      \
      delete x;            \
	  x = nullptr;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != nullptr )              \
	    {                            \
      delete[] x;                \
	  x = nullptr;                    \
	    }                            \
                              \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long long UID;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define ASSETS_FOLDER "/Assets/"
#define SETTINGS_FOLDER "/Settings/"
#define LIBRARY_FOLDER "/Library/"
#define LIBRARY_TEXTURES_FOLDER "Library/Textures/"
#define LIBRARY_MESH_FOLDER "/Library/Meshes/"

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};