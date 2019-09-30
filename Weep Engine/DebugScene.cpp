#include "Globals.h"
#include "App.h"
#include "DebugScene.h"
#include "ModuleRenderer3D.h"
#include <cmath>
#include "imgui.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_opengl.h"

#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\MathGeoLib.h"

#include <random>
#include "pcg_random.hpp"

#include "mmgr\mmgr.h" //must be after random !!!!!!

DebugScene::DebugScene(bool start_enabled) : Module( start_enabled)
{

	SetName("DebugScene");

	memset(name_input_buffer, 0, sizeof(name_input_buffer));
	memset(organization_input_buffer, 0, sizeof(organization_input_buffer));
	memset(version_input_buffer, 0, sizeof(version_input_buffer));

	info1.gpu_vendor = (const char*)glGetString(GL_VENDOR);
	info1.gpu_device = (const char*)glGetString(GL_RENDERER);
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &info1.vram_mb_dedicated);
	info1.vram_mb_dedicated /= 1024;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &info1.vram_mb_available);
	info1.vram_mb_available /= 1024;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &info1.vram_mb_current);
	info1.vram_mb_current /= 1024;
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &info1.vram_mb_evicted);
	info1.vram_mb_evicted /= 1024;

}

DebugScene::~DebugScene()
{}

bool DebugScene::Awake()
{
	bool ret = true;

	LoadStyle("green_purple");

	SDL_VERSION(&compiled_version);

	// Initial range set
	range_demo.x = 0;
	range_demo.y = 1;

	range_demo1.x = 1;
	range_demo1.y = 6;

	return ret;
}

// Load assets
bool DebugScene::Start()
{
	bool ret = true;

	strcpy(name_input_buffer, App->window->GetTitle().c_str());
	strcpy(organization_input_buffer, App->window->GetAppOrganization());
	strcpy(version_input_buffer, App->window->GetVersion().c_str());

	if (!App->renderer3D->GetVsync())
	{
		max_fps = App->GetMaxFps();
	}
	else
	{
		max_fps = App->renderer3D->GetRefreshRate();
	}

	return true;
}

// Load assets
bool DebugScene::CleanUp()
{
	bool ret = true;

	return ret;
}

bool DebugScene::PreUpdate() 
{
	bool ret = true;


	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && show_app_configuration == false) {
		App->debug_scene->show_app_configuration = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && show_app_configuration == true) {
		App->debug_scene->show_app_configuration = false;
	}

	return ret;
}

bool DebugScene::Update()
{
	bool ret = true;

	/*Plane p(0, 10, 0, 0);
	p.axis = true;
	p.Render();*/

	if (ImGui::BeginMainMenuBar()) 
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "Alt+F4 || ESC"))
			{
				ret = false;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu ("Window"))
		{
			ImGui::MenuItem("Configuration", "LShift+P", &show_app_configuration);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug") && App->GetDebugMode())
		{
			ImGui::MenuItem("Debug Console", NULL, &show_debug_console);
			ImGui::MenuItem("Test Window", NULL, &show_demo_window);
			ImGui::MenuItem("Geometry Math Test", NULL, &show_geometry_math_test);
			ImGui::MenuItem("RandomNumber Generator", NULL, &show_random_generator);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About Weep Engine", NULL, &show_app_about);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Save", NULL, &to_save);
			ImGui::MenuItem("Load", NULL, &to_load);
			ImGui::EndMenu();
		}

		ImGui::Text("Fps: %d", App->profiler->GetFPS());

		ImGui::EndMainMenuBar();
	}

	//Configuration
	if (show_app_configuration)
	{
		Configuration();
	}

	//Console
	if (show_debug_console)
	{
		DebugConsole();
	}

	//Test
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	// About
	if (show_app_about)
	{
		AppAbout();
	}

	// Geometry debug
	if (show_geometry_math_test)
	{
		MathGeoTest();
	}


	// Random Number
	if (show_random_generator)
	{
		RandomGenerator();
	}

	// Save
	if (to_save)
	{
		App->WantToSave();
	}

	// Load
	if (to_load)
	{
		App->WantToLoad();
	}

	return ret;
}

