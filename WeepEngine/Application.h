#pragma once

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"

class Application
{
public:
	Application(int argc, char* args[]);
	~Application();

	bool Init();
	bool Start();
	bool Update();
	bool CleanUp();

	int GetArgc() const;
	const char* GetArgv(int index) const;
	float GetDT();
	float GetFps();
	float GetAvgFps();
	int GetFramesSinceStart();


private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void FrameCalculations();

public:
	//Modules
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleAudio*       audio = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	ModulePhysics3D*   physics = nullptr;
	ModuleSceneIntro*  scene_intro = nullptr;

private:
	list<Module*>      modules;

	int				   argc;
	char**			   args;

	string		       title;
	string		       organization;

	// Timers
	int				   capped_ms = -1;
	PerfTimer		   ptimer;
	int				   frame_count = 0;
	Timer			   startup_time;
	Timer			   frame_time;
	Timer			   last_sec_frame_time;
	int				   last_sec_frame_count = 0;
	int				   prev_last_sec_frame_count = 0;
	float		   	   dt = 0.0f;
	float			   avg_fps = 0;
	float			   seconds_since_startup = 0;
	float			   last_frame_ms = 0;
	float			   frames_on_last_update = 0;

};