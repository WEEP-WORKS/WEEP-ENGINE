#include "ModuleGameObjectManager.h"

#include "Globals.h"
#include "GameObject.h"
#include "imgui.h"

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

	Hierarchy();

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

void GameObjectManager::Hierarchy() {
	if (ImGui::Begin("Hierarchy"))
	{
		for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
		{
			ImGui::Text("GameObject(num list)"); //should be able to change the text
		}
	}
	ImGui::End();
}