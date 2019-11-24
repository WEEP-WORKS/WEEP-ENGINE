#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "Globals.h"
#include "imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "jsoncpp/json/json.h"


class GameObject;
enum class ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA
};





class Component
{
public:
	ComponentType	type = ComponentType::NONE;
	GameObject*		object = nullptr;

public:
	virtual void Update() {};
	virtual void PostUpdate() {};

	virtual void CleanUp() {};
	virtual void InspectorDraw() {};

	virtual void Save(Json::Value&) const {};
	virtual void Load(const Json::Value&) {};

	const bool Component::IsActive() const { return is_active; };
	void Component::SetActive(bool to_active) { is_active = to_active; };

	virtual void OnGetBoundingBox(AABB &bbox) {};

private:
	bool			is_active = true;
};


#endif //__COMPONENT_H__