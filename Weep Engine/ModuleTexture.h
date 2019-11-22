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

	struct TextureInfo
	{
		uint id = 0;
		string path;
		uint width;
		uint height;
	};

	ModuleTexture(bool start_enabled = true);
	~ModuleTexture() {};

	bool Start() override;
	bool CleanUp() override;
	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);

	uint LoadTexture(const char* path, int& width, int& height); // path without directory.

	ComponentTexture* GetCheckersTexture() const;

	std::string GetPathTexture();

public:
	int Width;
	int Height;

private:
	std::string dir;
	std::string f_path;

	ComponentTexture* checkersTexture = 0u;
	std::vector<TextureInfo*> textures_paths;

private: 

	void LoadCheckersTexture();
};

#endif // !__MODULETEXTURE_H__

