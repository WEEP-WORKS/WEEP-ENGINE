#include "Globals.h"
#include "App.h"
#include "ModuleWindow.h"
#include "glew/glew.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#pragma comment (lib, "glew/glew32.lib")

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	SetName("Window");
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Awake()
{
	bool ret = true;

	LOG("Init SDL window & surface");
	

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int new_width = width * size;
		int new_height = height * size;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fullscreen_desktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, new_width, new_height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}

		SDL_GLContext gl_context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, gl_context);
		SDL_GL_SetSwapInterval(1); // Enable vsync

		// Setup Dear ImGui context
		glewInit();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init();
		
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	bool ret = true;

	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();

	return ret;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

int ModuleWindow::GetWidth() const
{
	return width;
}

int ModuleWindow::GetHeight() const
{
	return height;
}

float ModuleWindow::GetSize() const
{
	return size;
}

string ModuleWindow::GetTitle() const
{
	return title;
}

void ModuleWindow::Save(Json::Value& root)
{
	root[GetName()]["Title"]				= title;
	root[GetName()]["Width"]				= width;
	root[GetName()]["Height"]				= height;
	root[GetName()]["Size"]					= size;
	root[GetName()]["Fullscreen"]			= fullscreen;
	root[GetName()]["Resizable"]			= resizable;
	root[GetName()]["Borderless"]			= borderless;
	root[GetName()]["Fullscreen_desktop"]	= fullscreen_desktop;
}

void ModuleWindow::Load(Json::Value& root)
{
	title = root[GetName()]["Title"].asString();
	width = root[GetName()]["Width"].asInt();
	height = root[GetName()]["Height"].asInt();
	size = root[GetName()]["Size"].asFloat();
	fullscreen = root[GetName()]["Fullscreen"].asBool();
	resizable = root[GetName()]["Resizable"].asBool();
	borderless = root[GetName()]["Borderless"].asBool();
	fullscreen_desktop = root[GetName()]["Fullscreen_desktop"].asBool();
}

