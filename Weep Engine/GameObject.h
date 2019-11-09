#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <functional>
#include "Globals.h"

class Component;
class ComponentTexture;
class ComponentMesh;
enum class ComponentType;


class GameObject
{
public:
	GameObject(std::string name, GameObject* parent);

	void Update(); //this is not from the module class. This function will be called from objectManager and will call Components update or something... I don't know yet.

	void CleanUp();
	//Don't have cleanUp for each game objects and their components. TODO

	Component* AddComponent(ComponentType);
	void AddToComonentList(Component * &ret);

	void SetSelected(const bool & set);

	void SetName(const std::string& set);

	const char * GetName() const;

	const bool GetSelected() const;

	bool IsActive() const;

	void SetActive(const bool &to_active);

	ComponentTexture* GetTextureActivated() const;

	std::vector<ComponentTexture*> GetTextures() const;

	ComponentMesh* GetMesh() const;

	void DoForAllChildrens(std::function<void(GameObject*)>);

	void SelectThis();

	void printGO() { LOG("%i", active); }


public:
	bool					parametric		= false;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childrens;

	//should be private
	std::vector<Component*> components;

private:
	int						id				= 0;
	bool					selected		= false;
	std::string				name			= "";
	bool					active			= true;

};


#endif //__GAMEOBJECT_H__