#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "glew/glew.h"
#include "Globals.h"
class GameObject;
enum class ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH,
	TEXTURE
};


template <typename BufferType>
struct BuffersData
{
	~BuffersData()
	{
		RELEASE_ARRAY(buffer);
	}
	bool has_data = false;	// If exist the data type.
	uint num = 0u;		// Number of the data type.

	GLuint id_buffer = 0u;		// Id of the buffer.
	BufferType* buffer = nullptr;	// The buffer with data.
	uint buffer_size = 0u;		// The size of the buffer.

};


class Component
{
public:
	ComponentType type = ComponentType::NONE;
	GameObject* object = nullptr;

public:
	virtual void Update() {};
	virtual void CleanUp() {};
	virtual void InspectorDraw() {};
};


#endif //__COMPONENT_H__