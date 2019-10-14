#ifndef __COMPONENTTEXTURE_H__
#define __COMPONENTTEXTURE_H__

#include "Component.h"


class ComponentTexture : public Component
{
public:
	ComponentTexture() {};
	void SetBuffersWithData();
	
public:

	//UVs
	uint num_uvs_channels = 0u; //local var?
	uint channel_buffer_size = 0u; //local var?

	BuffersData<float> uvs;

	//Textures
	bool has_texture = false;
	GLuint id_texture = 0;

};


#endif // !__COMPONENTTEXTURE_H__

