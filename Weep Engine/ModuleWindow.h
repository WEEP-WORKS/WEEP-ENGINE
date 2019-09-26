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

	void Save(Json::Value&) override;
	void Load(Json::Value&) override;

	void SetTitle(const char* title);

	int GetWidth() const;
	int GetHeight() const;
	float GetSize() const;
	string GetTitle() const;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	ImGuiIO io;

private:

	string title;
	int width = 0u;
	int height = 0u;
	float size = 0.0f;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;

};

#endif // __ModuleWindow_H__