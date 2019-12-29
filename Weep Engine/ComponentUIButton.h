#ifndef __COMPONENTUIBUTTON_H__
#define __COMPONENTUIBUTTON_H__

#include "ComponentUIObjectBase.h"

class Module;
enum class UIButtonType
{
	TEST
};

class ComponentUIButton : public ComponentUIObjectBase
{
	enum class UIButtonState
	{
		NONE,
		HOVER,
		CLICKED,
	};

public:
	ComponentUIButton(UIType _type, float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent);

	void PreUpdate() override;
	void PostUpdate() override;
	void CleanUp() override;
public:
	Module* listener = nullptr;
	UIButtonType button_type;
	UIButtonState state = UIButtonState::NONE;

	uint texture_id_background;
	uint texture_id_hover;
	uint texture_id_clicked;
	uint current_texture_id;

};

#endif // !__COMPONENTUIBUTTON_H__
