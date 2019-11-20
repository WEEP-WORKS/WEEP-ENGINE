#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <functional>
#include "Globals.h"
#include "ComponentTransform.h"
#include "MathGeoLib/include\MathGeoLib.h"

#include "jsoncpp/json/json.h"

class Component;
class ComponentTexture;
class ComponentMesh;
class ComponentCamera;
enum class ComponentType;


class GameObject
{
public:
	GameObject(std::string name, GameObject* parent);
	GameObject() {};

	void PreUpdate();
	void Update(); //this is not from the module class. This function will be called from objectManager and will call Components update or something... I don't know yet.
	void PostUpdate();
	void CleanUp();

	void Save(Json::Value&) const ;
	void Load(const Json::Value&);

	Component* AddComponent(const ComponentType&);
	void AddToComponentList(Component * &ret);

	void SetSelected(const bool & set);

	void SetName(const std::string& set);

	const char * GetName() const;

	const bool GetSelected() const;

	const bool IsActive() const;

	void SetActive(const bool &to_active);

	void SetActiveFalse();

	void SetActiveTrue();

	ComponentTexture* GetTextureActivated() const;

	std::vector<ComponentTexture*> GetTextures() const;

	ComponentMesh* GetMesh() const;

	ComponentCamera * GetCam() const;

	ComponentTransform* GetTransform() const;
	const ComponentTransform* ConstGetTransform() const;

	int DoForAllChildrens(std::function<void(GameObject*)>);

	GameObject* DoForAllChildrens(std::function<const bool(const GameObject*, const uint& id)>, const uint& id);

	int DoForAllChildrens(std::function<void(GameObject*, Json::Value&)>, Json::Value&);

	int DoForAllSelected(std::function<bool(GameObject*, GameObject*)>);

	void SelectThis();

	void CalculateNumberOfChildrens()const {}

	const bool IsMyBrother(const GameObject* object) const;

	const bool HasChildrens() const;

	const bool SetAsNewChildren(GameObject* new_children);

	void SetGoSelectedAsChildrenFromThis();

	void CalcGlobalTransform();
	void CalcBBox();

	const bool IsParentOfMyParents(const GameObject* possible_parent) const;

	const bool IsThisGOId(const uint& id) const ;

public:
	bool					parametric		= false;
	bool hierarchy_opnened = false;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childrens;

	//should be private
	std::vector<Component*> components;

	AABB local_bbox;

	bool isInsideFrustum = false;

private:
	int						id				= 0;
	bool					selected		= false;
	std::string				name			= "";
	bool					active			= true;

};


#endif //__GAMEOBJECT_H__