#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModuleCamera3D.h"
#include "ModulePlayer.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleCamera3D::ModuleCamera3D(Application* app, uint cameraNum , ModulePlayer* playerToFollow, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	this->playerToFollow = playerToFollow;
	this->cameraNum = cameraNum;

	switch (this->cameraNum) {

	case 1:
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.w = SCREEN_WIDTH* 0.5f;
		viewport.h = SCREEN_HEIGHT;
		break;
	case 2: 
		viewport.x = SCREEN_WIDTH * 0.5f;
		viewport.y = 0.0f;
		viewport.w = SCREEN_WIDTH * 0.5f;
		viewport.h = SCREEN_HEIGHT;

		break;
	}
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		debug = !debug;

	if (debug == true)
	{
		CameraDebug(dt);
	}
	else
	{
		CameraFollowPlayer();
	}


	return UPDATE_CONTINUE;
}

update_status ModuleCamera3D::PostUpdate(float dt)
{
	// Draw all elements on camera viewport ------------------------
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(GetViewMatrix());
	glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
	App->Draw();

	return UPDATE_CONTINUE;
}

bool ModuleCamera3D:: CameraFollowPlayer()
{
	if (playerToFollow == nullptr)
	{
		return false;
	}

	PhysVehicle3D* vehicleToFollow = playerToFollow->GetPlayerCar();

	if (vehicleToFollow == nullptr)
	{
		return false;
	}

	btVector3 offset;
	btQuaternion q = vehicleToFollow->vehicle->getChassisWorldTransform().getRotation();
	mat4x4 t;

	vehicleToFollow->vehicle->getChassisWorldTransform().getOpenGLMatrix(&t);
	offset.setValue(0, 0, -9.0f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	vec3 camera_pos = vehicleToFollow->GetPos() + vec3(offset.getX(), 3.5f, offset.getZ());

	Position = camera_pos;
	LookAt(vehicleToFollow->GetPos());

	return true;
}


bool ModuleCamera3D::CameraDebug (float dt)
{
	// Implement a debug camera with keys and mouse
// Now we can make this movememnt frame rate independant!

	vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return true;
}


// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}