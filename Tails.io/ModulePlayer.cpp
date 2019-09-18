#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "Timer.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, uint playerNum, bool start_enabled) : Module(app, start_enabled), playerCar(NULL)
{
	turn = acceleration = brake = 0.0f;
	this->playerNum = playerNum;

	switch (playerNum)
	{
	case 1:
		initPosition.Set(0, 1, 85);
		firstColor = White;
		secondColor = {255.f/255.f,128.f / 255.f, 0.0f ,255.f / 255.f };
		break;
	case 2:
		initPosition.Set(0, 1, -85);
		firstColor = White;
		secondColor = Blue;
		break;
	}

	deadColor = { 255.f / 255.f,0, 0.0f , 0.5f };
	winnerColor = Green;
	loserColor = Red;


}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ---------------------------------------
	car.chassis_size.Set(1.4F, 0.5f, 3);
	car.chassis_offset.Set(0, 1, 0);
	car.mass = 150.0f;
	car.suspensionStiffness = 80.0F;
	car.suspensionCompression = 0.2f;
	car.suspensionDamping = 20.0f;
	car.maxSuspensionTravelCm = 200.0f;
	car.frictionSlip = 1000.0f;
	car.maxSuspensionForce = 6000.0f;
	// Wheel properties ---------------------------------------
	float connection_height = 0.9f;
	float front_wheel_radius = 0.4f;
	float rear_wheel_radius = 0.5f;

	float wheel_width = 0.3f;
	float suspensionRestLength = 0.25f;

	// Don't change anything below this line ------------------

	float half_width = (car.chassis_size.x + 0.6F)*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - front_wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = front_wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - front_wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = front_wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + rear_wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = rear_wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + rear_wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = rear_wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	playerCar = App->physics->AddVehicle(car);

	explosion_fx = App->audio->LoadFx("sfx/explosion.wav");
	shoot_fx = App->audio->LoadFx("sfx/shoot.wav");

	Reset();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	for (p2List_item<Missile*> * item = missiles.getFirst(); item; item = item->next)
	{
		App->physics->DeleteBody(item->data->physBody);
		delete item->data;
	}

	missiles.clear();

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	// Update Car -------------------------------------------

	turn = acceleration = brake = 0.0f;

	if (state == PlayerState::alive)
	{
		if (playerNum == 1)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				acceleration = MAX_ACCELERATION;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				if (turn < TURN_DEGREES)
					turn += TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				if (turn > -TURN_DEGREES)
					turn -= TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				acceleration = -MAX_ACCELERATION;
			}


			if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
			{
				brake = BRAKE_POWER;
			}

			if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			{
				if (ammo > 0)
				{
					AddMissile();
					--ammo;
					App->audio->PlayFx(shoot_fx);
				}
			}
		}

		else if (playerNum == 2)
		{
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				acceleration = MAX_ACCELERATION;
			}

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				if (turn < TURN_DEGREES)
					turn += TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				if (turn > -TURN_DEGREES)
					turn -= TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				acceleration = -MAX_ACCELERATION;
			}


			if (App->input->GetKey(SDL_SCANCODE_O) == KEY_REPEAT)
			{
				brake = BRAKE_POWER;
			}

			if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
			{
				if (ammo > 0)
				{
					AddMissile();
					--ammo;
					App->audio->PlayFx(shoot_fx);
				}
			}
		}
	}
	
	
	playerCar->ApplyEngineForce(acceleration);
	playerCar->Turn(turn);
	playerCar->Brake(brake);

	// Update light -----------------------------------------------
	vec3 lightPos = playerCar->GetPos() + vec3(0, 3, 0);
	App->renderer3D->lights[playerNum - 1].SetPos(lightPos.x, lightPos.y, lightPos.z);
	// Update Missiles  -------------------------------------------

	p2List_item<Missile*> * item = missiles.getFirst() ;

	while (item != nullptr)
	{
		item->data->Update();

		if (item->data->toDelete == true)
		{
			p2List_item<Missile*> *iterator = item->next;
			App->physics->DeleteBody(item->data->physBody);
			delete item->data;
			missiles.del(item);
			item = iterator;
		}
		else
		{
			item = item->next;
		}
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
	ModulePlayer* playerToKill = nullptr;

	switch (playerNum)
	{
	case 1:
		playerToKill = App->player_2;
		break;
	case 2:
		playerToKill = App->player_1;
		break;
	}

	if (body2 == playerToKill->playerCar)
	{
		for (p2List_item<Missile*> * item = missiles.getFirst(); item; item = item->next)
		{
			if (item->data->physBody == body1 && playerToKill->state == PlayerState::alive)
			{
				--playerToKill->lifes;

				if (playerToKill->lifes == 0)
				{
					playerToKill->state = PlayerState::loser;
					state = PlayerState::winner;
					App->scene_intro->haveWinner = true;
				}
				else
				{
					playerToKill->state = PlayerState::dead;
					App->scene_intro->StartAfterDeadTimer();
				}

				App->audio->PlayFx(explosion_fx);
				item->data->toDelete = true;

			}
		}
	}
}

