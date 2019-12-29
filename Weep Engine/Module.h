#pragma once

class Application;
enum class UIButtonType;
enum class UICheckBoxType;

using namespace std;

#include <string>
#include <list>
#include <queue>
#include <iostream>
#include <functional>


#include "SDL\include\SDL.h"

#include "jsoncpp/json/json.h"

class Module
{
public:
	Module(bool start_enabled = true) : enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Awake() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate(float dt)
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate(float dt)
	{
		return true;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void Save(Json::Value&) const {}

	virtual void Load(const Json::Value&) {}

	virtual void OnConfiguration()
	{

	}

	virtual bool ButtonEvent(const UIButtonType type)
	{
		return true;
	}

	virtual bool CheckBoxEvent(const UICheckBoxType type, const bool is_clicked)
	{
		return true;
	}

	void SetName(const char* set_name) { name = set_name; }
	const string GetName() const { return name; }
	bool GetEnabled() const { return enabled; };
	void SetEnabled(bool set) { enabled = set; };

	virtual void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension) {};


	const char *	name = "";
	bool			configuration = false;

private:

	bool			enabled = false;
};