#pragma once

#include "Globals.h"
#include "Profiler.h"
#include "Module.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Profiler;

class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleImporter;
class DebugScene;
class ShapeManager;


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

	bool GetDebugMode();
	void SetDebugMode(bool set);

	void OpenWeb(string web);

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
	ShapeManager* shape_manager = nullptr;
	ModuleImporter* importer = nullptr;

	Profiler*		   profiler = nullptr;
		
	std::list<Module*>      modules;

private:


	int				   argc;
	char**			   args;

	bool		       close_app = false;

	bool			   debug_mode = false;

	bool want_to_save = false;
	bool want_to_load = false;

};

extern Application* App;