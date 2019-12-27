#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ComponentRender2D.h"
#include "App.h"
#include "ModuleGameObjectManager.h"
#include "DebugScene.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleQuadtree.h"
#include <list>

#include "ResourceMesh.h"

GameObject::GameObject(std::string name, GameObject* parent, bool is_static) : name(name), parent(parent), is_static(is_static)
{
	LOG("New Game Object created!");
	if (parent != nullptr)
	{
		parent->childrens.push_back(this);
	}
	
	AddComponent(ComponentType::TRANSFORM);

	id = App->random->Int();

	local_bbox.SetNegativeInfinity();
}

GameObject::GameObject(bool is_static) : is_static(is_static) 
{
	local_bbox.SetNegativeInfinity();
}

void GameObject::PreUpdate()
{
	if (IsActive())
	{
		isInsideFrustum = false;
	}
}

void GameObject::Update()
{
	if (IsActive())
	{
		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter)->IsActive())
			{
				(*iter)->Update();//RenderMesh and texture in Update or PostUpdate??
			}
		}
	}
}

void GameObject::PostUpdate()
{
	if (IsActive())
	{
		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter)->IsActive())
			{
				(*iter)->PostUpdate();//Render
			}
		}
	}
}

void GameObject::CleanUp()
{

	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		(*iter)->CleanUp();
		RELEASE(*iter);
		
	}
	components.clear();

	//RELEASE(this);
}

Component* GameObject::AddComponent(const ComponentType& type)
{
	Component* ret = nullptr;

	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == type && type != ComponentType::TEXTURE)
		{
			LOG("This game object already has a component of this type. Create another game object and add this component!");
			return ret;
		}
	}

	switch (type)
	{
	case ComponentType::NONE:
		LOG("Component empty. Not accepted.");
		break;
	case ComponentType::TRANSFORM:
		ret = new ComponentTransform();
		ret->type = type;
		LOG("Component Transform added correctly.");
		break;
	case ComponentType::MESH:
		ret = new ComponentMesh();
		ret->type = type;
		LOG("Component Mesh added correctly.")
		break;
	case ComponentType::TEXTURE:
		ret = new ComponentTexture();
		ret->type = type;
		LOG("Component Texture added correcly.");
		break;
	case ComponentType::CAMERA:
		ret = new ComponentCamera();
		ret->type = type;
		
		LOG("Component Camera added correcly.");
		break;
	case ComponentType::RENDER2D:
		ret = new ComponentRender2D();
		ret->type = type;

		LOG("Component Render2D added correcly.");
		break;
	default:
		LOG("Component not found in the function. Not accepted.");
		break;
	}

	if(ret != nullptr)
		AddToComponentList(ret);

	return ret;
}

void GameObject::AddToComponentList(Component * &ret)
{
	ret->object = this;
	components.push_back(ret);
}

void GameObject::SetSelected(const bool& set)
{
	selected = set;
}

void GameObject::SetName(const std::string& set)
{
	name = set;
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

const bool GameObject::GetSelected() const
{
	return selected;
}

const bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(const bool & to_active)
{
	if (to_active)
	{
		DoForAllChildrens(&GameObject::SetActiveTrue);
	}
	else
	{
		DoForAllChildrens(&GameObject::SetActiveFalse);
	}
}

void GameObject::SetActiveFalse()
{
	active = false;
}

void GameObject::SetActiveTrue()
{
	active = true;
}

ComponentTexture* GameObject::GetTextureActivated() const
{
	std::vector<ComponentTexture*> textures = GetTextures();
	for (std::vector<ComponentTexture*>::const_iterator iter = textures.begin(); iter != textures.end(); ++iter)
	{
		if ((*iter)->IsTextureActive())
		{
			return (*iter);
		}
	}
	LOG("There is no texture activated.");
	return nullptr;
}

std::vector<ComponentTexture*> GameObject::GetTextures() const
{
	std::vector<ComponentTexture*> textures;
	for (std::vector<Component*>::const_iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == ComponentType::TEXTURE)
		{
			ComponentTexture* component = (ComponentTexture*)(*iter);
			textures.push_back(component);
		}
	}
	return textures;
}

ComponentMesh* GameObject::GetMesh() const
{
	for (std::vector<Component*>::const_iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == ComponentType::MESH)
		{
			return (ComponentMesh*)(*iter);
		}
	}

	return nullptr;
}

ComponentCamera* GameObject::GetCam() const
{
	for (std::vector<Component*>::const_iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == ComponentType::CAMERA)
		{
			return (ComponentCamera*)(*iter);
		}
	}

	return nullptr;
}

const bool GameObject::IsStatic() const
{
	return is_static;
}


