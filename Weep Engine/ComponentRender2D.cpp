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
#include "SceneManager.h"
#include "ResourceTexture.h"
#include "ModuleCamera3D.h"

ComponentRender2D::ComponentRender2D()
{
}


void ComponentRender2D::PostUpdate()
{
}

void ComponentRender2D::CleanUp()
{
	//RELEASE(mesh_data);//no TODO with references 
}



void ComponentRender2D::Render(BuffersData<float>& vertexs, BuffersData<float>& uvs, uint texture_id)
{
	Rect rect = App->window->GetWindowRect();
	if (App->scene_manager->GetState() == SCENE_STATE::PLAY)
	{
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, rect.right, rect.bottom, 0.0, 1.0, -1.0);

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else
	{
		//Reset Projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(App->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix().ptr());

		//Reset ModelView
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(App->camera->GetViewMatrix());

	}

	//Red quad
	/*glBegin(GL_QUADS);
	glColor3f(1.f, 0.f, 0.f);
	glVertex2f(vertexs.buffer[0], vertexs.buffer[1]);
	glVertex2f(vertexs.buffer[2], vertexs.buffer[3]);
	glVertex2f(vertexs.buffer[4], vertexs.buffer[5]);
	glVertex2f(vertexs.buffer[6], vertexs.buffer[7]);
	glEnd();*/
	//glColor3f(1.f, 0.f, 0.f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexs.id_buffer);
	glVertexPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, uvs.id_buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL); //every texCoord have 2 coordinates.

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//bind texture
	glBindTexture(GL_TEXTURE_2D, texture_id);


	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);



}

void ComponentRender2D::Draw(float * vertices, uint num_indices, uint * indices, float * uvs, uint texture_id)
{
	/*glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, vertices);
	glTexCoordPointer(3, GL_FLOAT, sizeof(float) * 3, uvs);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);


	glBindTexture(GL_TEXTURE_2D, 0);*/

	
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