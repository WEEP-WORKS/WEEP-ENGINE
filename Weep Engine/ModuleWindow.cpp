#include "Globals.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "glew/glew.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#pragma comment (lib, "glew/glew32.lib")

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;

	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	fullscreen = WIN_FULLSCREEN;
	resizable = WIN_RESIZABLE;
	borderless = WIN_BORDERLESS;
	full_dekstop = WIN_FULLSCREEN_DESKTOP;
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
	SetName("Window");

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

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

void ModuleWindow::OnConfiguration()
{
	if (ImGui::SliderInt("Width", &width, 720, 1920))
	{
		SetWindowSize(width, height);
	}
	if (ImGui::SliderInt("Height", &height, 480, 1080))
	{
		SetWindowSize(width, height);
	}

	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		SetFullscreen(fullscreen);
	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &resizable))
		SetResizable(resizable);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Restart to apply");
	
	if (ImGui::Checkbox("Borderless", &borderless))
		SetBorderless(borderless);
	ImGui::SameLine();
	if (ImGui::Checkbox("Full Desktop", &full_dekstop))
		SetFullDekstop(full_dekstop);

}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}


void ModuleWindow::SetWindowSize(int _width, int _height)
{
	if (_width > 0 && _height > 0)
	{
		width = _width;
		height = _height;
		SDL_SetWindowSize(window, width, height);

		if (!fullscreen)
			App->renderer3D->OnResize(width, height);
	}
}

void ModuleWindow::SetFullscreen(bool set)
{
	fullscreen = set;

	Uint32 flags;

	if (fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::SetResizable(bool set)
{
	resizable = set;
}

void ModuleWindow::SetBorderless(bool set)
{
	if (!fullscreen && !full_dekstop)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!set);
	}
}

void ModuleWindow::SetFullDekstop(bool set)
{
	full_dekstop = set;

	if (full_dekstop)
	{
		SetFullscreen(false);

		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			full_dekstop = false;
		}
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}