int GameObject::DoForAllChildrens(std::function<void(GameObject*)> funct)
{
	int number_childrens = -1; // -1 to not count this game object and only his childrens.
	std::list<GameObject*> all_childrens;

	all_childrens.push_back(this);

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

			funct(current);
			++number_childrens;
		}
	}

	return number_childrens;
}

void GameObject::DoForAllChildrens(std::function<void(GameObject*, float&, GameObject*&)> funct, float& distance, GameObject*& closest)
{
	std::list<GameObject*> all_childrens;
	all_childrens.push_back(this);

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
			funct(current, distance, closest);
		}
	}

}

int GameObject::DoForAllChildrens(std::function<void(GameObject*, Json::Value&)> funct, Json::Value& scene)
{
	int number_childrens = -1; // -1 to not count this game object and only his childrens.
	std::list<GameObject*> all_childrens;

	all_childrens.push_back(this);

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

			funct(current, scene);
			++number_childrens;
		}
	}

	return number_childrens;
}

GameObject* GameObject::DoForAllChildrens(std::function<const bool(const GameObject*, const uint& id)> funct,const uint& id)
{

	std::list<GameObject*> all_childrens;

	all_childrens.push_back(this);


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

			if (funct(current, id))
				return current;
		}
	}

	LOG("The Game Object with id: %i has not found", id);
	return nullptr;
}


int GameObject::DoForAllSelected(std::function<bool(GameObject* /*from*/, GameObject*/*target*/ )> funct)
{
	int number_of_selected = -1; // -1 to not count this game object and only his childrens.
	bool ret = true;
		for (std::vector<GameObject*>::const_iterator iter = App->game_object_manager->selected.cbegin(); iter != App->game_object_manager->selected.cend() && ret; ++iter)
		{
			ret = funct(this,*iter);
			++number_of_selected;
		}

	return number_of_selected;
}

void GameObject::SelectThis()
{
	App->game_object_manager->selected.push_back(this);
	SetSelected(true);
}



const bool GameObject::IsMyBrother(const GameObject* object) const
{

	for (std::vector<GameObject*>::const_iterator iter = parent->childrens.cbegin(); iter != parent->childrens.cend(); ++iter)
	{
		if (object == (*iter))
		{
			return true;
		}
	}

	return false;
}

const bool GameObject::HasChildrens() const
{
	return childrens.empty();
}


const bool GameObject::SetAsNewChildren(GameObject* new_children)
{
	if (std::find(childrens.begin(), childrens.end(), new_children) == childrens.end() && !IsParentOfMyParents(new_children)) //if it isn't a children of this GameObject and is not a parent o parents of this game object.
	{
		new_children->parent->childrens.erase(std::find(new_children->parent->childrens.begin(), new_children->parent->childrens.end(), new_children));
		new_children->parent = this;
		childrens.push_back(new_children);
		return true;
	}
	else
		return false;
}

const bool GameObject::IsParentOfMyParents(const GameObject* possible_parent) const
{
	if (parent == nullptr)
		return false;

	GameObject* current_go = parent;
	while (current_go->parent != nullptr)
	{
		if (possible_parent == current_go)
			return true;

		current_go = current_go->parent;
	}
	return false;
}

void GameObject::SetGoSelectedAsChildrenFromThis()
{
	DoForAllSelected(&GameObject::SetAsNewChildren);
}

void GameObject::CalcGlobalTransform()
{
	float4x4 global;
	if (parent != nullptr)
	{
		global = parent->ConstGetTransform()->GetGlobalTransform() * ConstGetTransform()->GetLocalTransform();
		GetTransform()->SetGlobalTransform(global);
	}
}

void GameObject::CalcBBox()
{
	if (GetMesh())
	{
		local_bbox.SetNegativeInfinity();

		GetMesh()->OnGetBoundingBox(local_bbox);
		local_bbox.TransformAsAABB(ConstGetTransform()->GetGlobalTransform());
	}
}

bool GameObject::AddAABB(AABB& new_AABB)
{
	bool ret = false;
	if (local_bbox.IsDegenerate())
	{
		local_bbox = new_AABB;
		if (is_static)
			App->quadtree->Insert(this);

		ret = true;
	}
	else
	{
		LOG("This game object has a AABB. Can't add another!");
	}

	return ret;
}

void GameObject::Save(Json::Value& scene) const
{
	/*string s =scene[0]["name"].asString();
	LOG("%s", s.c_str());*/

	Json::Value go;

	go["number_components"] = components.size();

	go["Components"] = Json::arrayValue;
	for (vector<Component*>::const_iterator citer = components.cbegin(); citer != components.cend(); ++citer)
	{
		(*citer)->Save(go["Components"]);
	}

	go["name"] = name;
	go["id"] = id;
	if (parent != nullptr)
		go["id_parent"] = parent->id;
	else
		go["id_parent"] = "nullptr";

	scene.append(go);
}

