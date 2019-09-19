#pragma once

#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2List2.h"
#include "imgui.h"
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
	bool show_demo_window = true;
	bool show_another_window = false;

	// Main loop
	bool done = false;



	ImGuiIO io;
	ImVec4 clear_color;

public:


};
