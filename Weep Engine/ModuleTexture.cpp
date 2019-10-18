#include "ModuleTexture.h"


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

	dir = "Models/Textures/";
	return true;
}

void ModuleTexture::OnLoadFile(const char * file_path, const char * file_name, const char * file_extension)
{
	if(strcmp("png",file_extension) == 0)
	{
		LoadTexture(file_name);
	}
}

uint ModuleTexture::LoadTexture(const char* path)
{
	uint ret = 0u;

	f_path = dir + path;

	if (ilLoadImage(f_path.c_str()))
	{
		LOG("Image Loaded correctly");
		Width = ilGetInteger(IL_IMAGE_WIDTH);
		Height = ilGetInteger(IL_IMAGE_HEIGHT);

		ret = ilutGLBindTexImage();
		if (ret > 0)
		{
			//glBindTexture(GL_TEXTURE_2D, model->id_texture);   test with loading more modesl with texture TODO
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