#ifndef __MODULETEXTURE_H__
#define __MODULETEXTURE_H__

#include "Module.h"
#include "Globals.h"

class ModuleTexture : public Module
{
public:

	ModuleTexture() {};
	~ModuleTexture() {};

	bool Start() override;

	uint LoadTexture(const char* path); // path without directory.

private:
	std::string dir;
};

#endif // !__MODULETEXTURE_H__

