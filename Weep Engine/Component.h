#ifndef __COMPONENT_H__
#define __COMPONENT_H__


enum class ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH,
	TEXTURE
};


class Component
{
public:
	ComponentType type = ComponentType::NONE;

public:
	virtual void Update() {};
};


#endif //__COMPONENT_H__