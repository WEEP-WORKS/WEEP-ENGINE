#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"

GameObject::GameObject()
{
	LOG("New Game Object created!");
	AddComponent(ComponentType::TRANSFORM);
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->IsActive())
			(*iter)->Update();//RenderMesh and texture in Update or PostUpdate??
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
		AddToComonentList(ret);
		LOG("Component Transform added correctly.");
		break;
	case ComponentType::MESH:
		ret = new ComponentMesh();
		ret->type = type;
		AddToComonentList(ret);
		LOG("Component Mesh added correctly.")
		break;
	case ComponentType::TEXTURE:
		ret = new ComponentTexture();
		ret->type = type;
		AddToComonentList(ret);
		LOG("Component Texture added correcly.");
		break;
	default:
		LOG("Component not found in the function. Not accepted.");
		break;
	}

	return ret;
}

void GameObject::AddToComonentList(Component * &ret)
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
	active = to_active;
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