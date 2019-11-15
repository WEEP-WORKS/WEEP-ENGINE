#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "Globals.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "DebugScene.h"

#include "glew/glew.h"
#include "SDL\include\SDL_opengl.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


#pragma comment (lib, "glew/glew32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	SetName("Renderer");
	configuration = true;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Awake()
{
	bool ret = true;


	if(ret == true)
	{
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Use Vsync
		if(SDL_GL_SetSwapInterval(vsync) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", __glewErrorStringREGAL(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", __glewErrorStringREGAL(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", __glewErrorStringREGAL(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f); // Color background


	// Projection matrix for
	OnResize(App->window->GetWidth(), App->window->GetHeight());

	//To get the refresh of the display
	int display_count = 0, display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) {
		LOG("SDL_GetDisplayMode failed: %s", SDL_GetError());
		return 1;
	}
	else
	{
		refresh_rate = mode.refresh_rate;
	}

	return ret;
}

bool ModuleRenderer3D::GetVsync() const
{
	return vsync;
}

int ModuleRenderer3D::GetRefreshRate() const
{
	return refresh_rate;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate()
{
	bool ret = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(0,0,0);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return ret;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	// Rendering ImGui
	ImGui::Render();
	glViewport(0, 0, (int)App->window->io.DisplaySize.x, (int)App->window->io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(App->window->window);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	return true;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	bool ret = true;

	LOG("Destroying ImGui");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();


	return ret;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	App->camera->GetCurrentCamera()->SetAspectRatio((float)width / (float)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(App->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawGrid(int HALF_GRID_SIZE)
{
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
	for (int i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; i++)
	{
		glVertex3f((float)i, 0, (float)-HALF_GRID_SIZE);
		glVertex3f((float)i, 0, (float)HALF_GRID_SIZE);

		glVertex3f((float)-HALF_GRID_SIZE, 0, (float)i);
		glVertex3f((float)HALF_GRID_SIZE, 0, (float)i);
	}
	glEnd();
	glColor3f(255.f, 255.f, 255.f);

}

void ModuleRenderer3D::Save(Json::Value& root)
{
	root[GetName()]["VSYNC"] = vsync;
}

void ModuleRenderer3D::Load(Json::Value& root)
{
	vsync = root[GetName()]["VSYNC"].asBool();
}

void ModuleRenderer3D::OnConfiguration()
{
	if (ImGui::Checkbox("Vsync", &vsync))
	{
		SetVsync(vsync);
		if (!vsync)
		{
			App->debug_scene->SetFpsMax();
			App->SetMaxFps(999);
		}
	}

	ImGui::Separator();

	ImGui::Checkbox("GL_DEPTH_TEST", &gl_depth);				
	ImGui::SameLine();
	ImGui::Checkbox("GL_CULL_FACE", &gl_cull_face);				
	ImGui::SameLine();
	ImGui::Checkbox("GL_LIGHTING", &gl_lighting);				

	ImGui::Checkbox("GL_COLOR_MATERIAL", &gl_color_material);
	ImGui::SameLine();
	ImGui::Checkbox("GL_TEXTURE_2D", &gl_texture_2d);
	ImGui::SameLine();
	ImGui::Checkbox("GL_BLEND", &gl_blend);
	ImGui::SameLine();
	ImGui::Checkbox("GL_SCISSOR_TEST", &gl_scissor);

	ImGui::Separator();

	ImGui::Checkbox("FILL RENDER", &fill_mode);					
	ImGui::SameLine();
	ImGui::Checkbox("WIREFRAME RENDER", &wireframe_mode);		
	ImGui::SameLine();
	ImGui::Checkbox("POINT RENDER", &point_mode);

	if (gl_lighting)
	{
		GLfloat	ambientProperties[] = { 1.f, 0.f, 0.f, 1.0f };

		if (ImGui::Button("Submit"))
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
			glEnable(GL_LIGHT0);
		}
	}

	if (gl_depth && glIsEnabled(GL_DEPTH_TEST) == false)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else if (gl_depth == false && glIsEnabled(GL_DEPTH_TEST))
	{
		glDisable(GL_DEPTH_TEST);
	}

	if (gl_blend && glIsEnabled(GL_BLEND) == false)						//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml
	{
		glEnable(GL_BLEND);
	}
	else if (gl_blend == false && glIsEnabled(GL_BLEND))
	{
		glDisable(GL_BLEND);
	}

	if (gl_scissor && glIsEnabled(GL_SCISSOR_TEST) == false)			// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glScissor.xml
	{
		glEnable(GL_SCISSOR_TEST);
	}
	else if (gl_scissor == false && glIsEnabled(GL_SCISSOR_TEST))
	{
		glDisable(GL_SCISSOR_TEST);
	}


	if (gl_cull_face && glIsEnabled(GL_CULL_FACE) == false)				// https://www.khronos.org/opengl/wiki/Face_Culling
	{
		glCullFace(GL_FRONT); 											//GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK.
		glEnable(GL_CULL_FACE);											// The latter will cull all triangles. Culling both faces will only cull triangles (since only they have faces).
	}
	else if (gl_cull_face == false && glIsEnabled(GL_CULL_FACE))
	{
		glDisable(GL_CULL_FACE);
	}

	if (gl_lighting && glIsEnabled(GL_LIGHTING) == false)				// https://www.khronos.org/opengl/wiki/How_lighting_works
	{
		glEnable(GL_LIGHTING);
	}
	else if (gl_lighting == false && glIsEnabled(GL_LIGHTING))
	{
		glDisable(GL_LIGHTING);
	}

	if (gl_color_material && glIsEnabled(GL_COLOR_MATERIAL) == false)	// https://www.khronos.org/opengl/wiki/How_lighting_works
	{
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
		glEnable(GL_COLOR_MATERIAL);
	}
	else if (gl_color_material == false && glIsEnabled(GL_COLOR_MATERIAL))
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	if (gl_texture_2d && glIsEnabled(GL_TEXTURE_2D) == false)			// https://www.khronos.org/opengl/wiki/Texture
	{
		glEnable(GL_TEXTURE_2D);
	}
	else if (gl_texture_2d == false && glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);

	// Fill 
	if (fill_mode && poly_mode != gl_fill)								// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPolygonMode.xml
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			
		poly_mode = gl_fill;									
		wireframe_mode = false;								
		point_mode = false;									
	} 														
	
	//Wireframe
	if (wireframe_mode && poly_mode != gl_line)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		poly_mode = gl_line;
		fill_mode = false;
		point_mode = false;
	}

	// Point											
	if (point_mode && poly_mode != gl_point)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		poly_mode = gl_point;
		fill_mode = false;
		wireframe_mode = false;
	}

	// Slider Point
	if (point_mode)
	{
		ImGui::SliderFloat("Point Size", &point_size_slider, 0, 10);
		glPointSize(point_size_slider);
	}
}

void ModuleRenderer3D::SetVsync(bool set)
{
	vsync = set;
	SDL_GL_SetSwapInterval(vsync);
}

