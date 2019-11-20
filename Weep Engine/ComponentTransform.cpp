#include "ComponentTransform.h"
#include "imgui.h"
#include "App.h"
#include "JsonHelper.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform()
{
	local_transform.SetIdentity();
	local_position = float3(0, 0, 0);
	local_rotation_euler = float3(0, 0, 0);
	local_rotation_quat = Quat::identity;
	local_scale = float3(1, 1, 1);

	global_transform.SetIdentity();
}

void ComponentTransform::Update()
{
	float4x4 a = local_transform * local_transform;
}

void ComponentTransform::InspectorDraw() {

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float3 position = GetPosition();
		float3 rotation = GetRotationEuler();
		float3 scale = GetScale();

		if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
			SetPosition(position);

		if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.1f))
			SetRotation(rotation);

		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f))
			SetScale(scale);
	}
}

const float3 ComponentTransform::GetPosition() const
{
	return local_position;
}

const float3 ComponentTransform::GetRotationEuler() const
{
	return local_rotation_euler;
}

const float3 ComponentTransform::GetScale() const
{
	return local_scale;
}

const void ComponentTransform::SetPosition(const float3 & pos)
{
	local_position = pos;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetRotation(const float3 & rotation)
{
	float3 diff = rotation - local_rotation_euler;
	Quat quat_diff = Quat::FromEulerXYZ(diff.x*DEGTORAD, diff.y*DEGTORAD, diff.z*DEGTORAD);
	local_rotation_quat = local_rotation_quat * quat_diff;
	local_rotation_euler = rotation;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetScale(const float3 & scale)
{
	local_scale = scale;

	RecalculateLocalTransform();
}

void ComponentTransform::RecalculateLocalTransform()
{
	local_transform = float4x4::FromTRS(local_position, local_rotation_quat, local_scale);
}

const void ComponentTransform::SetRotationQuat(const Quat & quat)
{
	local_rotation_quat = quat;
	local_rotation_euler = local_rotation_quat.ToEulerXYZ() * RADTODEG;

	RecalculateLocalTransform();
}

float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_transform;
}

void ComponentTransform::SetGlobalTransform(const float4x4& global)
{
	global_transform = global;
}

float4x4 ComponentTransform::GetLocalTransform() const
{
	string s = object->GetName();
	return local_transform;
}

void ComponentTransform::Save(Json::Value& scene) const
{
	Json::Value component_transform;

	component_transform["type"] = (int)type;

	App->json_helper->Fill(component_transform["Position"] = Json::arrayValue, GetPosition());
	App->json_helper->Fill(component_transform["Rotation"] = Json::arrayValue, GetRotationEuler());
	App->json_helper->Fill(component_transform["Scale"] = Json::arrayValue, GetScale());

	scene.append(component_transform);
}

void ComponentTransform::Load(const Json::Value& component)
{
	App->json_helper->Fill(local_position, component["Position"]);
	App->json_helper->Fill(local_rotation_euler, component["Rotation"]);
	local_rotation_quat = Quat::FromEulerXYZ(local_rotation_euler.x*DEGTORAD, local_rotation_euler.y*DEGTORAD, local_rotation_euler.z*DEGTORAD);
	App->json_helper->Fill(local_scale, component["Scale"]);
}

const void ComponentTransform::Translate(const float3 & pos)
{
	if (pos.x != 0 || pos.y != 0 || pos.z != 0)
		local_position += pos;

	RecalculateLocalTransform();
}

const void ComponentTransform::Rotate(const float3 & rotate)
{
	SetRotation(GetRotationEuler() + rotate);

	RecalculateLocalTransform();
}

const void ComponentTransform::Scale(const float3 & scale)
{
	SetScale(GetScale() + scale);

	RecalculateLocalTransform();
}