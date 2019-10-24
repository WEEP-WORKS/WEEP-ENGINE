#ifndef __MODULETEXTURE_H__
#define __MODULETEXTURE_H__

#include "Module.h"
#include "Globals.h"

#define checkImageWidth 64
#define checkImageHeight 64
class ComponentTexture;
class ModuleTexture : public Module
{
public:

	ModuleTexture(bool start_enabled = true);
	~ModuleTexture() {};

	bool Start() override;
	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);

	uint LoadTexture(const char* path); // path without directory.

	ComponentTexture* GetCheckersTexture() const;

	std::string GetPathTexture();
	int Width;
	int Height;

private:
	std::string dir;
	std::string f_path;

	ComponentTexture* checkersTexture = 0u;

private: 

	void LoadCheckersTexture();
};

#endif // !__MODULETEXTURE_H__