void ModulePlayer::AddMissile()
{
	Missile* missile = new Missile(this);
	missiles.add(missile);
}

void ModulePlayer::AddAmmo()
{
	ammo += 3;
}

void ModulePlayer::Reset()
{
	ammo = 0;

	if (state != PlayerState::winner || state != PlayerState::loser)
	{
		state = PlayerState::alive;
	}
	
	playerCar->GetBody()->getWorldTransform().setIdentity();

	if (playerNum == 1)
	{
		btQuaternion q;
		q.setRotation(btVector3(0, 1, 0), DEGTORAD * 180);
		playerCar->GetBody()->getWorldTransform().setRotation(q);
	}
	else if (playerNum == 2)
	{

	}

	playerCar->GetBody()->setLinearVelocity(btVector3(0, 0, 0));
	playerCar->GetBody()->setAngularVelocity(btVector3(0, 0, 0));
	playerCar->SetPos(initPosition.x, initPosition.y, initPosition.z);

	for (p2List_item<Missile*> * item = missiles.getFirst(); item; item = item->next)
	{
		App->physics->DeleteBody(item->data->physBody);
		delete item->data;
	}

	missiles.clear();
}

int ModulePlayer::GetLifes()
{
	return lifes;
}

int ModulePlayer::GetAmmo()
{
	return ammo;
}

void ModulePlayer::SetLifes(int lifes)
{
	this->lifes = lifes;
}

void ModulePlayer::SetAmmo(int ammo)
{
	this->ammo = ammo;
}

PhysVehicle3D * ModulePlayer::GetPlayerCar()
{
	return playerCar;
}


Missile::Missile(ModulePlayer* owner)
{
	this->owner = owner;

	timerToDelete.Start();

	Sphere s(1);
	s.color = owner->secondColor;
	btVector3 offset;
	btQuaternion q = owner->GetPlayerCar()->vehicle->getChassisWorldTransform().getRotation();

	offset.setValue(0, 1.0f, 6);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	// Set init position offset respect the car ---------------------
	vec3 missileInitPos = owner->GetPlayerCar()->GetPos() + vec3(offset.getX(), offset.getY(), offset.getZ());
	s.SetPos(missileInitPos.x, missileInitPos.y, missileInitPos.z);

	// Use car Z vector as reference --------------------------------
	float force = 2000.0F;
	btVector3 Z(0, 0, 1);

	Z = Z.rotate(q.getAxis(), q.getAngle());
	Z *= force;

	// Create a missile ---------------------------------------------
	physBody = owner->App->physics->AddBody(s, 40.0F);
	physBody->collision_listeners.add(owner);
	physBody->Push(Z.getX(), Z.getY(), Z.getZ());
}

void Missile::Update()
{
	if (timerToDelete.Read() > TIME_TO_DELETE_MISSILE)
	{
		toDelete = true;
	}
}

