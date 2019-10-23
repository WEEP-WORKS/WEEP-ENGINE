#include "ModuleGameObjectManager.h"
#include "App.h"
#include "Globals.h"
#include "GameObject.h"
#include "imgui.h"
#include "ModuleInput.h"
#include "ComponentMesh.h"
#include "par_shapes.h"
#include "ModuleTexture.h"
#include "DebugScene.h"

GameObjectManager::GameObjectManager(bool start_enabled) : Module(start_enabled)
{
	SetName("GameObjectManager");
}


bool GameObjectManager::Update() //dt?
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)  //with imgui do this when activate the option to the game object selected or something like that.
	{
		SetCheckersToGOSelected();
	}
	for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
	{
		(*item)->Update();
	}

	Hierarchy();

	return true;
}

void GameObjectManager::SetCheckersToGOSelected()
{
	for (std::vector<GameObject*>::iterator iter = App->game_object_manager->selected.begin(); iter != App->game_object_manager->selected.end(); ++iter)
	{
		if ((*iter)->GetMesh()->GetTexture() != App->texture->GetCheckersTexture())
		{
			(*iter)->GetMesh()->SetTexture(App->texture->GetCheckersTexture());
		}
		else
		{
			(*iter)->GetMesh()->SetTexture((*iter)->GetTextureActivated());
		}
	}
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

GameObject* GameObjectManager::CreateGeometryShape(int sides)
{
	GameObject* ret = new GameObject();

	
	return ret;
}

void GameObjectManager::CreateCube()
{
	GameObject* ret = new GameObject();
	par_shapes_mesh* mesh = par_shapes_create_cube();
	ComponentMesh* cmesh = (ComponentMesh*)ret->AddComponent(ComponentType::MESH);

	if (mesh != nullptr)
	{
		cmesh->vertexs.has_data = true;
		cmesh->vertexs.buffer = mesh->points;
		cmesh->vertexs.num = mesh->npoints;
		cmesh->vertexs.buffer_size = (cmesh->vertexs.num * 3/*num of coordinates by vertex*/);

		cmesh->indexs.has_data = true;
		cmesh->indexs.buffer = mesh->triangles;
		cmesh->indexs.num = mesh->ntriangles;
		cmesh->indexs.buffer_size = (cmesh->indexs.num * 3);

		cmesh->SetBuffersWithData();
	}



	AddObject(ret);
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

// vector<GameObject*> GameObjectManager::GetSelectedGameObjects() const
//{
//	return selected;
//}


void GameObjectManager::Hierarchy()
{
	if (App->debug_scene->activate_hierarchy)
	{
		ImGui::SetNextWindowSize(ImVec2(310, 984), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 22), ImGuiCond_::ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Hierarchy",NULL,ImGuiWindowFlags_NoSavedSettings))
		{
			for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
			{
				PrintGoList((*item));
			}
		}
		ImGui::End();
	}

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
	if (opened)
	{
		ImGui::TreePop();
	}
}