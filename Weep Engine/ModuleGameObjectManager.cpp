#include "ModuleGameObjectManager.h"
#include "App.h"
#include "Globals.h"
#include "GameObject.h"
#include "imgui.h"
#include "ModuleInput.h"

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

void GameObjectManager::AddGameObjectToSelected(GameObject * go)
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		if ((*it) == go)
			return;
	}

	go->SetSelected(true);
	selected.push_back(go);
}

void GameObjectManager::ClearSelection()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end();)
	{
		(*it)->SetSelected(false);
		it = selected.erase(it);
	}
}

const vector<GameObject*> GameObjectManager::GetSelectedGameObjects() const
{
	return selected;
}


void GameObjectManager::Hierarchy() {
	if (ImGui::Begin("Hierarchy"))
	{
		for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
		{
			PrintGoList((*item));
		}
	}
	ImGui::End();
}


void GameObjectManager::PrintGoList(GameObject * object)
{
	if (object == nullptr)
		return;

	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (object->GetSelected())
		flags |= ImGuiTreeNodeFlags_Selected;

	//treenode needs to be more understood
	bool opened = ImGui::TreeNodeEx(object->GetName(), flags);

	// Input
	if (ImGui::IsItemClicked(0))
	{
		//If ctrl is pressed do multiselection
		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		{
			AddGameObjectToSelected(object);
		}

		// If shift is pressed do fill gap selection
		else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{

		}

		// Monoselection
		else
		{
			ClearSelection();
			AddGameObjectToSelected(object);
		}
	}
}