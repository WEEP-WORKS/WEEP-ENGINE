#include "Globals.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "ModuleGameObjectManager.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ComponentMesh.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	SetName("Camera3D");

	editor_camera = new Camera3D();
	current_camera = editor_camera;
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Awake()
{
	bool ret = true;

	LOG("Loading Camera3D");

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	bool ret = true;

	LOG("Setting up the camera");

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	bool ret = true;

	LOG("Cleaning camera");

	delete editor_camera;

	return ret;
}

Camera3D * ModuleCamera3D::GetEditorCamera() const
{
	return editor_camera;
}

void ModuleCamera3D::SetCurrentCamera( Camera3D * set)
{
	if (set != nullptr)
		current_camera = set;
}

Camera3D * ModuleCamera3D::GetCurrentCamera() const
{
	return current_camera;
}

const float * ModuleCamera3D::GetViewMatrix() const
{
	return current_camera->GetViewMatrix().Transposed().ptr();
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update()
{
	bool ret = true;

	if (editor_camera == nullptr)
		return true;

	float Sensitivity = 0.25f;
	//vec3 newPos(0, 0, 0);

	float speed = 6.0f * App->GetDT();
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 12.0f * App->GetDT();

	if (App->input->GetMouseZ() == 1) editor_camera->MoveFront(speed*10);
	else if (App->input->GetMouseZ() == -1)	editor_camera->MoveBack(speed * 10);

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) editor_camera->MoveUp(speed);
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) editor_camera->MoveDown(speed);

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) editor_camera->MoveFront(speed);
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) editor_camera->MoveBack(speed);

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) editor_camera->MoveLeft(speed);
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) editor_camera->MoveRight(speed);

		editor_camera->Rotate(-App->input->GetMouseXMotion()*Sensitivity*0.01f, -App->input->GetMouseYMotion()*Sensitivity*0.01f);

	}
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LALT) || App->input->GetKey(SDL_SCANCODE_RALT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) editor_camera->MoveFront(speed);

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) editor_camera->MoveBack(speed);
			editor_camera->Orbit(float3(0, 0, 0), -App->input->GetMouseXMotion()*Sensitivity*0.01f, -App->input->GetMouseYMotion()*Sensitivity*0.01f);
			editor_camera->Look(float3(0, 0, 0));
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		editor_camera->Focus(float3(0, 0, 0), 10);
	}

	return ret;
}

Camera3D::Camera3D()
{
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.verticalFov = 0;
	frustum.horizontalFov = 0;
	frustum.farPlaneDistance = 0;
	frustum.nearPlaneDistance = 0;

	SetNearPlaneDistance(0.1f);
	SetFarPlaneDistance(1000.0f);
	SetAspectRatio(1.3f);
	SetFOV(60);

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = DEGTORAD * 120.0f;
}

void Camera3D::SetPosition(const float3 & pos)
{
	frustum.pos = pos;
}

const float3 Camera3D::GetPosition()
{
	return frustum.pos;
}


void Camera3D::SetNearPlaneDistance(const float & set)
{
	if (set > 0 && set < frustum.farPlaneDistance)
		frustum.nearPlaneDistance = set;
}

void Camera3D::SetFarPlaneDistance(const float & set)
{
	if (set < 0 && set > frustum.nearPlaneDistance)
		frustum.farPlaneDistance = set;
}

void Camera3D::SetFOV(const float & set)
{
	frustum.verticalFov = DEGTORAD * set;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

void Camera3D::SetAspectRatio(const float & set)
{
	aspect_ratio = set;

	if (frustum.horizontalFov > 0 && frustum.verticalFov > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

const float Camera3D::GetNearPlaneFistance() const
{
	return frustum.nearPlaneDistance;
}

const float Camera3D::GetFarPlaneDistance() const
{
	return frustum.farPlaneDistance;
}

const float Camera3D::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

const float Camera3D::GetHorizontalFOV() const
{
	return frustum.horizontalFov;
}

const float4x4 Camera3D::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

const float4x4 Camera3D::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}


void Camera3D::MoveFront(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.front * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveBack(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.front * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveRight(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveLeft(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveUp(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveDown(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::Orbit(const float3 & rotate_center, const float & motion_x, const float & motion_y)
{
	float3 distance = frustum.pos - rotate_center;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.up, motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	frustum.pos = distance + rotate_center;
}

void Camera3D::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.front = rotation_x.Mul(frustum.front).Normalized();
	frustum.up = rotation_x.Mul(frustum.up).Normalized();

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.front = rotation_y.Mul(frustum.front).Normalized();
	frustum.up = rotation_y.Mul(frustum.up).Normalized();
}

void Camera3D::Look(const float3 & look_pos)
{
	float3 dir = look_pos - frustum.pos;

	float3x3 direction_matrix = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = direction_matrix.MulDir(frustum.front).Normalized();
	frustum.up = direction_matrix.MulDir(frustum.up).Normalized();
}

void Camera3D::Focus(const float3 & focus_center, const float & distance)
{
	Look(focus_center);

	float3 dir = frustum.pos - focus_center;
	frustum.pos = dir.Normalized() * distance;
}