#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "App.h"
#include "ModuleRenderer3D.h"

ComponentCamera::ComponentCamera()
{
	camera = App->camera->CreateCamera();
}

void ComponentCamera::Update()
{
	camera->SetPosition(object->transform->GetPosition());
	camera->SetZDir(object->transform->GetGlobalTransform().WorldZ());
	camera->SetYDir(object->transform->GetGlobalTransform().WorldY());

	//float3 corners[8];
	//camera->GetCorners(corners);

	float3 vertices[8];
	camera->GetFrustum().GetCornerPoints(vertices);

	GLint previous[2];

	glGetIntegerv(GL_POLYGON_MODE, previous);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(255, 255, 255);

	glLineWidth(5);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, previous[0]);

	glColor3f(255, 255, 255);

	glLineWidth(1.0f);
}

void ComponentCamera::CleanUp()
{
	App->camera->DestroyCamera(camera);
}

Camera3D * ComponentCamera::GetCamera() const
{
	return camera;
}

void ComponentCamera::InspectorDraw()
{
	bool to_active = IsActive();
	if (ImGui::Checkbox("", &to_active))
		SetActive(to_active);
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float near_plane = camera->GetNearPlaneDistance();
		if (ImGui::DragFloat("Near plane", &near_plane, 0.2f, 0.01f, 1000))
		{
			camera->SetNearPlaneDistance(near_plane);
		}

		float far_plane = camera->GetFarPlaneDistance();
		if (ImGui::DragFloat("Far plane", &far_plane, 10, near_plane, 10000))
		{
			camera->SetFarPlaneDistance(far_plane);
		}

		float fov = camera->GetVerticalFOV();
		if (ImGui::DragFloat("Field of view", &fov, 1, 1, 179.9f))
		{
			camera->SetFOV(fov);
		}
		bool frustum_culling = camera->GetFrustumCulling();
		if (ImGui::Checkbox("Frustum culling", &frustum_culling))
		{
			camera->SetFrustumCulling(frustum_culling);
		}
	}
}