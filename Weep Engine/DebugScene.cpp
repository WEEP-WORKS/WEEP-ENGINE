#include "Globals.h"
#include "App.h"
#include "DebugScene.h"
#include <cmath>
#include "imgui.h"

#include "ModuleWindow.h"
#include "SDL/include/SDL_opengl.h"


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
			if (ImGui::MenuItem("Exit"))
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

		ImGui::EndMainMenuBar();
	}

	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	return ret;
}

void DebugScene::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
}