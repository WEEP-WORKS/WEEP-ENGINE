#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

GameObject::GameObject()
{
	//Create Component transform and add to the list.
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		(*iter)->Update();
	}
}

Component* GameObject::AddComponent(ComponentType type)
{
	Component* ret = nullptr;
	if (type == ComponentType::MESH) //this in a switch wth all types. At this moment testing the mesh component.
	{
		Component* ret = new ComponentMesh();
		ret->type = type;
		components.push_back(ret);
		return ret;
	}
	return ret;

}