bool ModulePlayer::Draw()
{
	// Colors -----------------
	Color color_1;
	Color color_2;

	switch (state)
	{
	case PlayerState::winner:
		color_1 = winnerColor;
		color_2 = winnerColor;
		break;
	case PlayerState::loser:
		color_1 = loserColor;
		color_2 = loserColor;
		break;
	case PlayerState::dead:
		color_1 = deadColor;
		color_2 = deadColor;
		break;
	case PlayerState::alive:
		color_1 = firstColor;
		color_2 = secondColor;
		break;
	}

	// Draw missiles ================================================
	for (p2List_item<Missile*> * item = missiles.getFirst(); item; item = item->next)
	{
		vec3 pos = item->data->physBody->GetPos();
		item->data->physBody->primitive->SetPos(pos.x, pos.y, pos.z);
		item->data->physBody->primitive->Render();
	}
	
	// Draw car ====================================================
	playerCar->Render();

	// Rotation info -----------------------------

	btQuaternion q = playerCar->GetBody()->getWorldTransform().getRotation();
	mat4x4 t;
	btVector3 offset;

	playerCar->vehicle->getChassisWorldTransform().getOpenGLMatrix(&t);
	offset.setValue(0, 0.7f, 0);

	t[12] += offset.getX();
	t[13] += offset.getY();
	t[14] += offset.getZ();


	// Main chasis -------------------------------
	Cube chassis_1(1.5f, 0.6, 1);
	chassis_1.color = color_1;
	chassis_1.transform = t;

	offset.setValue(0, 0.2f, -0.9f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis_1.transform.M[12] += offset.getX();
	chassis_1.transform.M[13] += offset.getY();
	chassis_1.transform.M[14] += offset.getZ();

	Cube chassis_2(1.1f, 0.6, 1);
	chassis_2.color = color_1;
	chassis_2.transform = t;

	offset.setValue(0, 0.2f, 0.0f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis_2.transform.M[12] += offset.getX();
	chassis_2.transform.M[13] += offset.getY();
	chassis_2.transform.M[14] += offset.getZ();

	Cube chassis_3(0.8, 0.4, 1.3f);
	chassis_3.transform = t;
	chassis_3.color = color_1;

	offset.setValue(0, 0.1f, 1.1f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis_3.transform.M[12] += offset.getX();
	chassis_3.transform.M[13] += offset.getY();
	chassis_3.transform.M[14] += offset.getZ();

	// Front chasis ------------------------------
	Cube front_chasis(2.3f, 0.15f, 0.5f);
	front_chasis.color = color_2;
	front_chasis.transform = t;

	offset.setValue(0.0f, 0.1f, 2.0f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	front_chasis.transform.M[12] += offset.getX();
	front_chasis.transform.M[13] += offset.getY();
	front_chasis.transform.M[14] += offset.getZ();

	// Rear chasis ------------------------------
	Cube rear_chasis(2.1f, 0.15f, 0.5f);
	rear_chasis.color = color_2;
	rear_chasis.transform = t;

	offset.setValue(0.0f, 0.8f, -1.5f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	rear_chasis.transform.M[12] += offset.getX();
	rear_chasis.transform.M[13] += offset.getY();
	rear_chasis.transform.M[14] += offset.getZ();

	// Sticks  ---------------------------------
	Cube rear_stick_1(0.1, 0.2, 0.1);
	rear_stick_1.transform = t;
	rear_stick_1.color = color_1;

	offset.setValue(-0.3f, 0.6f, -1.3f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	rear_stick_1.transform.M[12] += offset.getX();
	rear_stick_1.transform.M[13] += offset.getY();
	rear_stick_1.transform.M[14] += offset.getZ();


	Cube rear_stick_2(0.1, 0.2, 0.1);
	rear_stick_2.transform = t;
	rear_stick_2.color = color_1;

	offset.setValue(0.3f, 0.6f, -1.3f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	rear_stick_2.transform.M[12] += offset.getX();
	rear_stick_2.transform.M[13] += offset.getY();
	rear_stick_2.transform.M[14] += offset.getZ();

	// Pilot  ---------------------------------
	Sphere pilot;
	pilot.radius = 0.2f;
	pilot.color = { 60.f / 255.f, 60.f / 255.f, 60.f / 255.f, 255.f / 255.f, };
	pilot.transform = t;

	offset.setValue(0.0f, 0.65f, -0.4f);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	pilot.transform.M[12] += offset.getX();
	pilot.transform.M[13] += offset.getY();
	pilot.transform.M[14] += offset.getZ();

	// Render -----------------------------------
	front_chasis.Render();
	rear_chasis.Render();
	rear_stick_1.Render();
	rear_stick_2.Render();
	chassis_1.Render();
	chassis_2.Render();
	chassis_3.Render();
	pilot.Render();

	return true;
}