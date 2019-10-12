#include "ModuleTexture.h"


#include "DevIL/il.h"
#include "DevIL/ilu.h"
#include "DevIL/ilut.h"//temporally all DevIL


#pragma comment ( lib, "DevIL/DevIl.lib")
#pragma comment ( lib, "DevIL/ILU.lib")
#pragma comment ( lib, "DevIL/ILUT.lib")

bool ModuleTexture::Start()
{
	ilInit();
	iluInit();
	ilutInit();

	//ilutRenderer(ILUT_OPENGL);

	dir = "Models/Textures/";
	return true;
}

uint ModuleTexture::LoadTexture(const char* path)
{
	uint ret = 0u;

	std::string f_path = dir + path;

	if (ilLoadImage(f_path.c_str()))
	{
		LOG("Image Loaded correctly");


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