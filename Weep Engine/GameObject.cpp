#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"

GameObject::GameObject()
{
	AddComponent(ComponentType::TRANSFORM);
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		(*iter)->Update();//RenderMesh and texture in Update or PostUpdate??
	}
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
