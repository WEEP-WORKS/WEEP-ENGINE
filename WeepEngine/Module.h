#pragma once

#include "Application.h"

class Application;
struct PhysBody3D;

using namespace std;

#include <string>
#include <list>
#include <queue>
#include <iostream>

#include "SDL\include\SDL.h"

class Module
{
public:
	Module(bool start_enabled = true) : enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Init()
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

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	void SetName(const char* set_name) { name = set_name; }
	bool GetEnabled() { return enabled; };
	void SetEnabled(bool set) { enabled = set; };

private:
	string name = "";
	bool   enabled = false;
};