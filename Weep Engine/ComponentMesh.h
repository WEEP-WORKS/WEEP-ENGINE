#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Globals.h"
#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/Math/float3.h"

class ComponentTexture;



class ComponentMesh : public Component
{
public:
	

	//Normals:------
	uint num_faces = 0u;		// Number of faces of the mesh.
	float normal_lenght = 0.5f;		// Lenght of all normals.


	//UVs
	uint num_uvs_channels = 0u; //local var?
	uint channel_buffer_size = 0u; //local var?

	MeshData* mesh_data;

	Color color;

private:

	ComponentTexture* texture = nullptr;

	bool activate_face_normals = false;
	bool activate_vertex_normals = false;

public:
	ComponentMesh();


	void Update() override;

	void CleanUp() override;

	void SetBuffersWithData();

	void CalculateNormals();

	void SetTexture(ComponentTexture*);

	void InspectorDraw();

	ComponentTexture* GetTexture() const;

private:

	void Render();

	void RenderModel();

	void CalculateFacesNormals();

	void CalculateVertexsNormals();

	void RenderVertexNormals();

	void RenderFaceNormals();
};

#endif //__COMPONENTMESH_H__