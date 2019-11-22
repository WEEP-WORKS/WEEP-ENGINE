#ifndef __COMPONENTTEXTURE_H__
#define __COMPONENTTEXTURE_H__

#include "Component.h"
#include <string>
class ResourceTexture;

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
	void Save(Json::Value&) const override;
	void Load(const Json::Value&) override;

	const ResourceTexture* GetResource(UID id) const;
	ResourceTexture* GetResource(UID id) ;

	const UID GetResourceID() const;
	void SetResourceID(UID);


public:

	bool			has_texture				= false; //erase

private:

	bool			texture_active			= false;
	bool			activate_checkers		= false;

	UID resource_id = 0u;

};


#endif // !__COMPONENTTEXTURE_H__

