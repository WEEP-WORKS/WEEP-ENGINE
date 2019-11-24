#include "SceneManager.h"
#include "App.h"
#include "Profiler.h"
#include "JsonHelper.h"
#include <list>
#include <fstream>
#include <iostream>

SceneManager::SceneManager(bool start_enabled) : Module(start_enabled)
{
	SetName("SceneManager");
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	bool ret = true;

	if (gamemode)
		state = PLAY;
	else
		state = EDIT;

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	return ret;
}

void SceneManager::Save(Json::Value& root) const
{
	root[GetName()]["Gamemode"] = gamemode;
}

void SceneManager::Load(const Json::Value& root)
{
	gamemode = root[GetName()]["Gamemode"].asBool();
}

void SceneManager::SaveTmpScene()
{
	LOG("LOADING TMP SCENE");

	Json::Value new_root;
	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	for (list<Module*>::iterator it = App->modules.begin(); it != App->modules.end(); it++)
	{
		(*it)->Save(new_root);
	}


	std::ofstream outputFileStream("TmpScene.json");

	writer->write(new_root, &outputFileStream);

}

void SceneManager::LoadTmpScene()
{	
	LOG("SAVING TMP SCENE");

	std::ifstream file_input("TmpScene.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(file_input, root);

	for (list<Module*>::iterator it = App->modules.begin(); it != App->modules.end(); it++)
	{
		(*it)->Load(root);
	}
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