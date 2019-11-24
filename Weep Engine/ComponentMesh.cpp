#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include <vector>
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleImporter.h"
#include "ResourceManagment.h"
#include "ModuleFileSystem.h"

#include "ResourceTexture.h"
#include "ResourceMesh.h"
ComponentMesh::ComponentMesh()
{

	color.r = color.g = color.b = 1.f;
}


void ComponentMesh::PostUpdate()
{
//	if(object->isInsideFrustum)
		Render();
}

void ComponentMesh::CleanUp()
{
	//RELEASE(mesh_data);//no TODO with references 
}

void ComponentMesh::OnGetBoundingBox(AABB& box)
{
	box.Enclose(GetResource()->GetBbox());
}



void ComponentMesh::Render()
{
	// Push matrix
	glPushMatrix();
	glMultMatrixf(object->ConstGetTransform()->GetGlobalTransform().Transposed().ptr());

	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);

	RenderModel();
	ResourceMesh* resource = GetResource();
	if (resource->mesh_data->normal_vertexs.has_data && activate_vertex_normals)
	{
		RenderVertexNormals();
	}

	if (resource->mesh_data->normal_faces.has_data && activate_face_normals)
	{
		RenderFaceNormals();
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor3f(255, 255, 255);

	// Pop matrix
	glPopMatrix();

}

void ComponentMesh::RenderModel()
{

	ResourceMesh* resource = GetResource();
	GameObject* ob = object->parent;

	glBindBuffer(GL_ARRAY_BUFFER, resource->mesh_data->vertexs.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates.


	//bind normals direction for the ilumination
	if (resource->mesh_data->normals_direction.has_data)
	{
		glEnableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, resource->mesh_data->normals_direction.id_buffer);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	
	if (texture != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//bind UVs
		glBindBuffer(GL_ARRAY_BUFFER, resource->mesh_data->uvs.id_buffer);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL); //every texCoord have 2 coordinates.


		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture->GetResource(texture->GetResourceID())->id_texture);
	}

	//indexs final
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource->mesh_data->indexs.id_buffer);


	//Draw
	glDrawElements(GL_TRIANGLES, resource->mesh_data->indexs.buffer_size, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void ComponentMesh::RenderVertexNormals()
{
	ResourceMesh* resource = GetResource();

	glColor3f(0, 255.f, 255.f);
	glBindBuffer(GL_ARRAY_BUFFER, resource->mesh_data->normal_vertexs.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates

	glDrawArrays(GL_LINES, 0, resource-> mesh_data->normal_vertexs.num * 2/*number of vertexs. each normal have 2 points.*/);
	glColor3f(255.f, 255.f, 255.f);
}

void ComponentMesh::RenderFaceNormals()
{
	ResourceMesh* resource = GetResource();

	glColor3f(255.f, 255.f, 0.f);
	glBindBuffer(GL_ARRAY_BUFFER, resource->mesh_data->normal_faces.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates

	glDrawArrays(GL_LINES, 0, resource->mesh_data->normal_faces.num * 2/*number of vertexs. each normal have 2 points.*/);
	glColor3f(255.f, 255.f, 255.f);

}



void ComponentMesh::SetTexture(ComponentTexture* texture)
{
	this->texture = texture;	
}

ComponentTexture* ComponentMesh::GetTexture() const
{
	return texture;
}

void ComponentMesh::InspectorDraw() 
{
	bool to_active = IsActive();
	if (ImGui::Checkbox("", &to_active))
		SetActive(to_active);
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("FACE NORMALS", &activate_face_normals);
		ImGui::SameLine();
		ImGui::Checkbox("VERTEX NORMALS", &activate_vertex_normals);
	}

}

void ComponentMesh::Save(Json::Value& scene) const
{
	Json::Value comonent_mesh;

	comonent_mesh["type"] = (int)type;

	if (GetResource() != nullptr)
		App->importer->CreateOwnFile(GetResource(), object->GetName());
	
		

	comonent_mesh["Model name"] = string(object->GetName() + string(".mesh"));
	scene.append(comonent_mesh);
}

void ComponentMesh::Load(const Json::Value& component)
{
	App->importer->LoadOwnFile(component["Model name"].asString(), this);

	/*string s = App->file_system->GetWritePath();
	//App->file_system->SetWritePath("");
	App->file_system->Remove(std::string(LIBRARY_MESH_FOLDER + std::string(object->GetName()) + ".mesh").c_str());
	App->file_system->SetWritePath(s.c_str());
	*/
	AABB aabb;
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)GetResource()->mesh_data->vertexs.buffer, GetResource()->mesh_data->vertexs.num);
	GetResource()->mesh_data->aabb = aabb;
}

ResourceMesh* ComponentMesh::GetResource()
{
	return (ResourceMesh*)App->resource_managment->GetByID(resource_id);
}

const ResourceMesh* ComponentMesh::GetResource() const
{
	return (ResourceMesh*)App->resource_managment->GetByID(resource_id);
}

const UID ComponentMesh::GetResourceID() const
{
	return resource_id;
}
void ComponentMesh::SetResourceID(UID id)
{
	resource_id = id;
}