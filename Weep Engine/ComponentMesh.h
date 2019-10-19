#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__


#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/Math/float3.h"

class ComponentTexture;

class ComponentMesh : public Component
{
public:
	
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

	//UVs
	uint num_uvs_channels = 0u; //local var?
	uint channel_buffer_size = 0u; //local var?

	BuffersData<float> uvs;

	Color color;

private:

	ComponentTexture* texture = nullptr;

	bool activate_face_normals = false;
	bool activate_vertex_normals = false;

public:
	ComponentMesh();
	~ComponentMesh() {};

	void Update() override;

	void SetBuffersWithData();

	void CalculateNormals();

	void SetTexture(ComponentTexture*);

	void InspectorDraw();

private:

	void Render();

	void RenderModel();

	void CalculateFacesNormals();

	void CalculateVertexsNormals();

	void RenderVertexNormals();

	void RenderFaceNormals();

	inline float3 *ReturnVertexByIndex(const uint&) const;

	inline float3 *ReturnNormalDirectionByIndex(const uint&) const;
};

#endif //__COMPONENTMESH_H__