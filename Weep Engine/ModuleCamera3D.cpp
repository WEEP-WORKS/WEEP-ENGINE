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
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(5, 5, 5);
	Reference = vec3(0, 0, 0);

	LookAt(vec3(0, 1, 0));

	camera_distance = 0;

	SetName("Camera");
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
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update()
{
	bool ret = true;
	float Sensitivity = 0.25f;
	vec3 newPos(0, 0, 0);
	float speed = 3.0f * App->GetDT();
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 6.0f * App->GetDT();

	if (App->input->GetMouseZ() == 1) newPos -= Z * speed * 1;
	else if (App->input->GetMouseZ() == -1)	newPos += Z * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed * 1;
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed * 1;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed * 1;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed * 1;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed * 1;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed * 1;

		Position += newPos;
		Reference += newPos;

		Reference = Position;

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		

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
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LALT) || App->input->GetKey(SDL_SCANCODE_RALT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed * 1;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed * 1;

			Position += newPos;
			Reference += newPos;

			Reference = vec3(0,0,0);

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				// Rotate arround the y axis
				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				// Rotate arround the X direction
				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);
			}

			Position = Reference + Z * length(Position);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		Focus();
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return ret;
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

void ModuleCamera3D::Focus()
{
	GameObject* selected = App->game_object_manager->selected.back();

	ComponentMesh* c_mesh = selected->GetMesh();
		
		if (selected != nullptr)
		{
			AABB mesh_aabb = c_mesh->mesh_data->aabb;

			Sphere sphere = mesh_aabb.MinimalEnclosingSphere();

			if (sphere.Diameter() != 0)
			{
				float distance = sphere.Diameter() * 1.5f;
				
				Reference.x = mesh_aabb.CenterPoint().x;
				Reference.y = mesh_aabb.CenterPoint().y;
				Reference.z = mesh_aabb.CenterPoint().z;

				Position = Reference + Z * distance;
			}

			//Reference = focus;

			//Position = Reference + Z * distance;
		}
}