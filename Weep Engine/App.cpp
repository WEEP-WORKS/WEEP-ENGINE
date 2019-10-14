#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "DebugScene.h"
#include "ModuleGameObjectManager.h"
#include "ModuleImporter.h"
#include "ModuleTexture.h"

#include <list>
#include <fstream>
#include <iostream>

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	debug_scene = new DebugScene();
	game_object_manager = new GameObjectManager();
	importer = new ModuleImporter();
	texture = new ModuleTexture();

	// The order of calls is very important!
	// Modules will Awake() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(importer);
	AddModule(texture);
	AddModule(game_object_manager);
	AddModule(debug_scene);



	// Renderer last
	AddModule(renderer3D);

	profiler = new Profiler();
	SetDebugMode(true);
}

Application::~Application()
{
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		RELEASE(*it);
	}
}

bool Application::Awake()
{
	bool ret = true;

	LoadAll(); //Load all setings for the first time

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->Awake();
	}

	profiler->AwakeTime();

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->Start();
		if (!ret) return false;
	}

	profiler->StartTime();

	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules

void Application::PrepareUpdate()
{

}

bool Application::Update()
{
	bool ret = true;

	if (input->GetWindowEvent(WE_QUIT) == true || close_app)
		return false;

	// Cap fps
	if (App->profiler->capped_ms > 0 && GetDT() < App->profiler->capped_ms)
	{
		SDL_Delay(App->profiler->capped_ms - GetDT());
	}

	PrepareUpdate();
	
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		ret = (*it)->PreUpdate();

		if (!ret) return false;
	}
	
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		profiler->StartProfile((*it)->name);

		ret = (*it)->Update(); 

		profiler->FinishProfile();

		if (!ret) return false;
	}
	
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		ret = (*it)->PostUpdate(); 

		if (!ret) return false;
	}
	
	FinishUpdate();

	profiler->UpdateFinish();

	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	//---- save ----

	if (want_to_save)
	{
		SaveAll();
	}

	//---- Load -----

	if (want_to_load)
	{
		LoadAll();
	}
}

void Application::SaveAll()
{
	Json::Value new_root;
	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		(*it)->Save(new_root);
	}


	std::ofstream outputFileStream("SaveFile.json");

	writer->write(new_root, &outputFileStream);

	want_to_save = false;
}

void Application::LoadAll()
{
	std::ifstream file_input("SaveFile.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(file_input, root);

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		(*it)->Load(root);
	}

	want_to_load = false;

}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++)
	{
		ret = (*it)->CleanUp();
	}

	profiler->CleanUp();

	return ret;
}

void Application::CloseApp()
{
	close_app = true;
}

int Application::GetArgc() const
{
	return argc;
}

const char * Application::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

float Application::GetDT()
{
	return profiler->GetFrameTime() / 1000;
}

bool Application::GetDebugMode()
{
	return debug_mode;
}

void Application::SetDebugMode(bool set)
{
	debug_mode = set;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}

void Application::OpenWeb(string web)
{
	ShellExecute(NULL, "open", web.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}

const char * Application::GetBasePath()
{
	return SDL_GetBasePath();
}

void Application::OpenFolder(const char * folder)
{
	ShellExecute(NULL, "open", folder, NULL, NULL, SW_SHOWMAXIMIZED);
}

void Application::WantToSave()
{
	want_to_save = true;
}

void Application::WantToLoad()
{
	want_to_load = true;
}	

void Application::SetMaxFps(int set)
{
	/*if (!App->renderer3D->GetVsync())
	{
		set = 60;
	}
*/
	App->profiler->max_fps = set;
	App->profiler->capped_ms = (1000 / set);
	
}

int Application::GetMaxFps()
{
	return App->profiler->max_fps;
}	

