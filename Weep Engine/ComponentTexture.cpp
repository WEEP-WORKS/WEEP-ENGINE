#include "ComponentTexture.h"


void ComponentTexture::SetBuffersWithData()
{
	if (uvs.has_data)
	{
		glGenBuffers(1, &uvs.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvs.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*uvs.buffer_size, uvs.buffer, GL_STATIC_DRAW);
	}
}
