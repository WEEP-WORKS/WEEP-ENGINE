#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__


#include "Component.h"
#include "Globals.h"
#include "glew/glew.h"
#include "Color.h"
#include "Vector3.h"

class ComponentMesh : public Component
{
public:
	template <typename BufferType>
	struct BuffersData
	{
		bool has_data = false;	// If exist the data type.
		uint num = 0u;		// Number of the data type.

		GLuint id_buffer = 0u;		// Id of the buffer.
		BufferType* buffer = nullptr;	// The buffer with data.
		uint buffer_size = 0u;		// The size of the buffer.

	};



	//Vertexs:------
	BuffersData<float> vertexs;


	//Indexs:------
	BuffersData<uint> indexs;


	//Normals:------
	uint num_faces = 0u;		// Number of faces of the mesh.
	float normal_lenght = 0.5f;		// Lenght of all normals.

	BuffersData<float> normals_direction;

	BuffersData<float> normal_vertexs;

	BuffersData<float> normal_faces;

	Color color;

public:
	ComponentMesh();
	~ComponentMesh() {};

	void Update() override;

	void SetBuffersWithData();

	void CalculateNormals();

private:

	void Render();

	void RenderModel();

	void CalculateFacesNormals();

	void CalculateVertexsNormals();

	void RenderVertexNormals();

	void RenderFaceNormals();

	inline Vector3<float>* ReturnVertexByIndex(const uint&) const;

	inline Vector3<float>* ReturnNormalDirectionByIndex(const uint&) const;
};

#endif //__COMPONENTMESH_H__