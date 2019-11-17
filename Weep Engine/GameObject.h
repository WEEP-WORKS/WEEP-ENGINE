#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <functional>
#include "Globals.h"
#include "ComponentTransform.h"
#include "MathGeoLib/include\MathGeoLib.h"

class Component;
class ComponentTexture;
class ComponentMesh;
class ComponentCamera;
enum class ComponentType;


class GameObject
{
public:
	GameObject(std::string name, GameObject* parent);

	void PreUpdate();

	void Update(); //this is not from the module class. This function will be called from objectManager and will call Components update or something... I don't know yet.

	void PostUpdate();

	void CleanUp();
	//Don't have cleanUp for each game objects and their components. TODO

	Component* AddComponent(ComponentType);
	void AddToComponentList(Component * &ret);

	void SetSelected(const bool & set);

	void SetName(const std::string& set);

	const char * GetName() const;

	const bool GetSelected() const;

	bool IsActive() const;

	void SetActive(const bool &to_active);

	void SetActiveFalse();

	void SetActiveTrue();

	ComponentTexture* GetTextureActivated() const;

	std::vector<ComponentTexture*> GetTextures() const;

	ComponentMesh* GetMesh() const;

	ComponentCamera * GetCam() const;

	int DoForAllChildrens(std::function<void(GameObject*)>);

	int DoForAllSelected(std::function<bool(GameObject*, GameObject*)>);

	void SelectThis();

	void CalculateNumberOfChildrens() {}

	bool IsMyBrother(GameObject* object) const;

	bool HasChildrens() const;

	bool SetAsNewChildren(GameObject* new_children);

	void SetGoSelectedAsChildrenFromThis();

	void CalcGlobalTransform();
	void CalcBBox();

	void TestRay();//const LineSegment & segment, GameObject *& closest, float & dist

	bool IsParentOfMyParents(GameObject* possible_parent);

public:
	bool					parametric		= false;
	bool hierarchy_opnened = false;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childrens;

	//should be private
	std::vector<Component*> components;

	ComponentTransform* transform = nullptr;

	AABB local_bbox;

	bool isInsideFrustum = false;

private:
	int						id				= 0;
	bool					selected		= false;
	std::string				name			= "";
	bool					active			= true;

};


#endif //__GAMEOBJECT_H__