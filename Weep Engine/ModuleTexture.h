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
		uint resource_id = 0;
	};

	ModuleTexture(bool start_enabled = true);
	~ModuleTexture() {};

	bool Start() override;
	bool CleanUp() override;
	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);

	void LoadTexture(const char* path, ComponentTexture*); // path without directory.

	void Load(const char * path, ComponentTexture * component_texture);

	ComponentTexture* GetCheckersTexture() const;

	std::string GetPathTexture();

public:
	int Width;
	int Height;

private:
	ComponentTexture* checkersTexture = 0u;
	std::vector<TextureInfo*> textures_paths;

private: 

	void LoadCheckersTexture();
};

#endif // !__MODULETEXTURE_H__

