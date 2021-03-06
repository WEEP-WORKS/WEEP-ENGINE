#pragma once

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "glew/glew.h"
#include "ComponentUIImage.h"

#define MAX_SNAKE 2
#define LIB_NUM 11

struct hardware_info {
	const char* gpu_vendor = 0;
	const char* gpu_device = 0;

	int vram_mb_available = 0;
	int vram_mb_dedicated = 0;
	int vram_mb_current = 0;
	int vram_mb_evicted = 0;
};

class DebugScene : public Module
{
public:
	DebugScene(bool start_enabled = true);
	~DebugScene();

	bool Awake();
	bool Start();
	void init2d();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool ButtonEvent(const UIButtonType type) override;
	bool CheckBoxEvent(const UICheckBoxType type, const bool is_clicked) override;


	bool PostUpdate(float dt);

	void Tools();
	bool CleanUp();

	void Configuration();
	void OnConfiguration();
	void SetFpsMax();

	void ConsoleLog(const char * text);

public:
	bool show_hierarchy = true;
	bool frame_passed = false;

	bool window_hvr = false;
	bool about_hvr = false;
	bool config_hvr = false;
	bool menubar_hvr = false;
	bool tools_hvr = false;
	bool mathgeo_hvr = false;
	bool random_hvr = false;
	bool resource_hvr = false;

private:

	bool show_inspector = true;
	bool show_app_about = false;
	bool show_demo_window = false;
	bool show_geometry_math_test = false;
	bool show_random_generator = false;
	bool show_debug_console = true;
	bool show_resources = true;

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
	int   max_fps_slider = 0;

	ImGuiTextBuffer debug_console_buff;

	ComponentUIImage* bg_img = nullptr;
	ComponentUIImage* wndw_img = nullptr;
	ComponentUIImage* crsshair_img = nullptr;
	bool wndw_menu = false;

private:
	
	void PrintResourceList(const char * path);
	void Panels();
	void MenuBar(bool &ret);
	void Plane();
	void AppAbout();
	void DebugConsole();

	void AppInfo();

	void LoadStyle(const char * name);

	//HardWare
	void HardwareInfo();

	std::string GetHardwareInfo();

	hardware_info info1;

	//void UpdateVRAMInfo();

	void UpdateVRAMInfo1();

	//Random Number
	void RandomGenerator();
	void GenerateRandomNumbers(float2 range, float2 range1, int quantity);
	float GetFloatRandomValue(float range_f1, float range_f2);
	int GetIntRandomValue(float range_f1, float range_f2);

	//MathGeo
	void MathGeoTest();

};