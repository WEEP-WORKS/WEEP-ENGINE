	#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "DebugScene.h"
#include "ModuleGameObjectManager.h"
#include "ModuleImporter.h"
#include "ModuleTexture.h"
#include "JsonHelper.h"
#include "ModuleFileSystem.h"
#include "ModuleQuadtree.h"
#include "SceneManager.h"

#include <list>
#include <fstream>
#include <iostream>

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{

	random = new math::LCG();
	json_helper = new JsonHelper();

	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	debug_scene = new DebugScene();
	game_object_manager = new GameObjectManager();
	importer = new ModuleImporter();
	texture = new ModuleTexture();
	file_system = new ModuleFileSystem();
	quadtree = new ModuleQuadtree();
	scene_manager = new SceneManager();

	// The order of calls is very important!
	// Modules will Awake() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(file_system);
	AddModule(quadtree);
	AddModule(importer);
	AddModule(texture);
	AddModule(game_object_manager);
	AddModule(scene_manager);
	AddModule(debug_scene);


	// Renderer last
	AddModule(renderer3D);

	profiler = new Profiler();
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

void Application::LoadFile(const char * filepath)
{
	string path = GetFilePath(filepath);
	string name = GetFileName(filepath);
	string extension = ToLowerCase(GetFileExtension(name.c_str()));

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		(*it)->OnLoadFile(path.c_str(), name.c_str(), extension.c_str());
	}
}

std::string Application::GetFilePath(const char * file_path)
{
	string ret;

	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\')
		{
			ret += '/';
			continue;
		}

		ret += file_path[i];
	}

	return ret;
}

std::string Application::GetFileName(const char * file_path)
{
	string ret;

	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			ret.clear();
			continue;
		}

		ret += file_path[i];
	}

	return ret;
}

std::string Application::GetFileNameWithoutExtension(const char * file_path)
{
	string ret;
	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			ret.clear();
			continue;
		}

		else if (file_path[i] == '.')
			return ret;

		ret += file_path[i];
	}

	return ret;
}

string Application::GetFileExtension(const char * file_name)
{
	string ret;

	bool adding = false;
	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.')
		{
			ret.clear();
			adding = true;
			continue;
		}

		if (adding)
			ret += file_name[i];
	}

	return ret;
}

string Application::ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}

	return str;
}
