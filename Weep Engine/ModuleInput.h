#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Awake();
	bool PreUpdate(float dt);
	bool CleanUp();

	void OnConfiguration();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	bool GetWindowEvent(EventWindow ev);

	float2 GetMouse();

	void BufferInput(int key, KEY_STATE state, bool mouse);

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

private:

	bool	   windowEvents[WE_COUNT];

	KEY_STATE* keyboard;
	KEY_STATE  mouse_buttons[MAX_MOUSE_BUTTONS];

	int	       mouse_x;
	int	       mouse_y;
	int	       mouse_z;
	int	       mouse_x_motion;
	int	       mouse_y_motion;
	//int	   mouse_z_motion;

	ImGuiTextBuffer input_buff;

};