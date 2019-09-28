#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "imgui.h"

#define MAX_LIGHTS 8

enum polygon_mode
{
	gl_fill,
	gl_line,
	gl_point
};

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
	// OpenGL Options
	bool gl_blend = false;
	bool gl_depth = true;
	bool gl_scissor = false;
	bool gl_cull_face = false;
	bool gl_lighting = false;
	bool gl_color_material = true;
	bool gl_texture_2d = false;
	bool fill_mode = true;
	bool wireframe_mode = false;
	bool point_mode = false;

	polygon_mode poly_mode = polygon_mode::gl_fill;

	float point_size_slider = 1;

private:

	void SetVsync(bool set);

};