void GameObject::Load(const Json::Value& Json_go)
{
	name = Json_go["name"].asString();
	id = Json_go["id"].asInt();
	if (Json_go["id_parent"] != "nullptr")
	{
		parent = App->game_object_manager->GetGOById(Json_go["id_parent"].asInt());
		parent->childrens.push_back(this);
	}
	else
	{
		App->game_object_manager->root = this;
	}
	//int number_components = Json_go["number_components"].asInt();
	for (uint i = 0; i < Json_go["Components"].size(); ++i)
	{
		AddComponent((ComponentType)Json_go["Components"][i]["type"].asInt());
	}

	uint current_component = 0u;
	for (std::vector<Component*>::const_iterator citer = components.cbegin(); citer != components.cend(); ++citer)
	{
		(*citer)->Load(Json_go["Components"][current_component++]);
	}
	if (GetMesh() != nullptr)
	{
		CalcGlobalTransform();
		CalcBBox();
		App->quadtree->Insert(this);
	}
}

const bool GameObject::IsThisGOId(const uint& id) const
{
	return (this->id == id);
}

ComponentTransform* GameObject::GetTransform() const
{
	for (vector<Component*>::const_iterator citer = components.cbegin(); citer != components.cend(); ++citer)
	{
		if ((*citer)->type == ComponentType::TRANSFORM)
			return(ComponentTransform*)(*citer);
	}

	LOG("Component transform not found!");
	return nullptr;
}

const ComponentTransform* GameObject::ConstGetTransform() const
{
	for (vector<Component*>::const_iterator citer = components.cbegin(); citer != components.cend(); ++citer)
	{
		if ((*citer)->type == ComponentType::TRANSFORM)
			return(ComponentTransform*)(*citer);
	}

	LOG("Component transform not found!");
	return nullptr;
}

bool PointInRect(float2 point_xy, Rect rect_xywh)
{
	if (point_xy.x >= rect_xywh.left && point_xy.x <= rect_xywh.right && point_xy.y > rect_xywh.top && point_xy.y < rect_xywh.bottom)
		return true;

	return false;
}

void GameObject::TestRay(float& distance, GameObject*& closest)
{
	// Check if intersects with bbox
	if (local_bbox.IsFinite())
	{
		Rect rect = App->window->GetWindowRect();
		float2 mouse_pos = App->input->GetMouse();

		if (PointInRect(mouse_pos, rect) && !App->debug_scene->window_hvr && !App->game_object_manager->window_hvr && !App->debug_scene->about_hvr && !App->debug_scene->config_hvr && !App->debug_scene->menubar_hvr && !App->debug_scene->tools_hvr && !App->debug_scene->random_hvr && !App->debug_scene->mathgeo_hvr && !App->debug_scene->resource_hvr)
		{
			// MouseX - WindowX/Window Size
			// MouseY - WindowY/Window Height

			float first_normalized_x = (mouse_pos.x - rect.left) / (rect.right - rect.left);
			float first_normalized_y = (mouse_pos.y - rect.top) / (rect.bottom - rect.top);

			float normalized_x = (first_normalized_x * 2) - 1;
			float normalized_y = -((first_normalized_y * 2) - 1);

			LineSegment picking = App->camera->GetEditorCamera()->GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);

			if (picking.Intersects(local_bbox))
			{
				// Get mesh
				ComponentMesh* cmesh = GetMesh();

				if (cmesh != nullptr)
				{
					// Transform segment to match mesh transform
					LineSegment segment_local_space(picking);
					segment_local_space.Transform(ConstGetTransform()->GetGlobalTransform().Inverted());

					// Check every triangle
					Triangle tri;
					uint* indices = cmesh->GetResource()->mesh_data->indexs.buffer;
					float* vertices = cmesh->GetResource()->mesh_data->vertexs.buffer;
					for (int i = 0; i < cmesh->GetResource()->mesh_data->indexs.num;)
					{
						tri.a.Set(vertices[(3 * indices[i])], vertices[(3 * indices[i]) + 1], vertices[(3 * indices[i]) + 2]); ++i;
						tri.b.Set(vertices[(3 * indices[i])], vertices[(3 * indices[i]) + 1], vertices[(3 * indices[i]) + 2]); ++i;
						tri.c.Set(vertices[(3 * indices[i])], vertices[(3 * indices[i]) + 1], vertices[(3 * indices[i]) + 2]); ++i;

						float current_distance;
						float3 hit_point;
						if (segment_local_space.Intersects(tri, &current_distance, &hit_point))
						{
							if (current_distance < distance)
							{
								distance = current_distance;
								closest = this;
							}
						}
					}
				}
			}
		}
	}
}