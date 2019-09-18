#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"
#include "ModulePhysics3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->audio->PlayMusic("music/racetrack.ogg");
	reload_fx = App->audio->LoadFx("sfx/reload.wav");
	ready_go_fx = App->audio->LoadFx("sfx/readysetgo.wav");
	// Stage Primitives ------------------------------ !!	Add all to stagePrimitives List

	// RAMPS -------------------------------------------------

	Cube rampHLB(10, 0.5f, 9); //Ramp HigherLeftBottom
	rampHLB.SetPos(25, 0, -65);
	rampHLB.SetRotation(-30, vec3(1, 0, 0));
	rampHLB.color = { 255/255,0,255/255,255/255 };
	stagePrimitives.add(App->physics->AddBody(rampHLB, 0.0f));

	Cube rampHRB(10, 0.5f, 9); //Ramp HigherRightBottom
	rampHRB.SetPos(-25, 0, -65);
	rampHRB.SetRotation(-30, vec3(1, 0, 0));
	rampHRB.color = { 0,255/255,255 / 255,255 / 255 };
	stagePrimitives.add(App->physics->AddBody(rampHRB, 0.0f));

	Cube rampHLU(10, 0.5f, 9); //Ramp HigherLeftUpper
	rampHLU.SetPos(25, 0, 65);
	rampHLU.SetRotation(30, vec3(1, 0, 0));
	rampHLU.color = { 255 / 255,0,255 / 255,255 / 255 };
	stagePrimitives.add(App->physics->AddBody(rampHLU, 0.0f));

	Cube rampHRU(10, 0.5f, 9); //Ramp HigherRightUpper
	rampHRU.SetPos(-25, 0, 65);
	rampHRU.SetRotation(30, vec3(1, 0, 0));
	rampHRU.color = { 0,255 / 255,255 / 255,255 / 255 };
	stagePrimitives.add(App->physics->AddBody(rampHRU, 0.0f));

	Cube rampLB(10, 0.5f, 9); //Ramp LowerBottom
	rampLB.SetPos(0, 0, -25);
	rampLB.SetRotation(-15, vec3(1, 0, 0));
	rampLB.color = { 0,128/255,255 / 255,255 / 255 };
	stagePrimitives.add(App->physics->AddBody(rampLB, 0.0f));

	Cube rampLU(10, 0.5f, 9); //Ramp LowerUpper
	rampLU.SetPos(0, 0, 25);
	rampLU.SetRotation(15, vec3(1, 0, 0));
	rampLU.color = { 0, 128 / 255,255 / 255,255 / 255 };
	stagePrimitives.add(App->physics->AddBody(rampLU, 0.0f));

	Cube rampLL(10, 0.5f, 9); //Ramp LowerLeft
	rampLL.SetPos(13, 0, 0);
	rampLL.SetRotation(-15, vec3(0, 0, 1));
	rampLL.color = { 0,128 / 255,255 / 255,0.5f };
	stagePrimitives.add(App->physics->AddBody(rampLL, 0.0f));

	Cube rampLR(10, 0.5f, 9); //Ramp LowerRight
	rampLR.SetPos(-13, 0, 0);
	rampLR.SetRotation(15, vec3(0, 0, 1));
	rampLR.color = { 0,128 / 255,255 / 255,0.5f };
	stagePrimitives.add(App->physics->AddBody(rampLR, 0.0f));

	// PLATFORMS -------------------------------------------------
	
	Cube platformL(10, 0.5f, 122); //Platform Left
	platformL.SetPos(25, 2.25f, 0);
	platformL.color = { 255/255,0,255/255,0.5f };
	stagePrimitives.add(App->physics->AddBody(platformL, 0.0f));

	Cube platformR(10, 0.5f, 122); //Platform Right
	platformR.SetPos(-25, 2.25f, 0);
	platformR.color = { 0,255/255 ,255 / 255,0.5f };
	stagePrimitives.add(App->physics->AddBody(platformR, 0.0f));

	Cube platformMV(10, 3, 41); //Platform MiddleVertical
	platformMV.SetPos(0, 0, 0);
	platformMV.color = { 0,128/255,255 / 255,0.5f };
	stagePrimitives.add(App->physics->AddBody(platformMV, 0.0f));

	Cube platformMH(9, 3, 16); //Platform MiddleHoritzontal
	platformMH.SetPos(0, 0, 0);
	platformMH.color = { 0,128 / 255,255 / 255,0.5f };
	platformMH.SetRotation(90, vec3(0, 1, 0));
	stagePrimitives.add(App->physics->AddBody(platformMH, 0.0f));

	// Turbine 1----------------------------------------------

	Cube axis(1, 3, 1.0f);
	axis.SetPos(0, 1, -65);
	axis.color = { 255 / 255, 0, 0, 0.2f };

	PhysBody3D* body1;
	body1 = App->physics->AddBody(axis, 0.0f);
	stagePrimitives.add(body1);

	Cube helix(10, 2, 1);
	helix.SetPos(-17.5, 3.5f, -50);
	helix.color = { 255 / 255,0,0,255 / 255 };

	PhysBody3D* body2;
	body2 = App->physics->AddBody(helix, 1.0f);
	stagePrimitives.add(body2);

	btHingeConstraint* hinge = App->physics->AddConstraintHinge(*body1, *body2, vec3{ 0,0,0 }, vec3{ 5,0,0 }, vec3{ 0,1,0 }, vec3{ 0,1,0 }, true);
	hinge->setLimit(1.0f, 0);

	// Turbine 2----------------------------------------------

	Cube axis2(1,3,1);
	axis2.SetPos(0, 1, 65);
	axis2.color = { 255 / 255, 0, 0, 0.2f };
	
	PhysBody3D* body3;
	body3 = App->physics->AddBody(axis2, 0.0f);
	stagePrimitives.add(body3);

	Cube helix2(10, 2, 1);
	helix2.SetPos(-17.5, 3.5f, 50);
	helix2.color = { 255 / 255,0,0,255 / 255 };

	PhysBody3D* body4;
	body4 = App->physics->AddBody(helix2, 1.0f);
	stagePrimitives.add(body4);

	btHingeConstraint* hinge2 = App->physics->AddConstraintHinge(*body3, *body4, vec3{ 0,0,0 }, vec3{ 5, 0,0 }, vec3{ 0,1,0 }, vec3{ 0,1,0 }, true);
	hinge2->setLimit(1.0f, 0);

	// BORDERS --------------------------------------------------------
	Color colorBorders(1.0f, 1.0f, 0, 0.5f);

	Cube border1(100, 10, 1); //spawn player 1
	border1.SetPos(0, 0, 100);
	border1.color = colorBorders;
	stagePrimitives.add(App->physics->AddBody(border1, 0.0f));

	Cube border2(1, 10, 200); //right player 2
	border2.SetPos(-50, 0, 0);
	border2.color = colorBorders;
	stagePrimitives.add(App->physics->AddBody(border2, 0.0f));

	Cube border3(100, 10, 1); //spawn player 2
	border3.SetPos(0, 0, -100);
	border3.color = colorBorders;
	stagePrimitives.add(App->physics->AddBody(border3, 0.0f));

	Cube border4(1, 10, 200); //left player 2
	border4.SetPos(50, 0, 0);
	border4.color = colorBorders;
	stagePrimitives.add(App->physics->AddBody(border4, 0.0f));


	// Power Up Spawners  ------------------------------ 

	AddPowerUpSpawner(vec3(0, 3, 0)); //Middle
	AddPowerUpSpawner(vec3(-40, 1.5f, 0)); //MiddleRight
	AddPowerUpSpawner(vec3(40, 1.5f, 0)); //MiddleLeft
	AddPowerUpSpawner(vec3(40, 1.5f, 90)); //Border UpperLeft
	AddPowerUpSpawner(vec3(-40, 1.5f, 90)); //Border UpperRight
	AddPowerUpSpawner(vec3(40, 1.5f, -90)); //Border BottomLeft
	AddPowerUpSpawner(vec3(-40, 1.5f, -90)); //Border BottomRight
	AddPowerUpSpawner(vec3(25, 4, 0)); //Platform Left
	AddPowerUpSpawner(vec3(-25, 4, 0)); //Platform Right

	App->audio->PlayFx(ready_go_fx);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	// Delete all stage primitives -----------------------------------
	for (p2List_item<PhysBody3D*> *item = stagePrimitives.getFirst(); item; item = item->next)
	{
		App->physics->DeleteBody(item->data);
	}

	stagePrimitives.clear();


	// Delete all power up spawners ------------------------------------------
	for (p2List_item<PowerUpSpawner*> *item = stageSpawners.getFirst(); item; item = item->next)
	{
		delete item->data;
	}

	stageSpawners.clear();

	// Delete all power ups ------------------------------------------
	for (p2List_item<PowerUp*> *item = powerUps.getFirst(); item; item = item->next)
	{
		App->physics->DeleteBody(item->data->sensor);
		delete item->data;
	}

	powerUps.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// If game ends reset with enter ------------------------
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && haveWinner == true)
	{
		ResetGame();
		haveWinner = false;
	}

	// Reset ------------------------------------------------

	if (sceneToReset == true && playerDeadTimer.Read() > BTW_ROUNDS_TIME)
	{
		ResetScene();
		sceneToReset = false;
	}

	// Update Power Up Spawners ---------------------------------

	for (p2List_item<PowerUpSpawner*> *item = stageSpawners.getFirst(); item; item = item->next)
	{
		item->data->Update(dt);
	}

	// Update Power Ups -----------------------------------------

	p2List_item<PowerUp*> *item = powerUps.getFirst();
	p2List_item<PowerUp*> *iterator = nullptr;


	while (item != nullptr)
	{
		item->data->Update(dt);

		if (item->data->toDelete == true)
		{
			iterator = item->next;
			App->physics->DeleteBody(item->data->sensor);
			delete item->data;
			powerUps.del(item);
			item = iterator;
		}
		else
		{
			item = item->next;
		}
	}

	// Update title --------------------------------------------
	char title[200];

	if (haveWinner == false)
	{
		sprintf_s(title, "(( PLAYER 1    Ammo : %i  Lifes : %i ))   (( PLAYER 2    Ammo : %i  Lifes : %i ))"
			, App->player_1->GetAmmo(), App->player_1->GetLifes(), App->player_2->GetAmmo(), App->player_2->GetLifes());
	}
	else
	{
		sprintf_s(title, "PRESS SPACE TO RESTART");
	}

	App->window->SetTitle(title);


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	PhysVehicle3D* player_1 = App->player_1->GetPlayerCar();
	PhysVehicle3D* player_2 = App->player_2->GetPlayerCar();

	if (player_1 == body2 || player_2 == body2)
	{
		PowerUp* powerUp = nullptr;

		for (p2List_item<PowerUp*>* item = powerUps.getFirst(); item; item = item->next)
		{
			if (item->data->sensor == body1)
			{
				powerUp = item->data;
			}
		}

		if (powerUp == nullptr)
		{
			return;
		}

		powerUp->spawner->StartRespawnTime();
		powerUp->toDelete = true;

		if (player_1 == body2)
		{
			LOG("Added 3 ammo player 1");
			App->player_1->AddAmmo();
			App->audio->PlayFx(reload_fx);
		}
		else if (player_2 == body2)
		{
			LOG("Added 3 ammo player 2");
			App->player_2->AddAmmo();
			App->audio->PlayFx(reload_fx);
		}
	}

}

