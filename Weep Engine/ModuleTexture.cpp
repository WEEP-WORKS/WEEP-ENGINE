#include "ModuleTexture.h"
#include "App.h"
#include "ModuleGameObjectManager.h"
#include "GameObject.h"
#include "ComponentTexture.h"

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

				text->id_texture = LoadTexture(file_name);
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

uint ModuleTexture::LoadTexture(const char* path)
{
	uint ret = 0u;

	f_path = dir + path;

	//for (list<GameObject*>::iterator iter = App->game_object_manager->objects.begin(); iter != App->game_object_manager->objects.end(); ++iter)
	//{
	//	std::vector<ComponentTexture*> textures = (*iter)->GetTextures();
	//	for (std::vector<ComponentTexture*>::iterator iter_text = textures.begin(); iter_text != textures.end(); ++iter_text)
	//	{
	//		if ((*iter_text)->texture_path == path)
	//		{
	//			return (*iter_text)->id_texture;
	//		}
	//	}
	//}

	for (std::vector<TextureInfo*>::iterator iter = textures_paths.begin(); iter != textures_paths.end(); ++iter)
	{
		if (path == (*iter)->path)
		{
			LOG("The texture had already been loaded. returning saved texture...")
			return (*iter)->id;
		}
	}

	if (ilLoadImage(f_path.c_str()))
	{
		LOG("Image Loaded correctly");

		ret = ilutGLBindTexImage();
		if (ret > 0)
		{
			Width = ilGetInteger(IL_IMAGE_WIDTH);
			Height = ilGetInteger(IL_IMAGE_HEIGHT);

			TextureInfo* new_texture = new TextureInfo();
			new_texture->id = ret;
			new_texture->path = path;
			textures_paths.push_back(new_texture);

			ilDeleteImages(1, &ret);
		}
		else
		{
			LOG("Texture don't Loaded correctly")
		}
	}
	else
	{
		LOG("Image Don't loaded correctly");
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