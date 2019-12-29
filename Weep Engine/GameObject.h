#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <functional>
#include "Globals.h"
#include "ComponentTransform.h"
#include "MathGeoLib/include\MathGeoLib.h"
#include "ComponentUIObjectBase.h"

#include "jsoncpp/json/json.h"

class Component;
class ComponentTexture;
class ComponentMesh;
class ComponentCamera;
class ComponentRender2D;
class ComponentUIImage;
class ComponentUIButton;
class ComponentUICheckBox;
class Module;
enum class ComponentType;
enum class UIButtonType;
enum class UICheckBoxType;

class GameObject
{
public:
	GameObject(std::string name, GameObject* parent, bool is_static = true);
	GameObject(bool is_static = true);

	void PreUpdate();
	void Update(); //this is not from the module class. This function will be called from objectManager and will call Components update or something... I don't know yet.
	void PostUpdate();
	void CleanUp();

	void Save(Json::Value&) const ;
	void Load(const Json::Value&);

	Component* AddComponent(const ComponentType&);
	ComponentUIImage* AddComponentUIImage(float2 local_pos, Rect rect_spritesheet_original, bool draggable = false, ComponentUIObjectBase* parent = nullptr);
	ComponentUIButton* AddComponentUIButton(float2 local_pos, Rect rect_spritesheet_original, UIButtonType type, Module* listener, bool draggable = false, ComponentUIObjectBase* parent = nullptr);
	ComponentUICheckBox* AddComponentUICheckBox(float2 local_pos, Rect rect_spritesheet_original, UICheckBoxType type, Module* listener, bool draggable = false, ComponentUIObjectBase* parent = nullptr);


	void AddToComponentList(Component * ret);

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

	ComponentRender2D* GetRender2D() const;
	ComponentTransform* GetTransform() const;
	const ComponentTransform* ConstGetTransform() const;

	const bool IsStatic()const;

	int DoForAllChildrens(std::function<void(GameObject*)>);

	void DoForAllChildrens(std::function<void(GameObject*, float&, GameObject*&)>, float&, GameObject*&);

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

	bool AddAABB(AABB&);

	const bool IsParentOfMyParents(const GameObject* possible_parent) const;
	const bool IsThisGOId(const uint& id) const ;
	void TestRay(float& distance, GameObject*& closest);//const LineSegment & segment, GameObject *& closest, float & dist

private:
	ComponentUIObjectBase* AddComponentUI(const UIType& type, float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent);


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
	bool					is_static		= true;

};


#endif //__GAMEOBJECT_H__