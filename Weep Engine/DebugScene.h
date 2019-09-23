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
	float vec3a[3] = { 0.0f, 0.0f, 0.0f };
	float dist = 0;
	bool contact = false;
	bool contact_sphere_sphere = false;
	bool contact_sphere_capsules = false;
	bool contact_aabb_aabb = false;
	bool contact_obb_obb = false;
	bool contact_aabb_ray = false;
};
