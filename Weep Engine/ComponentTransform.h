#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component
{

public:
	ComponentTransform();

	void Save(Json::Value&) const override;
	void Load(const Json::Value&) override;

	void InspectorDraw();

	float4x4 GetGlobalTransform() const;
	void SetGlobalTransform(const float4x4& global);
	float4x4 GetLocalTransform() const;

	const void Translate(const float3 & pos);
	const void Rotate(const float3 & rotate);
	const void Scale(const float3 & scale);

	const void SetPosition(const float3& pos);
	const void SetRotation(const float3& rotation);
	const void SetRotationQuat(const Quat& quater);
	const void SetScale(const float3& scale);
	const float3 GetPosition() const;
	const float3 GetRotationEuler() const;
	const float3 GetScale() const;


private:
	void RecalculateLocalTransform();

private:
	float4x4 local_transform;
	float3   local_position;
	float3   local_rotation_euler;
	Quat     local_rotation_quat;
	float3   local_scale;

	float4x4 global_transform;

};
#endif // !__COMPONENTTRANSFORM_H__

