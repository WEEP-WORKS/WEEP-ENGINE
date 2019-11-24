#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Globals.h"
#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include\MathGeoLib.h"

class ResourceMesh;
class ComponentTexture;


class ComponentMesh : public Component
{
public:
	

	Color color;


private:

	ComponentTexture*	texture						= nullptr;

	bool				activate_face_normals		= false;
	bool				activate_vertex_normals		= false;

	UID resource_id = 0u;


public:
	ComponentMesh();

	void PostUpdate();

	void CleanUp() override;

	void OnGetBoundingBox(AABB & box);


	

	void SetTexture(ComponentTexture*);

	void InspectorDraw();

	void Save(Json::Value&) const override;
	void Load(const Json::Value&) override;

	const ResourceMesh* GetResource() const;
	ResourceMesh* GetResource();

	const UID GetResourceID() const;
	void SetResourceID(UID);


	ComponentTexture* GetTexture() const;

private:

	void Render();

	void RenderModel();

	

	void RenderVertexNormals();

	void RenderFaceNormals();
};

#endif //__COMPONENTMESH_H__