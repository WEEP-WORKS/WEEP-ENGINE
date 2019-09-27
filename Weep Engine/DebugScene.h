#pragma once

#include "Module.h"
#include "Globals.h"
#include "imgui.h"

#define MAX_SNAKE 2

struct hardware_info {
	uint gpu_vendor = 0;
	uint gpu_device = 0;
	char gpu_brand[250] = "";
	float vram_mb_budget = 0.f;
	float vram_mb_usage = 0.f;
	float vram_mb_available = 0.f;
	float vram_mb_reserved = 0.f;
};

class DebugScene : public Module
{
public:
	DebugScene(bool start_enabled = true);
	~DebugScene();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	void Configuration();
	bool CleanUp();
	void OnConfiguration();

	void ConsoleLog(const char * text);

public:

	// Main loop
	bool done = false;

private:

	bool show_app_about = false;
	bool show_demo_window = false;
	bool show_geometry_math_test = false;
	bool show_random_generator = false;
	bool show_debug_console = false;

	bool to_save = false;
	bool to_load = false;

	bool show_app_configuration = false;

	//MathGeo
	bool contact = false;
	float vec3a[3] = { 0.0f, 0.0f, 0.0f };
	float dist = 0;
	bool contact_sphere_sphere = false;
	bool contact_sphere_capsules = false;
	bool contact_aabb_aabb = false;
	bool contact_obb_obb = false;
	bool contact_aabb_ray = false;

	//Random Number
	float2 range_demo; // for the random generation demo
	float2 range_demo1;
	int quantity_demo = 1; // number of random generations of the demo

	SDL_version compiled_version;
	char  name_input_buffer[255];
	char  organization_input_buffer[255];
	char  version_input_buffer[255];
	int   max_fps = 0;

	ImGuiTextBuffer debug_console_buff;

private:
	
	void AppAbout();
	void DebugConsole();

	void AppInfo();

	void LoadStyle(const char * name);

	//HardWare
	void HardwareInfo();

	std::string GetHardwareInfo();

	hardware_info info1;

	void UpdateVRAMInfo();

	//Random Number
	void RandomGenerator();
	void GenerateRandomNumbers(float2 range, float2 range1, int quantity);
	float GetFloatRandomValue(float range_f1, float range_f2);
	int GetIntRandomValue(float range_f1, float range_f2);

	//MathGeo
	void MathGeoTest();

};