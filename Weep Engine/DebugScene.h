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

	// Main loop
	bool done = false;

private:

	bool show_app_about = false;
	bool show_demo_window = false;
	bool show_geometry_math_test = false;
	bool show_random_generator = false;
	bool to_save = false;
	bool to_load = false;

	//MathGeo
	bool contact = false;

	//Random Number
	float2 range_demo; // for the random generation demo
	float2 range_demo1;
	int quantity_demo = 1; // number of random generations of the demo

private:

	void LoadStyle(const char * name);

	//Random Number
	void RandomGenerator();
	void GenerateRandomNumbers(float2 range, float2 range1, int quantity);
	float GetFloatRandomValue(float range_f1, float range_f2);
	int GetIntRandomValue(float range_f1, float range_f2);

	//MathGeo
	void MathGeoTest();

	float vec3a[3] = { 0.0f, 0.0f, 0.0f };
	float dist = 0;
	bool contact_sphere_sphere = false;
	bool contact_sphere_capsules = false;
	bool contact_aabb_aabb = false;
	bool contact_obb_obb = false;
	bool contact_aabb_ray = false;
};