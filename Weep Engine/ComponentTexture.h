#ifndef __COMPONENTTEXTURE_H__
#define __COMPONENTTEXTURE_H__

#include "Component.h"
#include <string>

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

	bool has_texture = false;
	GLuint id_texture = 0;
	std::string texture_path;
	int texture_width;
	int texture_height;

private:

	bool texture_active = false;
	bool activate_checkers = false;

};


#endif // !__COMPONENTTEXTURE_H__

