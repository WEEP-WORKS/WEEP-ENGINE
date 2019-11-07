#pragma once

class Application;

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

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void Save(Json::Value&) {}

	virtual void Load(Json::Value&) {}

	virtual void OnConfiguration()
	{

	}


	void SetName(const char* set_name) { name = set_name; }
	string GetName() { return name; }
	bool GetEnabled() const { return enabled; };
	void SetEnabled(bool set) { enabled = set; };

	virtual void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension) {};


	const char *	name = "";
	bool			configuration = false;

private:

	bool			enabled = false;
};