#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"

class GameObject;

enum SCENE_STATE
{
	EDIT,
	PLAY
};

class SceneManager : public Module
{
public:
	SceneManager(bool start_enabled = true);
	virtual ~SceneManager();

	bool Start();
	bool CleanUp();

	void SaveTmpScene();
	void LoadTmpScene();

	void Save(Json::Value&) const override;
	void Load(const Json::Value&) override;

	SCENE_STATE GetState();
	bool GetPause();
	bool GetStep();

	void Edit();
	void Play();
	void Pause();
	void Step();

	bool	   pause = false;

private:
	SCENE_STATE state;
	//bool	   pause = false;
	bool	   step = false;
	string	   current_scene;

	bool gamemode = false;
};

#endif
