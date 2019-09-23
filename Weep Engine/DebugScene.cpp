#include "Globals.h"
#include "App.h"
#include "DebugScene.h"
#include <cmath>
#include "imgui.h"

#include "ModuleWindow.h"
#include "SDL/include/SDL_opengl.h"

#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\MathGeoLib.h"

#include <random>
#include "pcg_random.hpp"


DebugScene::DebugScene(bool start_enabled) : Module( start_enabled)
{}

DebugScene::~DebugScene()
{}

bool DebugScene::Awake()
{
	bool ret = true;

	SetName("DebugScene");

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

	return true;
}

// Load assets
bool DebugScene::CleanUp()
{
	bool ret = true;

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
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				ret = false;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu ("Window"))
		{
			ImGui::Checkbox("Test Window", &show_demo_window);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug") && App->GetDebugMode())
		{
			ImGui::MenuItem("Geometry Math Test", NULL, &show_geometry_math_test);
			ImGui::MenuItem("RandomNumber Generator", NULL, &show_random_generator);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About Weep Engine", NULL, &show_app_about);
			ImGui::EndMenu();
		}

		ImGui::Text("Fps: %f", App->GetFps());

		ImGui::EndMainMenuBar();
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
	if (show_random_generator)
	{
		RandomGenerator();
	}

	if (show_geometry_math_test)
	{
		MathGeoTest();
	}

	return ret;
}

void DebugScene::AppAbout()
{
	ImGui::Begin("About Weep Engine", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Weep Engine. v.0.1");
	ImGui::Separator();
	ImGui::Text("By Jorge Gemas and Lluis Moreu.");
	ImGui::Text("This Engine made for educational porpouses on the Game Engines' subject during the 3rd year of Video Game Design and Development degree at CITM, Terrassa");
	ImGui::Text("Weep Engine is licensed under the MIT License, see LICENSE for more information.");
	if (ImGui::Button("Github Repository")) {
		App->OpenWeb("https://github.com/WEEP-WORKS/WEEP-ENGINE");
	}
	if (ImGui::Button("Download Latest Release")) {
		App->OpenWeb("https://github.com/WEEP-WORKS/WEEP-ENGINE/releases");
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
