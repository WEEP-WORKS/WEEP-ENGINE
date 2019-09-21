#pragma once

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;


class DebugScene : public Module
{
public:
	DebugScene(bool start_enabled = true);
	~DebugScene();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	// Our state
	bool show_demo_window = false;
	//bool show_another_window = false;

	// Main loop
	bool done = false;





public:


};
