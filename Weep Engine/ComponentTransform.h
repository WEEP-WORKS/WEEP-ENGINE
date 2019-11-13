#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();

	void Update();

	void InspectorDraw();

	const float4x4 GetGlobalTransform() const;
	const float4x4 GetLocalTransform() const;

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

