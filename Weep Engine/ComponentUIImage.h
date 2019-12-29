#ifndef __COMPONENTUIIMAGE_H__
#define __COMPONENTUIIMAGE_H__

#include "ComponentUIObjectBase.h"
#include "GameObject.h"

class ComponentUIImage : public ComponentUIObjectBase
{
public:
	ComponentUIImage(UIType _type, float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent) : ComponentUIObjectBase(local_pos, rect_spritesheet_original, draggable, parent) { type = _type; }
	
	void InspectorDraw()
	{
		if (ImGui::CollapsingHeader("UIImage", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (parent != nullptr)
			{
				ImGui::Text("Parent: %s ", parent->object->GetName());
				ImGui::Text("Has Parent, be careful when drag");
			}
			else
				ImGui::Text("Don't have parent");

			ImGui::Separator();

			float2 position = local_pos;

			if (ImGui::DragFloat2("Position", (float*)&position, 0.1f))
				SetPos(position - local_pos);

		}
	}
};

#endif // !__COMPONENTUIIMAGE_H__
