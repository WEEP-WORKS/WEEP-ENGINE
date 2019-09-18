#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;

class Module;
class vec3;
class Primitive;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	btRigidBody* GetBody();
	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	vec3 GetPos();
	void SetAsSensor(bool is_sensor);

private:
	btRigidBody* body = nullptr;
	bool is_sensor = false;
public:
	Primitive* primitive = nullptr;
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__