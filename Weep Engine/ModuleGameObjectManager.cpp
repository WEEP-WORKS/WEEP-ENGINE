#include "ModuleGameObjectManager.h"
#include "App.h"
#include "Globals.h"
#include "GameObject.h"
#include "imgui.h"
#include "ModuleInput.h"
#include "ComponentMesh.h"
#include "ModuleTexture.h"
#include "ComponentTexture.h"
#include "ModuleCamera3D.h"
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

bool GameObjectManager::PreUpdate()
{
	if (!to_delete.empty())
	{
		//reverse iter to destroy first the childrens.
		for (list<GameObject*>::iterator iter = to_delete.begin(); iter != to_delete.end(); ++iter)
		{
			Destroy(*iter);
		}
		to_delete.clear();
	}


	root->DoForAllChildrens(&GameObject::CalcGlobalTransform);
	root->DoForAllChildrens(&GameObject::CalcBBox);

	return true;
}

bool GameObjectManager::Update() 
{
	vector<Camera3D*> cameras = App->camera->GetCameras();
	vector<GameObject*> to_draw;

	// Get elements to draw from all cameras
	for (vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
	{
		if ((*it)->GetFrustumCulling())
			(*it)->GetElementsToDraw(to_draw);
	}

	// Draw
	for (vector<GameObject*>::iterator it = to_draw.begin(); it != to_draw.end(); ++it)
		(*it)->DoForAllChildrens(&GameObject::Update);

	root->DoForAllChildrens(&GameObject::Update);

	Hierarchy();

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		AddGameObjectsSelectedToDestroy();
	}

	return true;
}

bool GameObjectManager::PostUpdate()
{

	return true;
}


bool GameObjectManager::CleanUp()
{
	/*for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
	{
		(*item)->CleanUp();
		RELEASE(*item);
	}*/

	root->DoForAllChildrens(&GameObject::CleanUp);
	DoForAllChildrens(&GameObjectManager::ReleaseGameObject);
	return true;
}

void GameObjectManager::ReleaseGameObject(GameObject* object)
{
	RELEASE(object);
}

void GameObjectManager::Destroy(GameObject * go)
{
	LOG("deleting GameObject '%s'", go->GetName());

	//if the Game Object is selected, deselectect it. 
	std::vector<GameObject*>::iterator iter = std::find(selected.begin(), selected.end(), go);
	if (iter != selected.cend())
	{
		selected.erase(iter);
		go->SetSelected(false);
	}


	if (go->parent != nullptr)
		go->parent->childrens.erase(std::find(go->parent->childrens.begin(), go->parent->childrens.end(), go));

	go->CleanUp();
	ReleaseGameObject(go);

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
	GameObject* ret = new GameObject("Cube", root);
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
	GameObject* ret = new GameObject("Sphere", root);
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

	go->SelectThis();
}

void GameObjectManager::AddGameObjectsSelectedToDestroy()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		
		//(*it)->DoForAllChildrens(&GameObject::AddThisGameObjectToDelete);
		DoForAllChildrens(&GameObjectManager::AddGameObjectToDestroy, (*it));
	}
}

void GameObjectManager::AddGameObjectToDestroy(GameObject* go)
{
	if (std::find(to_delete.begin(), to_delete.end(), go) == to_delete.end())
	{
		to_delete.push_front(go); //push_front to have childrens--->parent to destroy in this order.
	}
}

void GameObjectManager::ClearSelection()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end();)
	{
		(*it)->SetSelected(false);
		it = selected.erase(it);
	}
}



void GameObjectManager::Hierarchy()
{
	if (App->debug_scene->show_hierarchy)
	{
		ImGui::SetNextWindowSize(ImVec2(310, 984), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 22), ImGuiCond_::ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Hierarchy",NULL,ImGuiWindowFlags_NoSavedSettings))
		{

			//create primitives should be here
			for (std::vector<GameObject*>::iterator iter = root->childrens.begin(); iter != root->childrens.end(); ++iter)
			{
				bool ret = PrintGoList(*iter);
				if (!ret)
					break;
			}
			
		//	DoForAllChildrensVertical(&GameObjectManager::PrintGoList);

			printed_hierarchy.clear();
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


bool GameObjectManager::PrintGoList(GameObject * object)
{
	bool ret = true;
	if (root == object || object == nullptr)
		return true;
		

	if (object->IsActive() == false)
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 1.f));

	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (object->childrens.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf;// | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	if (object->GetSelected())
	{
		flags |= ImGuiTreeNodeFlags_Selected;

		if (object->GetMesh())
		{
			DrawBBox(object);
		}		

		

		
	}

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
			//TODO
		}

		// Monoselection
		else
		{
			ClearSelection();
			AddGameObjectToSelected(object);
		}
	}
	if (object->GetSelected())
	{
		if (ImGui::BeginPopupContextItem("HerarchyPopup"))
		{
			if (!ImGui::IsPopupOpen(0))
			{
				if (ImGui::Button("Delete"))
				{
					AddGameObjectsSelectedToDestroy();
				}
			}
			ImGui::EndPopup();
		}
	}

	if (opened)
	{
		for (std::vector<GameObject*>::iterator iter = object->childrens.begin(); iter != object->childrens.end(); ++iter)
		{
			ret = PrintGoList(*iter);
			if (!ret)
				break;
		}
		ImGui::TreePop();
	}

	if (object->IsActive() == false)
		ImGui::PopStyleColor();
	

	return ret;

	

}



void GameObjectManager::DrawBBox(GameObject * object)
{
	ComponentMesh* c_mesh = object->GetMesh();

	AABB mesh_aabb = c_mesh->GetBbox();

	static float3 corners[8];
	mesh_aabb.GetCornerPoints(corners);

	const float4x4 &transform = float4x4::identity;

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());
	GLint previous[2];
	glGetIntegerv(GL_POLYGON_MODE, previous);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(255.f, 45.f, 26.f);

	glLineWidth(4.0);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, previous[0]);

	glLineWidth(1.0f);

	glColor3f(255, 255, 255);
	glPopMatrix();
}



int GameObjectManager::DoForAllChildrens(std::function<void(GameObjectManager*, GameObject*)> funct, GameObject* start)
{
	int number_childrens = -1; //-1 to not count the root GameObject.
	std::list<GameObject*> all_childrens;
	if(start != nullptr)
		all_childrens.push_back(start);
	else
		all_childrens.push_back(root);


	while (!all_childrens.empty())
	{
		GameObject* current = (*all_childrens.begin());
		all_childrens.pop_front();
		if (current != nullptr)
		{
			for (std::vector<GameObject*>::const_iterator iter = current->childrens.cbegin(); iter != current->childrens.cend(); ++iter)
			{
				all_childrens.push_back(*iter);
			}

			funct(this, current);
			++number_childrens;
		}
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
		if (current != nullptr)
		{
			funct(this, current);

			if (!current->childrens.empty())
			{
				for (std::vector<GameObject*>::const_reverse_iterator iter = current->childrens.crbegin(); iter != current->childrens.crend(); ++iter)
				{
					all_childrens.push_front(*iter);
				}
			}

			++number_childrens;
		}
	}

	return number_childrens;
}


uint GameObjectManager::GetAllGameObjectNumber()
{
	return (uint)root->DoForAllChildrens(&GameObject::CalculateNumberOfChildrens);
}


