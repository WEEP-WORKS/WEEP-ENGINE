#ifndef __COMPONENTUIOBJECTBASE_H__
#define __COMPONENTUIOBJECTBASE_H__

#include "Component.h"
#include <list>
#include "MathGeoLib/include/Math/Rect.h"
#include "ResourceMesh.h"

enum class UIType
{
	NONE = -1,
	IMAGE,
	TEXT,
	BUTTON,
	INPUTTEXT,
	SCROLLBAR
};

class ComponentUIObjectBase : public Component
{
public:
	ComponentUIObjectBase(float2 pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent);
	virtual void PreUpdate();
	virtual void Update();
	virtual void CleanUp();
	void SetAllPos(float2 &mouse_move);
	virtual void SetPos(float2 & mouse_move);
	virtual void PostUpdate();

	const bool MouseInRect() const;

	const bool GetVisible() const;
	void SetAllVisible(const bool visible);
	virtual void SetFocusThis(bool focus_value);

private:
	void SetVisible(const bool visible);


public:
	std::list<ComponentUIObjectBase*> childrens;
	ComponentUIObjectBase* parent;
	bool dragging = false;
	bool draggable = false;
	UIType type = UIType::NONE;
	float2 local_pos;
	float2 world_pos_original;
	float2 world_pos_final;
	Rect rect_spritesheet_original;
	Rect rect_spritesheet_final;
	Rect rect_world;

	BuffersData<float>	vertexs_quad;
protected:
	bool focus = false;
	bool visible = true;
};

#endif // !__COMPONENTUIOBJECTBASE_H__


