#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
class Camera3D;

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	void Update();
	void CleanUp() override;

	Camera3D * GetCamera() const;

	void InspectorDraw();

	void Save(Json::Value&) const override;
	void Load(const Json::Value&) override;


private:
	Camera3D* camera = nullptr;
};

#endif
