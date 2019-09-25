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
class DebugScene;

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

	bool GetDebugMode();
	void SetDebugMode(bool set);

	void OpenWeb(string web);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	//Modules
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	DebugScene*        debug_scene = nullptr;

	Profiler*		   profiler = nullptr;
		
	std::list<Module*>      modules;

private:


	int				   argc;
	char**			   args;

	string		       title;
	string		       organization;

	bool		       close_app = false;

	// Engine debug info
	bool			   debug_mode = false;

};

extern Application* App;