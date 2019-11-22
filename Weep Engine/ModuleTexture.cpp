#include "ModuleTexture.h"
#include "App.h"
#include "ModuleGameObjectManager.h"
#include "GameObject.h"
#include "ComponentTexture.h"
#include "ModuleFileSystem.h"

#include "DevIL/il.h"
#include "DevIL/ilu.h"
#include "DevIL/ilut.h"//temporally all DevIL


#pragma comment ( lib, "DevIL/DevIl.lib")
#pragma comment ( lib, "DevIL/ILU.lib")
#pragma comment ( lib, "DevIL/ILUT.lib")

ModuleTexture::ModuleTexture(bool start_enabled) : Module(start_enabled)
{
	SetName("Texture");
}

bool ModuleTexture::Start()
{

	ilInit();
	iluInit();
	ilutInit();

	//ilutRenderer(ILUT_OPENGL);
	LoadCheckersTexture();
	dir = "Models/Textures/";
	return true;
}

bool ModuleTexture::CleanUp()
{
	for (std::vector<TextureInfo*>::iterator iter = textures_paths.begin(); iter != textures_paths.end(); ++iter)
	{
		RELEASE(*iter);
	}
	textures_paths.clear();

	return true;
}

void ModuleTexture::OnLoadFile(const char * file_path, const char * file_name, const char * file_extension)
{
	if (strcmp("png", file_extension) == 0 || strcmp("dds", file_extension) == 0)
	{
		for (std::vector<GameObject*>::iterator iter = App->game_object_manager->selected.begin(); iter != App->game_object_manager->selected.end(); ++iter)
		{
			bool to_load = true;
			std::vector<ComponentTexture*> textures = (*iter)->GetTextures();

			for (std::vector<ComponentTexture*>::iterator iter2 = textures.begin(); iter2 != textures.end(); ++iter2)
			{
				if ((*iter2)->texture_path == file_name)
				{
					LOG("there is a texture component with the same texture in this game object");
					to_load = false;;
				}
			}

			if (to_load)
			{
				ComponentTexture* text = (ComponentTexture*)(*iter)->AddComponent(ComponentType::TEXTURE);

				text->id_texture = LoadTexture(file_path, text->texture_width, text->texture_height);
				text->texture_path = file_name;
				text->ActivateThisTexture();
			}
		}
		if (App->game_object_manager->selected.size() == 0)
		{
			LOG("There is not a object selected to aply this texture. Please, select one Game Object first.");
		}
	}
}

uint ModuleTexture::LoadTexture(const char* path, int& width, int& height)
{
	uint ret = 0u;

	f_path = path;

	for (std::vector<TextureInfo*>::iterator iter = textures_paths.begin(); iter != textures_paths.end(); ++iter)
	{
		if (path == (*iter)->path)
		{
			LOG("The texture had already been loaded. returning saved texture...The texture was %s", path);
			width = (*iter)->width;
			height = (*iter)->height;
			return (*iter)->id;
		}
	}

	if (ilLoadImage(path))
	{
		LOG("Image Loaded correctly. The texture was %s", path);

		ret = ilutGLBindTexImage();
		if (ret > 0)
		{
		


		
			width = ilGetInteger(IL_IMAGE_WIDTH);
			height = ilGetInteger(IL_IMAGE_HEIGHT);

			LOG("Size texture: %i x %i", width, height);

			TextureInfo* new_texture = new TextureInfo();
			new_texture->width = width;
			new_texture->height = height;
			new_texture->id = ret;
			new_texture->path = path;
			textures_paths.push_back(new_texture);

			string name_file;
			
			ILuint size;
			ILubyte *data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
			size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
			string file(LIBRARY_TEXTURES_FOLDER + App->GetFileNameWithoutExtension(path) + ".dds");

			if (size > 0) {
				data = new ILubyte[size]; // allocate data buffer
				if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
					App->file_system->Save(file.c_str(), data, size);
				RELEASE_ARRAY(data);
			}

			ilDeleteImages(1, &ret);
		}
		else
		{
			LOG("Texture don't Loaded correctly")
		}
	}
	else
	{
		LOG("Image Don't loaded correctly. the path %s is not found", path);
	}

	return ret;
}

std::string ModuleTexture::GetPathTexture()
{
	return f_path;
}

void ModuleTexture::LoadCheckersTexture()
{
	checkersTexture = new ComponentTexture();
	GLubyte checkImage[checkImageHeight][checkImageWidth][4];
	for (int i = 0; i < checkImageHeight; i++) {
		for (int j = 0; j < checkImageWidth; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkersTexture->id_texture);
	glBindTexture(GL_TEXTURE_2D, checkersTexture->id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}

ComponentTexture* ModuleTexture::GetCheckersTexture() const
{
	return checkersTexture;
}