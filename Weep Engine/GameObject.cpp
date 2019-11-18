#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "App.h"
#include "ModuleGameObjectManager.h"
#include <list>

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{
	LOG("New Game Object created!");
	if (parent != nullptr)
	{
		parent->childrens.push_back(this);
	}
	transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);

	id = App->random->Int();
}

GameObject::GameObject(Json::Value& Json_go)
{
	name = Json_go["name"].asString();
	id = Json_go["id"].asInt();
	if (Json_go["id_parent"] != "nullptr")
	{
		/*Get GO* by id and assign to the parent and this as children*/
		LOG("no root");
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

Component* GameObject::AddComponent(ComponentType type)
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

bool GameObject::IsActive() const
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



bool GameObject::IsMyBrother(GameObject* object) const
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

bool GameObject::HasChildrens() const
{
	return childrens.empty();
}


bool GameObject::SetAsNewChildren(GameObject* new_children)
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

bool GameObject::IsParentOfMyParents( GameObject* possible_parent)
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
		global = parent->transform->GetGlobalTransform() * transform->GetLocalTransform();
		transform->SetGlobalTransform(global);
	}
}

void GameObject::CalcBBox()
{

	local_bbox.SetNegativeInfinity();

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
		(*it)->OnGetBoundingBox(local_bbox);

	if (local_bbox.IsFinite())
		local_bbox.Transform(transform->GetGlobalTransform());
}

void GameObject::Save(Json::Value& scene)
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

