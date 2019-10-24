#include "ComponentTransform.h"
#include "imgui.h"

ComponentTransform::ComponentTransform()
{
	position = float3::zero;
	rotation = float3::zero;
	scale = float3::zero;
	
}

void ComponentTransform::InspectorDraw() {
	
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 position1 = position;
		float3 rotation1 = rotation;
		float3 scale1 = scale;

		if (ImGui::DragFloat3("Position", (float*)&position1, 0.1f))
			position1 = position;

		if (ImGui::DragFloat3("Rotation", (float*)&rotation1, 0.1f))
			rotation1 = rotation;

		if (ImGui::DragFloat3("Scale", (float*)&scale1, 0.1f, 0.0f))
			scale1 = scale;
	}
}