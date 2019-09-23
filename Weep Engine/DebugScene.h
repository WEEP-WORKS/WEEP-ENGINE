#pragma once

#include "Module.h"
#include "Globals.h"

#define MAX_SNAKE 2



class DebugScene : public Module
{
public:
	DebugScene(bool start_enabled = true);
	~DebugScene();

	bool Awake();
	bool Start();
	bool Update();
	void AppAbout();
	bool CleanUp();

public:

	// Our state
	//bool show_demo_window = false;
	//bool show_another_window = false;

	// Main loop
	bool done = false;

private:

	void GeometryMathTest();

private:

	bool show_app_about = false;
	bool show_demo_window = false;

	// Geometry math test
	bool geometry_math_test = false;

	bool contact = false;

};
