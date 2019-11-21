#include "SceneManager.h"
#include "App.h"

SceneManager::SceneManager(bool start_enabled) : Module(start_enabled)
{
	SetName("Scene Manager");
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
	pause = false;

	if (state == PLAY)
	{
		pause = true;
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

float SceneManager::GetGameDT()
{
	float ret = 0.0f;

	if (pause)
		ret = 0.0f;
	else
		ret = App->GetDT();

	return ret;
}

float SceneManager::GetGameExecutionTime()
{
	return 0.0f;
}