#include "Globals.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleImporter.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "ModuleWindow.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);

	SetName("Input");
	configuration = true;
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Awake()
{
	bool ret = true;

	LOG("Init SDL input event system");
	

	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
bool ModuleInput::PreUpdate()
{
	bool ret = true;

	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
				BufferInput(i, KEY_DOWN,false);
			}
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
			}
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->GetSize();
	mouse_y /= App->window->GetSize();
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
			{
				mouse_buttons[i] = KEY_DOWN;
				BufferInput(i + 1, KEY_DOWN, true);
			}
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
			{
				mouse_buttons[i] = KEY_UP;
			}
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;

			case SDL_WINDOWEVENT_RESIZED:
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			break;

			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->GetSize();
			mouse_y = e.motion.y / App->window->GetSize();

			mouse_x_motion = e.motion.xrel / App->window->GetSize();
			mouse_y_motion = e.motion.yrel / App->window->GetSize();
			break;

			case SDL_DROPFILE:
				string file = e.drop.file;
				App->LoadFile(file.c_str());
				SDL_free(e.drop.file);
			break;
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return false;

	return ret;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	bool ret = true;

	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return ret;
}

void ModuleInput::BufferInput(int key, KEY_STATE state, bool mouse)
{
	std::string output_string;
	std::string states[] = { "IDLE","DOWN","REPEAT","UP" };

	if (mouse == false) 
	{
		output_string = "Keyboard: " + std::to_string(key) + " " + states[state] + "\n";
	}
	else {
		output_string = "Mouse: " + std::to_string(key) + " " + states[state] + "\n";
	}

	input_buff.appendf(output_string.c_str());
}

void ModuleInput::OnConfiguration()
{
	ImGui::Text("Mouse position:");
	ImGui::SameLine();
	std::string mouse_log = std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY());
	ImGui::TextColored({ 0,1.0f,1.0f,1.0f }, mouse_log.c_str());

	ImGui::Text("Mouse motion:");
	ImGui::SameLine();
	mouse_log = std::to_string(GetMouseXMotion()) + ", " + std::to_string(GetMouseYMotion());
	ImGui::TextColored({ 0,1.0f,1.0f,1.0f }, mouse_log.c_str());

	ImGui::Text("Mouse wheel:");
	ImGui::SameLine();
	mouse_log = std::to_string(App->input->GetMouseZ());
	ImGui::TextColored({ 0,1.0f,1.0f,1.0f }, mouse_log.c_str());
	ImGui::Separator();

	ImGui::BeginChild("Buffer");
	ImGui::TextUnformatted(input_buff.begin());
	ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
}

bool ModuleInput::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

float2 ModuleInput::GetMouse()
{
	return float2(mouse_x, mouse_y);
}
