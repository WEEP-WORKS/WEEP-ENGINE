#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "glew/glew.h"
#include "Globals.h"
#include "imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
enum class ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA
};


template <typename BufferType>
struct BuffersData
{
	~BuffersData()
	{
		RELEASE_ARRAY(buffer);
	}
	bool			has_data		= false;	// If exist the data type.
	uint			num				= 0u;		// Number of the data type.

	GLuint			id_buffer		= 0u;		// Id of the buffer.
	BufferType*		buffer			= nullptr;	// The buffer with data.
	uint			buffer_size		= 0u;		// The size of the buffer.

};

struct MeshData
{
	~MeshData()
	{
		RELEASE_ARRAY(vertexs.buffer);
		RELEASE_ARRAY(indexs.buffer);
		RELEASE_ARRAY(normals_direction.buffer);
		RELEASE_ARRAY(normal_vertexs.buffer);
		RELEASE_ARRAY(normal_faces.buffer);
		RELEASE_ARRAY(uvs.buffer)

		glDeleteBuffers(1, &vertexs.id_buffer);
		glDeleteBuffers(1, &indexs.id_buffer);
		glDeleteBuffers(1, &normals_direction.id_buffer);
		glDeleteBuffers(1, &normal_vertexs.id_buffer);
		glDeleteBuffers(1, &normal_faces.id_buffer);
		glDeleteBuffers(1, &uvs.id_buffer);
	}
	BuffersData<float> vertexs;

	BuffersData<uint> indexs;

	BuffersData<float> normals_direction;

	BuffersData<float> normal_vertexs;

	BuffersData<float> normal_faces;

	BuffersData<float> uvs;

	AABB aabb;

};

class Component
{
public:
	ComponentType	type = ComponentType::NONE;
	GameObject*		object = nullptr;

public:
	virtual void Update() {};
	virtual void CleanUp() {};
	virtual void InspectorDraw() {};

	const bool Component::IsActive() const { return is_active; };
	void Component::SetActive(bool to_active) { is_active = to_active; };

	virtual void OnGetBoundingBox(AABB &bbox) {};

private:
	bool			is_active = true;
};


#endif //__COMPONENT_H__