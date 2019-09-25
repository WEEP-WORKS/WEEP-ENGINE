#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "imgui.h"
class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Awake();
	bool CleanUp();
	void OnConfiguration();

	void SetTitle(const char* title);

	void SetWindowSize(int width, int height);
	void SetFullscreen(bool set);
	void SetResizable(bool set);
	void SetBorderless(bool set);
	void SetFullDekstop(bool set);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	ImGuiIO io;

private:

	int     width = 0;
	int     height = 0;
	bool    fullscreen = false;
	bool    resizable = false;
	bool    borderless = false;
	bool    full_dekstop = false;

};

#endif // __ModuleWindow_H__