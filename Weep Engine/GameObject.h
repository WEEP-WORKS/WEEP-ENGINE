#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>

class Component;
enum class ComponentType;


class GameObject
{
public:
	GameObject();

	void Update(); //this is not from the module class. This function will be called from objectManager and will call Components update or something... I don't know yet.

	//Don't have cleanUp for each game objects and their components. TODO

	Component* AddComponent(ComponentType);
	void AddToComonentList(Component * &ret);
public:
	std::vector<Component*> components;
};


#endif //__GAMEOBJECT_H__