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

//#include <functional>

GameObjectManager::GameObjectManager(bool start_enabled) : Module(start_enabled)
{
	SetName("GameObjectManager");

}

bool GameObjectManager::Awake()
{
	root = new GameObject("root", nullptr);

	return true;
}


bool GameObjectManager::Update() 
{
	root->DoForAllChildrens(&GameObject::Update);

	Hierarchy();

	return true;
}

void GameObjectManager::DoUpdateIfActivated(GameObject* go)
{
	if (go->IsActive())
		go->Update();
}

bool GameObjectManager::CleanUp()
{
	/*for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
	{
		(*item)->CleanUp();
		RELEASE(*item);
	}*/

	root->DoForAllChildrens(&GameObject::CleanUp);
	
	return true;
}

//void GameObjectManager::AddObject(GameObject* object)
//{
//	objects.push_back(object);
//}

//to create a geometry shape automatically by their sides.
//GameObject* GameObjectManager::CreateGeometryShape(int sides)
//{
//	GameObject* ret = new GameObject();
//
//	
//	return ret;
//}

void GameObjectManager::CreateCube()
{
	GameObject* ret = new GameObject("Cube", nullptr);
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
}

void GameObjectManager::CreateSphere()
{
	GameObject* ret = new GameObject("Sphere", nullptr);
	par_shapes_mesh* mesh = par_shapes_create_subdivided_sphere(2);
	ComponentMesh* cmesh = (ComponentMesh*)ret->AddComponent(ComponentType::MESH);

	if (mesh != nullptr)
	{
		LoadGeometryShapeInfo(cmesh, mesh);
	}
	ret->SetName("sphere");
	ClearSelection();
	AddGameObjectToSelected(ret);
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

	go->DoForAllChildrens(&GameObject::SelectThis);
	
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

			
			DoForAllChildrensVertical(&GameObjectManager::PrintGoList);

			printed_hierarchy.clear();
		}
		ImGui::End();
	}
	//std::function<void(GameObjectManager*)> funct_var =&GameObjectManager::print1;
	if (create_cube)
	{
		//printThisFunction(std::bind(&GameObjectManager::print1, this));    //-> with void()
	//CreateCube();
		//funct_var = std::bind(&GameObjectManager::print1, this);
		//printThis(&GameObjectManager::print1);
		//funct_var(this);
		create_cube = false;
	}

	if (create_sphere)
	{
		//std::list<GameObject*>::iterator GO = objects.begin();
		//printThisFunction(&GameObject::printGO);			//-> with void(GameObject) 
		//printThisFunction(std::bind(&GameObject::printGO, (*GO)));			//-> with void() problem if GO function need some var from itself, because we have to bind the function to wich entity will execute the function. 
		//funct_var = &GameObjectManager::print2;
		//printThis(&GameObjectManager::print2);
		//funct_var(this);
		//CreateSphere();
		create_sphere = false;
	}

}


void GameObjectManager::PrintGoList(GameObject * object)
{
	if (root == object)
		return;




	bool is_first_children = false;
	for (std::vector<GameObject*>::const_iterator iter = root->childrens.cbegin(); iter != root->childrens.cend(); ++iter)
	{
		if ((*iter) == object)
			is_first_children = true;
	}


	if (object->parent->hierarchy_opnened || is_first_children)
	{
		
		if (object->parent->hierarchy_opnened)
		{
			uint i = 0u;
		}
		if (object->IsActive() == false)
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 1.f));

		if (object == nullptr)
			return;

		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if (object->childrens.empty())
		{
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}

		if (object->GetSelected())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		//treenode needs to be more understood
		object->hierarchy_opnened = ImGui::TreeNodeEx(object->GetName(), flags);

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
				//TODO
			}

			// Monoselection
			else
			{
				ClearSelection();
				AddGameObjectToSelected(object);
			}
		}

		if (object->IsActive() == false)
			ImGui::PopStyleColor();

		for (std::list<GameObject*>::const_iterator iter = printed_hierarchy.cbegin(); iter != printed_hierarchy.cend(); ++iter)
		{
			if (object->IsMyBrother(*iter))
			{
				if(!is_first_children)
				ImGui::TreePop();


				GameObject* before = (*printed_hierarchy.begin());

				while (before->parent != object->parent)
				{
					//ImGui::TreePop();
					before = before->parent;
				}
			}
		}
		printed_hierarchy.push_front(object);
	}

}

void GameObjectManager::AllTreePop(GameObject* object)
{
	if (object->hierarchy_opnened)
	{
		ImGui::TreePop();
	}
}

int GameObjectManager::DoForAllChildrens(std::function<void(GameObjectManager*, GameObject*)> funct)
{
	int number_childrens = -1; //-1 to not count the root GameObject.
	std::list<GameObject*> all_childrens;

	all_childrens.push_back(root);

	while (!all_childrens.empty())
	{
		GameObject* current = (*all_childrens.begin());
		all_childrens.pop_front();
		for (std::vector<GameObject*>::const_iterator iter = current->childrens.cbegin(); iter != current->childrens.cend(); ++iter)
		{
			all_childrens.push_back(*iter);
		}

		funct(this, current);
		++number_childrens;
	}

	return number_childrens;
}

int GameObjectManager::DoForAllChildrensVertical(std::function<void(GameObjectManager*, GameObject*)> funct)
{
	int number_childrens = -1; //-1 to not count the root GameObject.
	std::list<GameObject*> all_childrens;

	all_childrens.push_back(root);

	while (!all_childrens.empty())
	{
		GameObject* current = (*all_childrens.begin());
		all_childrens.pop_front();
		for (std::vector<GameObject*>::const_reverse_iterator iter = current->childrens.crbegin(); iter != current->childrens.crend(); ++iter)
		{
			all_childrens.push_front(*iter);
		}

		funct(this, current);
		++number_childrens;
	}

	return number_childrens;
}


uint GameObjectManager::GetAllGameObjectNumber()
{
	return (uint)root->DoForAllChildrens(&GameObject::CalculateNumberOfChildrens);
}


