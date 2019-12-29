#ifndef __COMPONENTRENDER2D_H__
#define __COMPONENTRENDER2D_H__

#include "Globals.h"
#include "Component.h"
#include "Color.h"
#include "ResourceMesh.h"

class ComponentTexture;
class ComponentRender2D : public Component
{

public:
	ComponentRender2D();

	void PostUpdate();

	void CleanUp() override;

	void InspectorDraw();

	void SetTexture(ComponentTexture* texture);
	ComponentTexture* GetTexture() const;
	void Render(BuffersData<float>& vertexs);

private:


	void Draw(float * vertices, uint num_indices, uint * indices, float * uvs, uint texture_id);

private:
	ComponentTexture*	texture = nullptr;
	
	//BuffersData<float>	uvs;

};

#endif //__COMPONENTRENDER_H__
