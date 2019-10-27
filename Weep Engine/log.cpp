#pragma once
#include "Globals.h"
#include "App.h"
#include "DebugScene.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s", tmp_string);
	OutputDebugString(tmp_string2);

	if (App)
		App->debug_scene->ConsoleLog(tmp_string2);
}