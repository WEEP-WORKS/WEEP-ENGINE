#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

enum Type
{
	pb_vehicle,
	pb_checkpoint,
	pb_scoredot,
	pb_hammer,
	pb_die_sensor,
	pb_undefined
};

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);

private:
	bool IsSensor;
public:
	btRigidBody* body = nullptr;
	p2List<Module*> collision_listeners;
	Type type = pb_undefined;
};

#endif // __PhysBody3D_H__