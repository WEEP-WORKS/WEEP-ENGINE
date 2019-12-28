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

ComponentRender2D::ComponentRender2D()
{
	vertexs.buffer = new float[8];
	//vertice 1
	vertexs.buffer[0] = 0.0f;
	vertexs.buffer[1] = 0.0f;

	//vertice 2
	vertexs.buffer[2] = 0.0f;
	vertexs.buffer[3] = 100.0f;

	//vertice 3
	vertexs.buffer[4] = 100.0f;
	vertexs.buffer[5] = 100.0f;

	//vertice 4
	vertexs.buffer[6] = 100.0f;
	vertexs.buffer[7] = 0.0f;

	LOG("vertice 0: %f", vertexs.buffer[0]);
	LOG("vertice 1: %f", vertexs.buffer[1]);
	LOG("vertice 2: %f", vertexs.buffer[2]);
	LOG("vertice 3: %f", vertexs.buffer[3]);
	LOG("vertice 4: %f", vertexs.buffer[4]);
	LOG("vertice 5: %f", vertexs.buffer[5]);
	LOG("vertice 6: %f", vertexs.buffer[6]);
	LOG("vertice 7: %f", vertexs.buffer[7]);
	glGenBuffers(1, &vertexs.id_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexs.id_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * /*vertexs.buffer_size*/ 4 * 2, vertexs.buffer, GL_STATIC_DRAW);

	Rect rect = App->window->GetWindowRect();


	//Set the viewport
	//glViewport(0.f, 0.f, rect.right, rect.bottom);

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0.0, rect.right, rect.bottom, 0.0, 1.0, -1.0);

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//Initialize clear color
	//glClearColor(0.f, 0.f, 0.f, 1.f);

	////Check for error
	//GLenum error = glGetError();
	//if (error != GL_NO_ERROR)
	//{
	//	printf("Error initializing OpenGL! %s\n", gluErrorString(error));
	//	return;
	//}
	
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
	Rect rect = App->window->GetWindowRect();
	//Clear color buffer
	//glClear(GL_COLOR_BUFFER_BIT);

	//Reset modelview matrix
	//glLoadIdentity();

	//Move to center of the screen
	//glTranslatef(rect.right / 2.f, rect.bottom / 2.f, 0.f);
	//glViewport(0.f, 0.f, rect.right, rect.bottom);

	//Draw(nullptr, 0, nullptr, nullptr, 0);
	glBegin(GL_QUADS);
	glColor3f(1.f, 0.f, 0.f);
	glVertex2f(-rect.right / 2.f, -rect.bottom / 2.f);
	glVertex2f(rect.right / 2.f, -rect.bottom / 2.f);
	glVertex2f(rect.right / 2.f, rect.bottom / 2.f);
	glVertex2f(-rect.right / 2.f, rect.bottom / 2.f);
	glEnd();


	glDisableClientState(GL_VERTEX_ARRAY);

}

void ComponentRender2D::Draw(float * vertices, uint num_indices, uint * indices, float * uvs, uint texture_id)
{
	/*glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, vertices);
	glTexCoordPointer(3, GL_FLOAT, sizeof(float) * 3, uvs);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);


	glBindTexture(GL_TEXTURE_2D, 0);*/

	LOG("vertice 0: %f", vertexs.buffer[0]);
	LOG("vertice 1: %f", vertexs.buffer[1]);
	LOG("vertice 2: %f", vertexs.buffer[2]);
	LOG("vertice 3: %f", vertexs.buffer[3]);
	LOG("vertice 4: %f", vertexs.buffer[4]);
	LOG("vertice 5: %f", vertexs.buffer[5]);
	LOG("vertice 6: %f", vertexs.buffer[6]);
	LOG("vertice 7: %f", vertexs.buffer[7]);

	glBegin(GL_QUADS);
	glVertex2f(vertexs.buffer[0], vertexs.buffer[1]);
	glVertex2f(vertexs.buffer[2], vertexs.buffer[3]);
	glVertex2f(vertexs.buffer[4], vertexs.buffer[5]);
	glVertex2f(vertexs.buffer[6], vertexs.buffer[7]);
	glEnd();
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