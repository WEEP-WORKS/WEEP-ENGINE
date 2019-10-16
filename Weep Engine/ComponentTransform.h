#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();

	void InspectorDraw();

public:

	float3 position;
	float3 rotation;
	float3 scale;

private: 
	
	float positionx;
};
#endif // !__COMPONENTTRANSFORM_H__

