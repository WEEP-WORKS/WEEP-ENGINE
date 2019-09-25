#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "DebugScene.h"
#include <list>

Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	debug_scene = new DebugScene();

	// The order of calls is very important!
	// Modules will Awake() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
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