#include "SceneManager.h"
#include "App.h"
#include "Profiler.h"

SceneManager::SceneManager(bool start_enabled) : Module(start_enabled)
{
	SetName("Scene Manager");

	//should it be here or in start or preupdate if it reads from json?
	if (gamemode)
		state = PLAY;
	else
		state = EDIT;
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	bool ret = true;

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	return ret;
}

void SceneManager::SaveTmpScene()
{
	//SaveScene("tmp_scene.scene");
}

void SceneManager::LoadTmpScene()
{	
	//void DestroyScene();
	//LoadScene("tmp_scene.scene");
}

SCENE_STATE SceneManager::GetState()
{
	return state;
}

bool SceneManager::GetPause()
{
	return pause;
}

bool SceneManager::GetStep()
{
	return step;
}

void SceneManager::Edit()
{
	if (state != EDIT)
	{
		state = EDIT;
		pause = false;
		step = false;

		LoadTmpScene();

		App->profiler->SetGameTime(0.0f);

	}
}

void SceneManager::Play()
{
	if (state != PLAY)
	{
		state = PLAY;

		SaveTmpScene();
	}
}

void SceneManager::Pause()
{
	//pause = false;

	//if (step)return;

	if (state == PLAY && GetPause() == false)
	{
		pause = true;
	}
	else if (GetPause() == true)
	{
		pause = false;
	}
}

void SceneManager::Step()
{
	step = false;

	if (state == PLAY)
	{
		step = true;
	}
}