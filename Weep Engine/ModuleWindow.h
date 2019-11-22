#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "imgui.h"
#include "glmath.h"
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

	void Save(Json::Value&) const override;
	void Load(const Json::Value&) override;

	void SetTitle(const char* title);
	string GetTitle() const;

	int GetWidth() const;
	int GetHeight() const;
	float GetSize() const;

	string GetTitleWithVersion() const;
	void SetAppName(string name);

	void SetAppOrganization(const char* name);
	const char* GetAppOrganization();

	void SetVersion(const char * set);
	const Rect GetWindowRect() const;
	string GetVersion() const;

	void GetWindowSize(int & width, int & height);
	const vec2 GetWindowSizeVec();

	void SetWindowSize(int width, int height);
	void SetBrightness(float set);
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

	string title;
	string version;
	string organization;

	SDL_GLContext gl_context;

	int     width = 0u;
	int     height = 0u;
	float	brightness = 1.0f;
	float	size = 0.0f;
	bool    fullscreen = false;
	bool    resizable = false;
	bool    borderless = false;
	bool    full_dekstop = false;

};

#endif // __ModuleWindow_H__