void DebugScene::Configuration()
{
	ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Configuration", &show_app_configuration, ImGuiWindowFlags_NoSavedSettings))
	{
		if (ImGui::CollapsingHeader("App"))
			AppInfo();

		for (list<Module*>::iterator it = App->modules.begin(); it != App->modules.end(); it++)
		{
			if ((*it)->name != "Camera") 
			{
				if (ImGui::CollapsingHeader((*it)->name))
				{
					(*it)->OnConfiguration();
				}
			}
		}

		if (ImGui::CollapsingHeader("Hardware"))
			HardwareInfo();

		ImGui::End();
	}

}

void DebugScene::OnConfiguration()
{
	char title[25];
	std::vector<float> framerate = App->profiler->GetFramesVector();
	sprintf_s(title, 25, "Framerate %.1f", framerate[framerate.size() - 1]);
	ImGui::PlotHistogram("##Framerate", &framerate[0], framerate.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	ImGui::SameLine();
	std::vector<float> milliseconds = App->profiler->GetMillisecondsVector();
	sprintf_s(title, 25, "Milliseconds %.1f", milliseconds[milliseconds.size() - 1]);
	ImGui::PlotHistogram("##Framerate", &milliseconds[0], milliseconds.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

	std::vector<float> memory = App->profiler->GetMemoryVector();
	if (!memory.empty())
	{
		char title[25];
		sprintf_s(title, 25, "%.1f", memory[memory.size() - 1]);
		ImGui::PlotHistogram("##Memory", &memory[0], memory.size(), 0, title, 0.0f, 30000.0f, ImVec2(0, 100));
	}

	ImGui::Text("Total Reported Mem: %d", m_getMemoryStatistics().totalReportedMemory);
	ImGui::Text("Peak Reported Mem: %d", m_getMemoryStatistics().peakReportedMemory);
	ImGui::Text("Total Actual Mem: %d", m_getMemoryStatistics().totalActualMemory);
	ImGui::Text("Peak Actual Mem: %d", m_getMemoryStatistics().peakActualMemory);
	ImGui::Text("Accumulated Reported Mem: %d", m_getMemoryStatistics().accumulatedReportedMemory);
	ImGui::Text("Accumulated Actual Mem: %d", m_getMemoryStatistics().accumulatedActualMemory);
	ImGui::Text("Accumulated Alloc Unit Count: %d", m_getMemoryStatistics().accumulatedAllocUnitCount);
	ImGui::Text("Total Alloc Unit Count: %d", m_getMemoryStatistics().totalAllocUnitCount);
	ImGui::Text("Peak Alloc Unit Count: %d", m_getMemoryStatistics().peakAllocUnitCount);
}

void DebugScene::DebugConsole()
{
	ImGui::BeginChild("Console Log");
	ImGui::TextUnformatted(debug_console_buff.begin());
	ImGui::EndChild();
}

void DebugScene::ConsoleLog(const char* text)
{
	debug_console_buff.appendf(text);
}

void DebugScene::AppInfo()
{
	if (ImGui::InputText("App Name", name_input_buffer, 254, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		App->window->SetAppName(name_input_buffer);
	}

	if (ImGui::InputText("Organization", organization_input_buffer, 254, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		App->window->SetAppOrganization(organization_input_buffer);
	}

	if (ImGui::InputText("Version", version_input_buffer, 254, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		App->window->SetVersion(version_input_buffer);
	}

	if (App->renderer3D->GetVsync())
	{
		max_fps = App->renderer3D->GetRefreshRate();
	}

	if (ImGui::SliderInt("Max FPS (VSYNC ON)", &max_fps, 0, 999))
	{
		App->SetMaxFps(max_fps);
	}
	
}

void DebugScene::HardwareInfo()
{

	UpdateVRAMInfo1();


	ImGui::Text("SDL Version:");
	ImGui::SameLine();
	std::string temp = std::to_string(compiled_version.major) + "." + std::to_string(compiled_version.minor) + "." + std::to_string(compiled_version.patch);
	ImGui::TextColored({ 0, 1.0f, 1.0f, 1.0f }, temp.c_str());

	ImGui::Separator();

	ImGui::Text("CPU cores:");
	ImGui::SameLine();
	temp = std::to_string(SDL_GetCPUCount()) + " (Cache: " + std::to_string(SDL_GetCPUCacheLineSize()) + "Kb)";
	ImGui::TextColored({ 0, 1.0f, 1.0f, 1.0f }, temp.c_str());

	ImGui::Text("System RAM:");
	ImGui::SameLine();
	temp = std::to_string(SDL_GetSystemRAM()) + "Mb";
	ImGui::TextColored({ 0, 1.0f, 1.0f, 1.0f }, temp.c_str());

	ImGui::Text("Caps:");
	ImGui::SameLine();
	ImGui::TextColored({ 0, 1.0f, 1.0f, 1.0f }, GetHardwareInfo().c_str());

	ImGui::Separator();

	ImGui::Text("GPU Brand:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), info1.gpu_vendor);

	ImGui::Text("Device: ");						
	ImGui::SameLine();		
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), info1.gpu_device);

	ImGui::Text("Video Memory Dedicated: ");				
	ImGui::SameLine();		
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i Mb", info1.vram_mb_dedicated);

	ImGui::Text("Video Memory Available: ");		
	ImGui::SameLine();		
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i Mb", info1.vram_mb_available);

	ImGui::Text("Video Memory Current: ");	
	ImGui::SameLine();		
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i Mb", info1.vram_mb_current);

	ImGui::Text("Video Memory Evicted: ");		
	ImGui::SameLine();		
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i Mb", info1.vram_mb_evicted);
}

std::string DebugScene::GetHardwareInfo()
{
	std::string info;

		if (SDL_Has3DNow())
			info.append("3DNow, ");
		if (SDL_HasAVX())
			info.append("AVX, ");
		if (SDL_HasAVX2())
			info.append("AVX2, ");
		if (SDL_HasMMX())
			info.append("MMX, ");
		if (SDL_HasRDTSC())
			info.append("RDTSC, ");
		if (SDL_HasSSE())
			info.append("SSE, ");
		if (SDL_HasSSE2())
			info.append("SSE2, ");
		if (SDL_HasSSE3())
			info.append("SSE3, ");
		if (SDL_HasSSE41())
			info.append("SSE41, ");
		if (SDL_HasSSE42())
			info.append("SSE42, ");
		return info;
}

void DebugScene::UpdateVRAMInfo1()
{
	info1.gpu_vendor = (const char*)glGetString(GL_VENDOR);
	info1.gpu_device = (const char*)glGetString(GL_RENDERER);
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &info1.vram_mb_dedicated);
	info1.vram_mb_dedicated /= 1024;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &info1.vram_mb_available);
	info1.vram_mb_available /= 1024;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &info1.vram_mb_current);
	info1.vram_mb_current /= 1024;
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &info1.vram_mb_evicted);
	info1.vram_mb_evicted /= 1024;

}

