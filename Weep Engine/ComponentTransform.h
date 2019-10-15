#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "Vector3.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();

public:

	Vector3<float> position;
	Vector3<float> rotation;
	Vector3<float> scale;
};
#endif // !__COMPONENTTRANSFORM_H__

