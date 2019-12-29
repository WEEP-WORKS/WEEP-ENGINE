#ifndef __COMPONENTUICHECKBOX_H__
#define __COMPONENTUICHECKBOX_H__

#include "ComponentUIObjectBase.h"

class Module;
enum class UICheckBoxType
{
	TEST,
	VSYNC
};

class ComponentUICheckBox : public ComponentUIObjectBase
{
	enum class UICheckBoxState
	{
		NONE,
		CLICKED,
	};

public:
	ComponentUICheckBox(UIType _type, float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent);

	void PreUpdate() override;
	void PostUpdate() override;
	void CleanUp() override;

	const bool IsClicked() const;
public:
	Module* listener = nullptr;
	UICheckBoxType check_box_type;
	UICheckBoxState state = UICheckBoxState::NONE;

	uint texture_id_background;
	uint texture_id_clicked;
	uint current_texture_id;

};

#endif // !__COMPONENTUICHECKBOX_H__

