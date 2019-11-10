#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Camera3D
{
public:
	Camera3D();

	void SetPosition(const float3& pos);
	const float3 GetPosition();

	void SetNearPlaneDistance(const float& set);
	void SetFarPlaneDistance(const float& set);
	void SetFOV(const float& set);
	void SetAspectRatio(const float& set);
	const float GetNearPlaneFistance() const;
	const float GetFarPlaneDistance() const;
	const float GetVerticalFOV() const;
	const float GetHorizontalFOV() const;
	const float4x4 GetViewMatrix() const;
	const float4x4 GetProjectionMatrix() const;

	void MoveFront(const float& speed);
	void MoveBack(const float& speed);
	void MoveRight(const float& speed);
	void MoveLeft(const float& speed);
	void MoveUp(const float& speed);
	void MoveDown(const float& speed);

	void Orbit(const float3& rotate_center, const float& motion_x, const float& motion_y);
	void Rotate(const float& motion_x, const float& motion_y);
	void Focus(const float3& focus_center, const float& distance);

	void Look(const float3& look_pos);

private:
	Frustum frustum;
	float	aspect_ratio = 0.0f;

};


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	Camera3D* GetEditorCamera() const;
	void SetCurrentCamera(Camera3D* set);
	Camera3D* GetCurrentCamera() const;

	const float * GetViewMatrix() const;

private:

	Camera3D* editor_camera = nullptr;
	Camera3D* current_camera = nullptr;

};