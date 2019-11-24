#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"
#include "ModuleGameObjectManager.h"
#include "ModuleQuadtree.h"
#include "glew/glew.h"

ComponentCamera::ComponentCamera()
{
	camera = App->camera->CreateCamera();
}

void ComponentCamera::Update()
{
	camera->SetPosition(object->ConstGetTransform()->GetPosition());
	camera->SetZDir(object->ConstGetTransform()->GetGlobalTransform().WorldZ());
	camera->SetYDir(object->ConstGetTransform()->GetGlobalTransform().WorldY());

	if (camera->GetFrustumCulling())
	{
		int all_gameObjects = App->game_object_manager->GetAllGameObjectNumber();
		std::vector<GameObject*> game_objects_inside_frustrum = App->quadtree->GetAllGameObjectsinsideFrustrum(camera->GetFrustum());

			for(std::vector<GameObject*>::iterator iter = game_objects_inside_frustrum.begin(); iter != game_objects_inside_frustrum.end(); ++iter)
			{
				if ((*iter)->GetMesh())
				{
					if (camera->CheckInsideFrustum((*iter)->local_bbox))
					{
						if((*iter) != nullptr)
							(*iter)->isInsideFrustum = true;
					}
				}
			}
	}

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

void ComponentCamera::Save(Json::Value& scene) const
{
	Json::Value component_camera;

	component_camera["type"] = (int)type;

	component_camera["FOV"] = camera->GetVerticalFOV();
	component_camera["Near Plane"] = camera->GetNearPlaneDistance();
	component_camera["Far Plane"] = camera->GetFarPlaneDistance();
	component_camera["Frustum Culling"] = camera->GetFrustumCulling();

	scene.append(component_camera);

}

void ComponentCamera::Load(const Json::Value& component)
{
	camera->SetFOV(component["FOV"].asFloat());
	camera->SetFrustumCulling(component["Frustum Culling"].asBool());
	camera->SetNearPlaneDistance(component["Near Plane"].asFloat());
	camera->SetFarPlaneDistance(component["Far Plane"].asFloat());

}