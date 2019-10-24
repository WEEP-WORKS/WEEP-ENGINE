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

void ModuleTexture::OnLoadFile(const char * file_path, const char * file_name, const char * file_extension)
{
	if(strcmp("png",file_extension) == 0 || strcmp("dds", file_extension) == 0)
	{

		for (std::vector<GameObject*>::iterator iter = App->game_object_manager->selected.begin(); iter != App->game_object_manager->selected.end(); ++iter)
		{
			ComponentTexture* text = (ComponentTexture*)(*iter)->AddComponent(ComponentType::TEXTURE);

			text->id_texture = LoadTexture(file_name);
			text->ActivateThisTexture();
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