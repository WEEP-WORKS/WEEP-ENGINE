#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "imgui.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();
	void OnConfiguration();

	void Save(Json::Value&) override;
	void Load(Json::Value&) override;

	void OnResize(int width, int height);
	void DrawGrid(int HALF_GRID_SIZE);

public:

	Light lights[MAX_LIGHTS];
	//SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	
	ImVec4 clear_color;

private:

	bool vsync = false;

private:

	void SetVsync(bool set);

};