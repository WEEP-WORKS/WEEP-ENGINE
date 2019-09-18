#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModulePlayer;
class PhysVehicle3D;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, uint cameraNum, ModulePlayer* playerToFollow , bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool CameraFollowPlayer();
	bool CameraDebug(float dt);

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	bool debug = false;

private:

	mat4x4            ViewMatrix, ViewMatrixInverse;
	ModulePlayer*     playerToFollow = nullptr;
	uint              cameraNum = 1u;
	SDL_Rect          viewport = { 0,0,0,0 };


};