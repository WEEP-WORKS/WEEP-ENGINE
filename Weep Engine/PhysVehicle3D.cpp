#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	body->setUserPointer(this);
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = DarkGrey;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	mat4x4 transform;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&transform);
	//--------
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	//--------
	Cube front_forkTube(info.front_forkTube_size.x, info.front_forkTube_size.y, info.front_forkTube_size.z);
	front_forkTube.color = Grey;
	front_forkTube.SetRotation(45, vec3(1, 0, 0));
	front_forkTube.transform = transform * front_forkTube.transform;
	btVector3 front_FT_offset(info.front_forkTube_offset.x, info.front_forkTube_offset.y, info.front_forkTube_offset.z);
	front_FT_offset = front_FT_offset.rotate(q.getAxis(), q.getAngle());

	front_forkTube.transform.M[12] += front_FT_offset.getX();
	front_forkTube.transform.M[13] += front_FT_offset.getY();
	front_forkTube.transform.M[14] += front_FT_offset.getZ();

	//--------
	Cube back_forkTube(info.back_forkTube_size.x, info.back_forkTube_size.y, info.back_forkTube_size.z);
	back_forkTube.color = Grey;
	back_forkTube.SetRotation(-45, vec3(1, 0, 0));
	back_forkTube.transform = transform * back_forkTube.transform;
	btVector3 back_FT_offset(info.back_forkTube_offset.x, info.back_forkTube_offset.y, info.back_forkTube_offset.z);
	back_FT_offset = back_FT_offset.rotate(q.getAxis(), q.getAngle());

	back_forkTube.transform.M[12] += back_FT_offset.getX();
	back_forkTube.transform.M[13] += back_FT_offset.getY();
	back_forkTube.transform.M[14] += back_FT_offset.getZ();

	//--------
	Cube handleBar(info.handleBar_size.x, info.handleBar_size.y, info.handleBar_size.z);
	handleBar.color = Black;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&handleBar.transform);
	btVector3 HB_offset(info.handleBar_offset.x, info.handleBar_offset.y, info.handleBar_offset.z);
	HB_offset = HB_offset.rotate(q.getAxis(), q.getAngle());

	handleBar.transform.M[12] += HB_offset.getX();
	handleBar.transform.M[13] += HB_offset.getY();
	handleBar.transform.M[14] += HB_offset.getZ();
	//--------
	Cube front_seat(info.front_seat_size.x, info.front_seat_size.y, info.front_seat_size.z);
	front_seat.color = Blue1;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front_seat.transform);
	btVector3 FS_offset(info.front_seat_offset.x, info.front_seat_offset.y, info.front_seat_offset.z);
	FS_offset = FS_offset.rotate(q.getAxis(), q.getAngle());

	front_seat.transform.M[12] += FS_offset.getX();
	front_seat.transform.M[13] += FS_offset.getY();
	front_seat.transform.M[14] += FS_offset.getZ();
	//--------
	Cube back_seat(info.back_seat_size.x, info.back_seat_size.y, info.back_seat_size.z);
	back_seat.color = Blue1;
	back_seat.SetRotation(25, vec3(1, 0, 0));
	back_seat.transform = transform * back_seat.transform;
	btVector3 BS_offset(info.back_seat_offset.x, info.back_seat_offset.y, info.back_seat_offset.z);
	BS_offset = BS_offset.rotate(q.getAxis(), q.getAngle());

	back_seat.transform.M[12] += BS_offset.getX();
	back_seat.transform.M[13] += BS_offset.getY();
	back_seat.transform.M[14] += BS_offset.getZ();
	//--------
	Cube back_seat_colored(info.back_seat_colored_size.x, info.back_seat_colored_size.y, info.back_seat_colored_size.z);
	back_seat_colored.color = Orange;
	back_seat_colored.SetRotation(25, vec3(1, 0, 0));
	back_seat_colored.transform = transform * back_seat_colored.transform;
	btVector3 BSC_offset(info.back_seat_colored_offset.x, info.back_seat_colored_offset.y, info.back_seat_colored_offset.z);
	BSC_offset = BSC_offset.rotate(q.getAxis(), q.getAngle());

	back_seat_colored.transform.M[12] += BSC_offset.getX();
	back_seat_colored.transform.M[13] += BSC_offset.getY();
	back_seat_colored.transform.M[14] += BSC_offset.getZ();

	front_forkTube.Render();
	back_forkTube.Render();
	handleBar.Render();
	front_seat.Render();
	back_seat.Render();
	back_seat_colored.Render();

	//Man----------------------------
	Sphere head(info.head_size);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&head.transform);
	btVector3 HEAD_offset(info.head_offset.x, info.head_offset.y, info.head_offset.z);
	HEAD_offset = HEAD_offset.rotate(q.getAxis(), q.getAngle());
	head.color = Pink;

	head.transform.M[12] += HEAD_offset.getX();
	head.transform.M[13] += HEAD_offset.getY();
	head.transform.M[14] += HEAD_offset.getZ();
	//-----------
	Cube torso(info.torso_size.x, info.torso_size.y, info.torso_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&torso.transform);
	btVector3 TORSO_offset(info.torso_offset.x, info.torso_offset.y, info.torso_offset.z);
	TORSO_offset = TORSO_offset.rotate(q.getAxis(), q.getAngle());
	torso.color = Blue2;

	torso.transform.M[12] += TORSO_offset.getX();
	torso.transform.M[13] += TORSO_offset.getY();
	torso.transform.M[14] += TORSO_offset.getZ();
	//-----------
	Cube biceps1(info.biceps1_size.x, info.biceps1_size.y, info.biceps1_size.z);
	biceps1.SetRotation(-50, vec3(1, 0, 0));
	biceps1.transform = transform * biceps1.transform;
	btVector3 BICEPS1_offset(info.biceps1_offset.x, info.biceps1_offset.y, info.biceps1_offset.z);
	BICEPS1_offset = BICEPS1_offset.rotate(q.getAxis(), q.getAngle());
	biceps1.color = Pink;

	biceps1.transform.M[12] += BICEPS1_offset.getX();
	biceps1.transform.M[13] += BICEPS1_offset.getY();
	biceps1.transform.M[14] += BICEPS1_offset.getZ();
	//-----------
	Cube biceps2(info.biceps2_size.x, info.biceps2_size.y, info.biceps2_size.z);
	biceps2.SetRotation(-50, vec3(1, 0, 0));
	biceps2.transform = transform * biceps2.transform;
	btVector3 BICEPS2_offset(info.biceps2_offset.x, info.biceps2_offset.y, info.biceps2_offset.z);
	BICEPS2_offset = BICEPS2_offset.rotate(q.getAxis(), q.getAngle());
	biceps2.color = Pink;

	biceps2.transform.M[12] += BICEPS2_offset.getX();
	biceps2.transform.M[13] += BICEPS2_offset.getY();
	biceps2.transform.M[14] += BICEPS2_offset.getZ();
	//-----------
	
	head.Render();
	torso.Render();
	biceps1.Render();
	biceps2.Render();

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}