bool ModuleSceneIntro::ResetScene()
{
	App->audio->PlayFx(ready_go_fx);
	App->player_1->Reset();
	App->player_2->Reset();
	return true;
}

bool ModuleSceneIntro::ResetGame()
{
	App->player_1->SetLifes(PLAYER_INIT_LIFES);
	App->player_2->SetLifes(PLAYER_INIT_LIFES);
	ResetScene();
	
	return true;
}

bool ModuleSceneIntro::StartAfterDeadTimer()
{
	playerDeadTimer.Start();
	sceneToReset = true;

	return true;
}

bool ModuleSceneIntro::AddPowerUpSpawner(vec3 position)
{
	PowerUpSpawner* spawner = new PowerUpSpawner(position, App);
	stageSpawners.add(spawner);
	return true;
}

bool ModuleSceneIntro::Draw()
{

	// Render ground -------------------------------------------------
	Cube ground(100, 2, 200);
	ground.SetPos(0, -1, 0);
	ground.Render();

	// Render all power ups ------------------------------------------
	for (p2List_item<PowerUp*> *item = powerUps.getFirst(); item; item = item->next)
	{
		item->data->Render();
	}


	// Render all stage primitives -----------------------------------
	for (p2List_item<PhysBody3D*> *item = stagePrimitives.getFirst(); item; item = item->next)
	{
		mat4x4 t;
		item->data->GetBody()->getWorldTransform().getOpenGLMatrix(&t);
		item->data->primitive->transform = t;
		item->data->primitive->Render();
	}

	return true;
}

