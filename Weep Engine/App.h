#pragma once

#include "Globals.h"
#include "Profiler.h"
#include "Module.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"

class Profiler;

class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleImporter;
class DebugScene;
class GameObjectManager;
class SceneManager;
class ModuleTexture;
class ModuleQuadtree;

class JsonHelper;

class ModuleFileSystem;



class Application
{
public:
	Application(int argc, char* args[]);
	~Application();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();
	void CloseApp();

	int GetArgc() const;
	const char* GetArgv(int index) const;
	float GetDT();

	
	void SetMaxFps(int set);
	int GetMaxFps();

	void LoadFile(const char * filepath);

	std::string GetFilePath(const char * file_path);
	std::string GetFileName(const char * file_path);
	std::string GetFileNameWithoutExtension(const char * file_path);
	string GetFileExtension(const char * file_name);
	string ToLowerCase(std::string str);

	void OpenWeb(string web);
	const char* GetBasePath();
	void OpenFolder(const char* folder);

	void WantToSave();
	void WantToLoad();

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveAll();

	void LoadAll();

public:
	//Modules
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	DebugScene*        debug_scene = nullptr;
	GameObjectManager* game_object_manager = nullptr;
	ModuleImporter* importer = nullptr;
	ModuleTexture*	texture = nullptr;
	ModuleFileSystem* file_system = nullptr;
	ModuleQuadtree* quadtree = nullptr;
	SceneManager*	   scene_manager = nullptr;

	Profiler*		   profiler = nullptr;

	LCG*	random = nullptr;

	JsonHelper* json_helper = nullptr;
		
	std::list<Module*>      modules;

private:


	int				   argc;
	char**			   args;

	bool close_app = false;

	bool want_to_save = false;
	bool want_to_load = false;

};

extern Application* App;