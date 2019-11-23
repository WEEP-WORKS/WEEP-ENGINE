#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__

#include "Resource.h"
#include "Globals.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "glew/glew.h"


template <typename BufferType>
struct BuffersData
{
	~BuffersData()
	{
		RELEASE_ARRAY(buffer);
	}
	bool			has_data = false;	// If exist the data type.
	uint			num = 0u;		// Number of the data type.

	uint			id_buffer = 0u;		// Id of the buffer.
	BufferType*		buffer = nullptr;	// The buffer with data.
	uint			buffer_size = 0u;		// The size of the buffer.

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
		RELEASE_ARRAY(uvs.buffer);

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

class ResourceMesh : public Resource
{
public:
	


	ResourceMesh(UID uid, Resource::Type type) : Resource(uid, type) 
	{
		mesh_data = new MeshData();
	};

	void SetBuffersWithData();

	void CalculateNormals();

	AABB GetBbox();

	//virtual ~ResourceTexture();
	//bool LoadInMemory() override;
	//void Save(Config& config) const override;
	//void Load(const Config& config) override;
private:
	void CalculateFacesNormals();

	void CalculateVertexsNormals();
public:
	//uint width = 0;
	//uint height = 0;
	//uint depth = 0;
	//uint mips = 0;
	//uint bytes = 0;
	//uint gpu_id = 0;
	//Format format = unknown;
	std::string name;
	std::string imported_file;

	uint				num_faces = 0u;		// Number of faces of the mesh.
	uint				num_uvs_channels = 0u;

	MeshData*			mesh_data;

private:
	float				normal_lenght = 0.5f;		// Lenght of all normals.

};
#endif // !__RESOURCEMESH_H__
