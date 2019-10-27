#include "ModuleGameObjectManager.h"
#include "App.h"
#include "Globals.h"
#include "GameObject.h"
#include "imgui.h"
#include "ModuleInput.h"
#include "ComponentMesh.h"
#include "ModuleTexture.h"
#include "ComponentTexture.h"
#include "DebugScene.h"
#include "par_shapes.h"

GameObjectManager::GameObjectManager(bool start_enabled) : Module(start_enabled)
{
	SetName("GameObjectManager");
}


bool GameObjectManager::Update() //dt?
{
	for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
	{
		if((*item)->IsActive())
			(*item)->Update();
	}

	Hierarchy();

	return true;
}

bool GameObjectManager::CleanUp()
{
	for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
	{
		(*item)->CleanUp();
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
	ret->parametric = true;
	if (mesh != nullptr)
	{
		LoadGeometryShapeInfo(cmesh, mesh);
	}
	ret->SetName("cube");
	ClearSelection();
	AddGameObjectToSelected(ret);
	AddObject(ret);
}

void GameObjectManager::CreateSphere()
{
	GameObject* ret = new GameObject();
	par_shapes_mesh* mesh = par_shapes_create_subdivided_sphere(2);
	ComponentMesh* cmesh = (ComponentMesh*)ret->AddComponent(ComponentType::MESH);

	if (mesh != nullptr)
	{
		LoadGeometryShapeInfo(cmesh, mesh);
	}
	ret->SetName("sphere");
	ClearSelection();
	AddGameObjectToSelected(ret);
	AddObject(ret);
}

void GameObjectManager::LoadGeometryShapeInfo(ComponentMesh * cmesh, par_shapes_mesh * mesh)
{
	cmesh->mesh_data->vertexs.has_data = true;
	cmesh->mesh_data->vertexs.num = mesh->npoints;
	cmesh->mesh_data->vertexs.buffer_size = (cmesh->mesh_data->vertexs.num * 3/*num of coordinates by vertex*/);
	cmesh->mesh_data->vertexs.buffer = new float[cmesh->mesh_data->vertexs.buffer_size];
	memcpy(cmesh->mesh_data->vertexs.buffer, mesh->points, sizeof(float) * cmesh->mesh_data->vertexs.buffer_size);

	cmesh->mesh_data->indexs.has_data = true;
	cmesh->mesh_data->indexs.num = mesh->ntriangles;
	cmesh->mesh_data->indexs.buffer_size = (cmesh->mesh_data->indexs.num * 3);
	cmesh->mesh_data->indexs.buffer = new uint[cmesh->mesh_data->indexs.buffer_size];
	memcpy(cmesh->mesh_data->indexs.buffer, mesh->triangles, sizeof(uint) * cmesh->mesh_data->indexs.buffer_size);


	if (cmesh->object->parametric)
	{
		//par_shapes_unweld(mesh, true);
		par_shapes_compute_normals(mesh);
	}

	if (mesh->normals != nullptr)
	{
		cmesh->mesh_data->normal_vertexs.has_data = true;
		cmesh->mesh_data->normal_faces.has_data = true;
		cmesh->mesh_data->normals_direction.has_data = true;

		cmesh->num_faces = cmesh->mesh_data->indexs.num;

		cmesh->mesh_data->normal_vertexs.num = cmesh->mesh_data->vertexs.num;
		cmesh->mesh_data->normal_faces.num = cmesh->num_faces;
		cmesh->mesh_data->normals_direction.num = cmesh->mesh_data->vertexs.num *3;

		cmesh->mesh_data->normals_direction.buffer_size = (cmesh->mesh_data->vertexs.num * 3/*num of coordinates by vertex*/);
		cmesh->mesh_data->normals_direction.buffer = new float[cmesh->mesh_data->normals_direction.buffer_size];
		memcpy(cmesh->mesh_data->normals_direction.buffer, mesh->normals, sizeof(float) * cmesh->mesh_data->normals_direction.buffer_size);

		
		
		cmesh->CalculateNormals();
	}


	cmesh->SetBuffersWithData();
		
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
	if (App->debug_scene->show_hierarchy)
	{
		ImGui::SetNextWindowSize(ImVec2(310, 984), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 22), ImGuiCond_::ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Hierarchy",NULL,ImGuiWindowFlags_NoSavedSettings))
		{

			//create primitives should be here

			for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
			{
				PrintGoList((*item));
			}

		}
		ImGui::End();
	}

	if (create_cube)
	{
		CreateCube();
		create_cube = false;
	}

	if (create_sphere)
	{
		CreateSphere();
		create_sphere = false;
	}

}


void GameObjectManager::PrintGoList(GameObject * object)
{
	if (object->IsActive() == false)
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 1.f));

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
	if (object->IsActive() == false)
		ImGui::PopStyleColor();
}