#include "ModuleGameObjectManager.h"

#include "Globals.h"
#include "GameObject.h"
GameObjectManager::GameObjectManager(bool start_enabled) : Module(start_enabled)
{
	SetName("GameObjectManager");
}


bool GameObjectManager::Update() //dt?
{
	for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
	{
		(*item)->Update();
	}

	return true;
}

bool GameObjectManager::CleanUp()
{
	for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
	{
		RELEASE(*item);
	}
	objects.clear();
	return true;
}

void GameObjectManager::AddObject(GameObject* object)
{
	objects.push_back(object);
}