void DebugScene::AppAbout()
{
	ImGui::Begin("About Weep Engine", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text(App->window->GetTitleWithVersion().c_str());
	ImGui::Separator();
	ImGui::Text("By Jorge Gemas and Lluis Moreu.");
	ImGui::Text("This Engine made for educational porpouses on the Game Engines' subject during the 3rd year of Video Game Design and Development degree at CITM, Terrassa");
	if (ImGui::Button("Jorge's Github")) {
		App->OpenWeb("https://github.com/https://github.com/jorgegh2");
	}
	ImGui::SameLine();
	if (ImGui::Button("Lluis' Github")) {
		App->OpenWeb("https://github.com/youis11");
	}
	ImGui::SameLine();
	if (ImGui::Button("Github Repository")) {
		App->OpenWeb("https://github.com/WEEP-WORKS/WEEP-ENGINE");
	}
	ImGui::SameLine();
	if (ImGui::Button("Download Latest Release")) {
		App->OpenWeb("https://github.com/WEEP-WORKS/WEEP-ENGINE/releases");
	}
	ImGui::SameLine();
	if (ImGui::Button("Report Issue"))
	{
		App->OpenWeb("https://github.com/WEEP-WORKS/WEEP-ENGINE/issues");
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Libraries"))
	{
		ImGui::Columns(3, "Name");
		ImGui::Separator();
		ImGui::Text("Use"); ImGui::NextColumn();
		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("Version"); ImGui::NextColumn();
		ImGui::Separator();
		const char* use[LIB_NUM] = { "Graphics", "Graphics", "Math", "Random Number Generator", "UI", "File System", "OpenGL Supporter" , "Memory Tracker"};
		const char* name[LIB_NUM] = { "SDL", "OpenGL", "MathGeoLib", "PCG", "ImGui", "JSonCpp", "Glew" , "mmgr"};
		const char* version[LIB_NUM] = { "v2.0", "v.3._", "v1.5", "v.0.98" ,"v1.72b", "v1.9.1", "v2.1.0", "---"};
		static int selected = -1;
		for (int i = 0; i < LIB_NUM; i++)
		{
			ImGui::Text(use[i]); ImGui::NextColumn();
			ImGui::Text(name[i]); ImGui::NextColumn();
			ImGui::Text(version[i]); ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
	}
	if (ImGui::CollapsingHeader("License"))
	{
		ImGui::Text("MIT License");
		ImGui::Text("Copyright(c) 2019 WEEP-WORKS");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and / or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :");

		ImGui::TextColored({ 0.8f, 1.0f, 1.0f, 0.7f }, "The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.");

		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");
	}
	ImGui::End();
}

void DebugScene::MathGeoTest()
{
	ImGui::Begin("Geometry Math test", &show_geometry_math_test, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Contact: %s", contact ? "Yes" : "No");

	ImGui::Separator();

	ImGui::InputFloat3("Position", vec3a);
	ImGui::SliderFloat("Distance", &dist, 0, 10);

	if (ImGui::Button("Sphere - Sphere"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_sphere_sphere = !contact_sphere_sphere;
	}

	if (ImGui::Button("Sphere - Capsule"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_sphere_capsules = !contact_sphere_capsules;
	}

	if (ImGui::Button("AABB - AABB"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_aabb_aabb = !contact_aabb_aabb;
	}

	if (ImGui::Button("OBB - OBB"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_obb_obb = !contact_obb_obb;
	}

	if (ImGui::Button("AABB - Ray"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_aabb_ray = !contact_aabb_ray;
	}

	math::float3 p1 = { vec3a[0] - dist / 2, vec3a[1], vec3a[2] };
	math::float3 p2 = { vec3a[0] + dist / 2, vec3a[1], vec3a[2] };
	contact = false;

	if (contact_sphere_sphere)
	{
		// Sphere 1
		ImGui::Text("contact_sphere_sphere");

		Sphere sph1(p1, 2);

		Sphere sph2(p2, 2);

		if (sph1.Intersects(sph2))
			contact = true;
	}

	if (contact_sphere_capsules)
	{
		ImGui::Text("contact_sphere_capsules");

		Sphere sph1(p1, 2);

		Capsule c(float3(p2.x, p2.y - 1, p2.z), float3(p2.x, p2.y + 1, p2.z), 1.0f);

		if (sph1.Intersects(c))
			contact = true;
	}

	if (contact_aabb_aabb)
	{
		ImGui::Text("contact_aabb_aabb");

		AABB sph1(float3(p1.x - 1, p1.y - 1, p1.z - 1), float3(p1.x + 1, p1.y + 1, p1.z + 1));

		AABB sph2(float3(p2.x - 1, p2.y - 1, p2.z - 1), float3(p2.x + 1, p2.y + 1, p2.z + 1));

		if (sph1.Intersects(sph2))
			contact = true;
	}

	if (contact_obb_obb)
	{
		ImGui::Text("contact_obb_obb");

		AABB ab1(float3(p1.x - 1, p1.y - 1, p1.z - 1), float3(p1.x + 1, p1.y + 1, p1.z + 1));
		OBB sph1(ab1);

		AABB ab2(float3(p2.x - 1, p2.y - 1, p2.z - 1), float3(p2.x + 1, p2.y + 1, p2.z + 1));
		OBB sph2(ab2);

		if (sph1.Intersects(sph2))
			contact = true;
	}		

	if (contact_aabb_ray)
	{
		ImGui::Text("contact_aabb_ray");

		AABB sph1(float3(p1.x - 1, p1.y - 1, p1.z - 1), float3(p1.x + 1, p1.y + 1, p1.z + 1));

		Ray sph2(float3(p2.x, p2.y, p2.z - 10), float3(p2.x, p2.y, p2.z + 10));

		if (sph1.Intersects(sph2))
			contact = true;
	}

	ImGui::End();
}

void DebugScene::RandomGenerator()
{
	ImGui::Begin("Random Number Generation", &show_random_generator, ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::InputFloat2("Min | Max FLOAT", range_demo.ptr(), 2);
	ImGui::InputFloat2("Min | Max INT", range_demo1.ptr(), 2);
	ImGui::InputInt("Number of generations", &quantity_demo);

	if (ImGui::Button("Generate", { 400, 30 }))
	{
		GenerateRandomNumbers(range_demo, range_demo1, quantity_demo);
	}

	ImGui::End();
}

void DebugScene::GenerateRandomNumbers(float2 range, float2 range1, int quantity)
{
	for (int i = 0; i < quantity; ++i)
	{
		GetFloatRandomValue(range.x, range.y);
		GetIntRandomValue(range1.x, range1.y);		
	}
}

float DebugScene::GetFloatRandomValue(float range_f1, float range_f2) 
{

	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// Make a random number engine
	pcg32 rng(seed_source);

	//Choose a random mean between 0 and 1
	std::uniform_real_distribution<float> uniform_dist(range_f1, range_f2);
	float mean = uniform_dist(rng);

	LOG("float num is: %f", mean);

	//ImGui::Text("float num is: %f", mean);


	return mean;
}

int DebugScene::GetIntRandomValue(float range_i1, float range_i2) 
{
	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// Make a random number engine
	pcg32 rng(seed_source);

	// Choose a random mean between 1 and 6
	std::uniform_int_distribution<int> uniform_dist1((int)range_i1, (int)range_i2);
	int mean1 = uniform_dist1(rng);
	LOG("int num is: %i", mean1);

	//ImGui::Text("int num is: %i", mean1);

	return mean1;
}

void DebugScene::LoadStyle(const char * name)
{
	ImGuiStyle * style = &ImGui::GetStyle();

	if (name == "black_orange")
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 0.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.90f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.80f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.80f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.40f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.45f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.44f, 0.30f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.27f, 0.27f, 0.27f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.88f, 0.22f, 0.06f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.44f, 0.32f, 0.80f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.86f, 0.20f, 0.04f, 0.80f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.89f, 0.24f, 0.08f, 0.80f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.92f, 0.31f, 0.16f, 0.00f);
	}

	else if (name == "blue_yellow")
	{
		style->WindowPadding = ImVec2(6, 13);
		style->WindowRounding = 3.0f;
		style->FramePadding = ImVec2(3, 4);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 6);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 9.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 1.0f;
		style->WindowMinSize = ImVec2(10, 10);
		style->GrabMinSize = 17;

		style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.16f, 0.17f, 0.95f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.16f, 0.18f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.80f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.40f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.45f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.29f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.29f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 0.49f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.74f, 0.45f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.11f, 0.43f, 0.77f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.88f, 0.53f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.90f, 0.55f, 0.02f, 1.00f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.89f, 0.54f, 0.01f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.0f, 0.0f, 0.00f, 0.40f);
	}

	else if (name == "green_purple")
	{
	style->WindowPadding = ImVec2(6, 13);
	style->WindowRounding = 3.0f;
	style->FramePadding = ImVec2(3, 4);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 6);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 9.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 1.0f;
	style->WindowMinSize = ImVec2(10, 10);
	style->GrabMinSize = 17;

	ImVec4 darkblue = ImVec4(0.15f, 0.13f, 0.19f, 1.0f);
	ImVec4 creme = ImVec4(1.0f, 0.9f, 0.6f, 1.0f); //CMY
	ImVec4 magenta = ImVec4(0.85f, 0.36f, 0.37f, 1.0f);
	ImVec4 darkpurple = ImVec4(0.46f, 0.14f, 0.35f, 1.0f);
	ImVec4 green = ImVec4(0.78f, 0.94f, 0.0f, 1.0f);

	style->Colors[ImGuiCol_Text] = creme;
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(creme.w, creme.x, creme.y, 0.6f);
	style->Colors[ImGuiCol_ChildWindowBg] = darkblue;
	style->Colors[ImGuiCol_PopupBg] = darkblue;
	style->Colors[ImGuiCol_Border] = darkblue;
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	style->Colors[ImGuiCol_FrameBg] = darkpurple;
	style->Colors[ImGuiCol_FrameBgHovered] = darkpurple;
	style->Colors[ImGuiCol_FrameBgActive] = darkpurple;
	style->Colors[ImGuiCol_TitleBg] = darkblue;
	style->Colors[ImGuiCol_TitleBgCollapsed] = darkblue;
	style->Colors[ImGuiCol_TitleBgActive] = darkblue;
	style->Colors[ImGuiCol_MenuBarBg] = darkblue;
	style->Colors[ImGuiCol_ScrollbarBg] = darkblue;
	style->Colors[ImGuiCol_ScrollbarGrab] = darkblue;
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = darkblue;
	style->Colors[ImGuiCol_ScrollbarGrabActive] = darkblue;
	style->Colors[ImGuiCol_CheckMark] = green;
	style->Colors[ImGuiCol_SliderGrab] = green;
	style->Colors[ImGuiCol_SliderGrabActive] = green;
	style->Colors[ImGuiCol_Button] = magenta;
	style->Colors[ImGuiCol_ButtonHovered] = magenta;
	style->Colors[ImGuiCol_ButtonActive] = magenta;
	style->Colors[ImGuiCol_Header] = magenta;
	style->Colors[ImGuiCol_HeaderHovered] = darkpurple;
	style->Colors[ImGuiCol_HeaderActive] = darkpurple;
	style->Colors[ImGuiCol_Separator] = darkblue;
	style->Colors[ImGuiCol_SeparatorHovered] = darkblue;
	style->Colors[ImGuiCol_SeparatorActive] = darkblue;
	style->Colors[ImGuiCol_ResizeGrip] = magenta;
	style->Colors[ImGuiCol_ResizeGripHovered] = magenta;
	style->Colors[ImGuiCol_ResizeGripActive] = magenta;
	style->Colors[ImGuiCol_PlotLines] = green;
	style->Colors[ImGuiCol_PlotLinesHovered] = green;
	style->Colors[ImGuiCol_PlotHistogram] = green;
	style->Colors[ImGuiCol_PlotHistogramHovered] = green;
	style->Colors[ImGuiCol_TextSelectedBg] = green;
	style->Colors[ImGuiCol_ModalWindowDarkening] = green;
	}
}