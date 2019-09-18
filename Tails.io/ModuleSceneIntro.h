#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2
#define PU_LEVITATION_OFFSET 0.4F
#define RESPAWN_TIME 10000.F
#define BTW_ROUNDS_TIME 4000.F
#define PLAYER_INIT_LIFES 5

struct PhysBody3D;
struct PhysVehicle3D;
struct PhysMotor3D;
class ModuleSceneIntro;
class PowerUpSpawner;

class PowerUp
{
public:
	PowerUp(vec3 position, Application* app);
	~PowerUp();

	void Render();
	void Update(float dt);
private:
	vec3 position;
	bool toDelete = false;
	PhysBody3D* sensor = nullptr;
	PowerUpSpawner* spawner = nullptr;
	float angle = 0.0f;

private:
	friend ModuleSceneIntro;
	friend PowerUpSpawner;
};

class PowerUpSpawner
{
public:
	PowerUpSpawner(vec3 position, Application* app);
	~PowerUpSpawner();

	void StartRespawnTime();
	void Update(float dt);

private:
	bool AddPowerUp(vec3 position);

private:
	bool  powerUpTaked = false;
	Timer spawnTimer;
	vec3  position;
	Application* App;
private:
	friend ModuleSceneIntro;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Draw();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	bool ResetScene();
	bool ResetGame();
	bool StartAfterDeadTimer();

private:
	bool AddPowerUpSpawner(vec3 position);

public:
	bool haveWinner = false;

private:
	p2List<PowerUpSpawner*>  stageSpawners;
	p2List<PhysBody3D*>      stagePrimitives;
	p2List<PowerUp*>         powerUps;
	Timer                    playerDeadTimer;

	uint reload_fx = 0;
	uint ready_go_fx = 0;

	bool sceneToReset = false;

	friend PowerUpSpawner;

};
