#ifndef __COMPONENTTEXTURE_H__
#define __COMPONENTTEXTURE_H__

#include "Component.h"


class ComponentTexture : public Component
{
public:
	ComponentTexture() {};
	void ActivateThisTexture();
	bool IsTextureActive() const;
	void DesactivateTexture();
	void InspectorDraw();
	void SetCheckersToGOSelected();
	void Update() override;

public:

	//Textures
	bool has_texture = false;
	GLuint id_texture = 0;
	bool activate_checkers = false;

private:

	bool texture_active = false;

};


#endif // !__COMPONENTTEXTURE_H__