PowerUp::PowerUp(vec3 position, Application * app): position(position)
{
	Cube sensor_cube(1.5f, 1.5f, 1.5f);
	sensor_cube.SetPos(position.x, position.y, position.z);
	sensor = app->physics->AddBody(sensor_cube, 0.0f);
	sensor->collision_listeners.add(app->scene_intro);
	sensor->SetAsSensor(true);
}

PowerUp::~PowerUp()
{
}

void PowerUp::Render()
{
	Cube cube(1.0f,1.0f,1.0f);
	cube.color = Green;
	cube.SetPos(sensor->GetPos().x, sensor->GetPos().y, sensor->GetPos().z);
	cube.SetRotation(angle, vec3(1, 1, 0));
	cube.Render();
}

void PowerUp::Update(float dt)
{
	angle = angle +  0.7f;
}

PowerUpSpawner::PowerUpSpawner(vec3 position, Application * app): position(position), App(app)
{
	AddPowerUp(position);
	powerUpTaked = false;

}

PowerUpSpawner::~PowerUpSpawner()
{
}

bool PowerUpSpawner::AddPowerUp(vec3 position)
{
	PowerUp* power = new PowerUp(position, App);
	power->spawner = this;
	App->scene_intro->powerUps.add(power);
	
	return true;
}


void PowerUpSpawner::StartRespawnTime()
{
	spawnTimer.Start();
	powerUpTaked = true;
}

void PowerUpSpawner::Update(float dt)
{
	if (spawnTimer.Read() > RESPAWN_TIME && powerUpTaked == true)
	{
		AddPowerUp(position);
		powerUpTaked = false;
	}
}
