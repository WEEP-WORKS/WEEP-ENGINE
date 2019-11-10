#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"

ComponentCamera::ComponentCamera()
{
	camera = new Camera3D();
}

void ComponentCamera::Update()
{

}

void ComponentCamera::CleanUp()
{
	delete camera;
}

void ComponentCamera::InspectorDraw()
{
}
