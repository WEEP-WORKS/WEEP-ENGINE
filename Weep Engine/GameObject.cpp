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
		AddToComponentList(ret);
		LOG("Component Transform added correctly.");
		break;
	case ComponentType::MESH:
		ret = new ComponentMesh();
		ret->type = type;
		AddToComponentList(ret);
		LOG("Component Mesh added correctly.")
		break;
	case ComponentType::TEXTURE:
		ret = new ComponentTexture();
		ret->type = type;
		AddToComponentList(ret);
		LOG("Component Texture added correcly.");
		break;
	case ComponentType::CAMERA:
		ret = new ComponentCamera();
		ret->type = type;
		AddToComponentList(ret);
		LOG("Component Camera added correcly.");
		break;
	default:
		LOG("Component not found in the function. Not accepted.");
		break;
	}

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

void GameObject::DeselectThis()
{
	std::vector<GameObject*>* selected = &App->game_object_manager->selected;
	std::vector<GameObject*>::iterator iter = std::find(selected->begin(), selected->end(), this);
	if (iter != selected->end())
	{
		selected->erase(iter);
		SetSelected(false);
	}
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
	if (std::find(childrens.begin(), childrens.end(), new_children) == childrens.end()) //if it isn't a children of this GameObject.
	{
		new_children->parent->childrens.erase(std::find(new_children->parent->childrens.begin(), new_children->parent->childrens.end(), new_children));
		new_children->parent = this;
		childrens.push_back(new_children);
		return true;
	}
	else
		return false;
}

void GameObject::SetGoSelectedAsChildrenFromThis()
{
	DoForAllSelected(&GameObject::SetAsNewChildren);
}

void GameObject::CalcGlobalTransform()
{
	float4x4 global_transform = transform->GetGlobalTransform();

	if (parent != nullptr)
	{
		global_transform = parent->transform->GetGlobalTransform() * transform->GetLocalTransform();
	}
}

void GameObject::CalcBBox()
{
	AABB local_bbox;

	local_bbox.SetNegativeInfinity();

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
		(*it)->OnGetBoundingBox(local_bbox);

	if (local_bbox.IsFinite())
		local_bbox.Transform(transform->GetGlobalTransform());
}