#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#define MAX_ACCELERATION 700.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 100.0F

#define TIME_TO_DELETE_MISSILE 1000.0f

struct PhysVehicle3D;
class Timer;

class Missile
{
public:
	Missile(ModulePlayer* owner);
	void Update();

private:

	bool toDelete = false;
	PhysBody3D* physBody = nullptr;
	ModulePlayer* owner = nullptr;
	Timer timerToDelete;
	
	friend class ModulePlayer;
};

class Missile;

enum class PlayerState 
{
	winner,
	loser,
	dead,
	alive
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, uint playerNum, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	bool Draw();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void AddMissile();
	void AddAmmo();
	void Reset();
	int  GetLifes();
	int  GetAmmo();
	void  SetLifes(int lifes);
	void  SetAmmo(int ammo);

	PhysVehicle3D* GetPlayerCar();

public:
	vec3 initPosition;

	Color firstColor;
	Color secondColor;
	Color deadColor;
	Color winnerColor;
	Color loserColor;

private:
	PlayerState state = PlayerState::alive;
	int  lifes = PLAYER_INIT_LIFES;
	int  ammo = 0;
	uint playerNum = 0u;

	PhysVehicle3D* playerCar = nullptr;
	p2List<Missile*> missiles;

	float turn;
	float acceleration;
	float brake;

	uint explosion_fx;
	uint shoot_fx;
};