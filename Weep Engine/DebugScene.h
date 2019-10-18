#pragma once

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
//#include "GeometryShape.h"
#include "glew/glew.h"

#define MAX_SNAKE 2
#define LIB_NUM 10

#define checkImageWidth 64
#define checkImageHeight 64

#include "par_shapes.h"

struct hardware_info {
	const char* gpu_vendor = 0;
	const char* gpu_device = 0;

	int vram_mb_available = 0.f;
	int vram_mb_dedicated = 0.0f;
	int vram_mb_current = 0.0f;
	int vram_mb_evicted = 0.0f;
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
	void Panels();
	void MenuBar(bool &ret);
	void CubeElementaArrayMode();
	void CubeDrawArrayMode();
	void Plane();
	void CubeDirectMode();
	void DrawCircle();
	void Configuration();
	bool CleanUp();
	void OnConfiguration();

	void SetFpsMax();

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
	int   max_fps_slider = 0;

	ImGuiTextBuffer debug_console_buff;

	//Cubes
	uint my_id = 0;
	uint my_id1 = 2;
	uint my_indices = 1;
	uint uv_id = 4;
	uint num_vertices = 36;
	uint number_of_indices = 36;

	// Sphere
	std::vector<float> vertices;
	std::vector<uint>  indices;

	/*//Shapes
	GeometrySphere* sphere;
	GeometrySphere* sphere2;*/

	//Textures
	GLuint texName;
	GLuint Texture;

private:
	
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