#include "ComponentRender2D.h"
#include <vector>
#include "ModuleWindow.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleImporter.h"
#include "ResourceManagment.h"
#include "ModuleFileSystem.h"

#include "ResourceTexture.h"
#include "ResourceMesh.h"

ComponentRender2D::ComponentRender2D()
{

}


void ComponentRender2D::PostUpdate()
{
	Render();
}

void ComponentRender2D::CleanUp()
{
	//RELEASE(mesh_data);//no TODO with references 
}



void ComponentRender2D::Render()
{
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLfloat last_projection[16]; glGetFloatv(GL_PROJECTION_MATRIX, &last_projection[0]);

	Rect rect = App->window->GetWindowRect();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, rect.right, rect.bottom, 0, -1000.0f, +1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//glMultMatrixf((object)->GetOrtoTransform().Transposed().ptr());

	//Draw((object)->GetVertices(), (object)->GetNumIndices(), (object)->GetIndices(), (object)->GetUvs(), (object)->GetTextureId());

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	/*if (UI_EDIT)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf((*it)->GetTransform().Transposed().ptr());

		Draw((*object)->GetVertices(), (*object)->GetNumIndices(), (*object)->GetIndices(), (*object)->GetUvs(), (*object)->GetTextureId());
	}
	

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();*/

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);


	//Restore modified state
	glPolygonMode(GL_FRONT, last_polygon_mode[0]); glPolygonMode(GL_BACK, last_polygon_mode[1]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void ComponentRender2D::Draw(float * vertices, uint num_indices, uint * indices, float * uvs, uint texture_id)
{
	glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, vertices);
	glTexCoordPointer(3, GL_FLOAT, sizeof(float) * 3, uvs);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ComponentRender2D::SetTexture(ComponentTexture* texture)
{
	this->texture = texture;
}

ComponentTexture* ComponentRender2D::GetTexture() const
{
	return texture;
}

void ComponentRender2D::InspectorDraw()
{
	bool to_active = IsActive();
	if (ImGui::Checkbox("", &to_active))
		SetActive(to_active);
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Render2D", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
	}

}