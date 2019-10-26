#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>

class Component;
class ComponentTexture;
class ComponentMesh;
enum class ComponentType;


class GameObject
{
public:
	GameObject();

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


	//should be private
	std::vector<Component*> components;

public:
	bool parametric = false;
private:
	int			id = 0;
	bool		selected = false;
	std::string name;
	bool active = true;

};


#endif //__GAMEOBJECT_H__