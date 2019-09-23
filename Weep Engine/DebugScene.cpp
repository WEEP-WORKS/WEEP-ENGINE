#include "Globals.h"
#include "App.h"
#include "DebugScene.h"
#include <cmath>
#include "imgui.h"

#include "ModuleWindow.h"
#include "SDL/include/SDL_opengl.h"

#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\MathGeoLib.h"


DebugScene::DebugScene(bool start_enabled) : Module( start_enabled)
{}

DebugScene::~DebugScene()
{}

bool DebugScene::Awake()
{
	bool ret = true;

	SetName("DebugScene");

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
			ImGui::MenuItem("Geometry math test", NULL, &geometry_math_test);
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
	if (geometry_math_test)
	{
		GeometryMathTest();
	}

	return ret;
}

void DebugScene::AppAbout()
{
	ImGui::Begin("About SuSto Engine", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
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

void DebugScene::GeometryMathTest()
{
	ImGui::Begin("Geometry Math test", &geometry_math_test, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Contact: %s", contact ? "Yes" : "No");

	ImGui::Separator();

	ImGui::End();
}
