#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include <vector>
#include "GameObject.h"
#include "imgui.h"

ComponentMesh::ComponentMesh()
{
	mesh_data = new MeshData();
	color.r = color.g = color.b = 1.f;
}

void ComponentMesh::SetBuffersWithData()
{
	if (mesh_data->vertexs.has_data)
	{
		glGenBuffers(1, &mesh_data->vertexs.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_data->vertexs.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_data->vertexs.buffer_size, mesh_data->vertexs.buffer, GL_STATIC_DRAW);
	}
	if (mesh_data->indexs.has_data)
	{
		glGenBuffers(1, &mesh_data->indexs.id_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data->indexs.id_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh_data->indexs.buffer_size, mesh_data->indexs.buffer, GL_STATIC_DRAW);
	}

	if (mesh_data->normals_direction.has_data)
	{
		glGenBuffers(1, &mesh_data->normals_direction.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_data->normals_direction.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh_data->normals_direction.buffer_size, mesh_data->normals_direction.buffer, GL_STATIC_DRAW);
	}

	if (mesh_data->normal_vertexs.has_data)
	{
		glGenBuffers(1, &mesh_data->normal_vertexs.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_data->normal_vertexs.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh_data->normal_vertexs.buffer_size, mesh_data->normal_vertexs.buffer, GL_STATIC_DRAW);
	}

	if (mesh_data->normal_faces.has_data)
	{
		glGenBuffers(1, &mesh_data->normal_faces.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_data->normal_faces.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh_data->normal_faces.buffer_size, mesh_data->normal_faces.buffer, GL_STATIC_DRAW);
	}

	if (mesh_data->uvs.has_data)
	{
		glGenBuffers(1, &mesh_data->uvs.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_data->uvs.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh_data->uvs.buffer_size, mesh_data->uvs.buffer, GL_STATIC_DRAW);
	}
}

void ComponentMesh::Update()
{
	Render();
}

void ComponentMesh::CleanUp()
{
	RELEASE(mesh_data);
}

void ComponentMesh::Render()
{
	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);


	RenderModel();

	if (mesh_data->normal_vertexs.has_data && activate_vertex_normals)
	{
		RenderVertexNormals();
	}

	if (mesh_data->normal_faces.has_data && activate_face_normals)
	{
		RenderFaceNormals();
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void ComponentMesh::RenderModel()
{
	// Push matrix
	//glPushMatrix();
	//glMultMatrixf(transform->GetGlobalTransform().Transposed().ptr());

	glBindBuffer(GL_ARRAY_BUFFER, mesh_data->vertexs.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates.


	//bind normals direction for the ilumination
	if (mesh_data->normals_direction.has_data)
	{
		glEnableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh_data->normals_direction.id_buffer);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	if (texture != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//bind UVs
		glBindBuffer(GL_ARRAY_BUFFER, mesh_data->uvs.id_buffer);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL); //every texCoord have 2 coordinates.


		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture->id_texture);
	}

	//indexs final
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data->indexs.id_buffer);


	//Draw
	glDrawElements(GL_TRIANGLES, mesh_data->indexs.buffer_size, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void ComponentMesh::RenderVertexNormals()
{
	glColor3f(0, 255.f, 255.f);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data->normal_vertexs.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates

	glDrawArrays(GL_LINES, 0, mesh_data->normal_vertexs.num * 2/*number of vertexs. each normal have 2 points.*/);

}

void ComponentMesh::RenderFaceNormals()
{
	glColor3f(255.f, 255.f, 0.f);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data->normal_faces.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates

	glDrawArrays(GL_LINES, 0, mesh_data->normal_faces.num * 2/*number of vertexs. each normal have 2 points.*/);
}


void ComponentMesh::CalculateNormals()
{
	CalculateVertexsNormals();

	CalculateFacesNormals();

}

void ComponentMesh::CalculateVertexsNormals()
{
	//vertex_normals
	mesh_data->normal_vertexs.buffer_size = mesh_data->normals_direction.num * 3/*x, y, z*/ * 2 /* start point, end point*/;
	mesh_data->normal_vertexs.buffer = new float[mesh_data->normal_vertexs.buffer_size];
	uint new_array_pos = 0u;

	//iterate vertexs array to get the normal of each vertex.
	for (uint i = 0; i < mesh_data->normals_direction.buffer_size; i += 3/*each vertex have 3 coords. Every 3 increment we hace another new vertex.x*/)
	{
		//---Start Points:---
		mesh_data->normal_vertexs.buffer[new_array_pos++] = mesh_data->vertexs.buffer[i];//current_vertex.x
		mesh_data->normal_vertexs.buffer[new_array_pos++] = mesh_data->vertexs.buffer[i + 1];//current_vertex.y
		mesh_data->normal_vertexs.buffer[new_array_pos++] =mesh_data->vertexs.buffer[i + 2];//current_vertex.z

		//---End points:---
		//the normal_length is to lengthen or shrink the normal.
		//the normals[i] is to get the coordinate of the normals direction of each corresponding vertex coordinate.
		mesh_data->normal_vertexs.buffer[new_array_pos++] = mesh_data->vertexs.buffer[i] + mesh_data->normals_direction.buffer[i] * normal_lenght;
		mesh_data->normal_vertexs.buffer[new_array_pos++] = mesh_data->vertexs.buffer[i + 1] + mesh_data->normals_direction.buffer[i + 1] * normal_lenght;
		mesh_data->normal_vertexs.buffer[new_array_pos++] = mesh_data->vertexs.buffer[i + 2] + mesh_data->normals_direction.buffer[i + 2] * normal_lenght;
	}
}

void ComponentMesh::CalculateFacesNormals()
{
	//face_normals--

	//start point of each face_normal. buffer and size
	uint start_vertexs_of_face_normals_size = num_faces * 3;/*1 start point by face. each start point have 3 coordinates (x, y, z).*/
	float * start_vertexs_of_face_normals = new float[start_vertexs_of_face_normals_size]; //aray with start points calculated of the normals. Could be use to render it.

	//face_normals_buffer and size. each normal have start and end point.
	mesh_data->normal_faces.buffer_size = start_vertexs_of_face_normals_size * 2 /*every start point will have their end point on the face_normals_buffer.*/;
	mesh_data->normal_faces.buffer = new float[mesh_data->normal_faces.buffer_size];

	uint new_array_pos = 0u;
	//iterate indexs of each face to get the face_normal.
	for (uint i = 0; i < mesh_data->indexs.buffer_size/*iterate indexs to find the normals*/; i += 3/*every 3 indexs we have another face*/) // num_indices -> cada cara guarda 3 valores. numero de caras * 3 = num_indices;
	{
		//---Start point:---
		// i = first index of the current face. They are ordered, so i+1 is the 2nd index of the current face, and i+3 the final index of the face
		float* v1 = &mesh_data->vertexs.buffer[mesh_data->indexs.buffer[i] * 3/*every index have 3 coords saved in vertexs*/];
		float* v2 = &mesh_data->vertexs.buffer[mesh_data->indexs.buffer[i+1] * 3/*every index have 3 coords saved in vertexs*/];
		float* v3 = &mesh_data->vertexs.buffer[mesh_data->indexs.buffer[i+2] * 3/*every index have 3 coords saved in vertexs*/];


		//average point of the plane and start point of the normal
		start_vertexs_of_face_normals[i]	 = (v1[0] + v2[0] + v3[0]) / 3.0f; // x coord
		start_vertexs_of_face_normals[i + 1] = (v1[1] + v2[1] + v3[1]) / 3.0f; // y coord
		start_vertexs_of_face_normals[i + 2] = (v1[2] + v2[2] + v3[2]) / 3.0f; // z coord

		//---End point:---
		 //same as the vertex but with the normals. the vertex_normal of each vertex of the face
		float* n1 = &mesh_data->normals_direction.buffer[mesh_data->indexs.buffer[i] * 3/*every index have 3 coords saved in vertexs*/]; 
		float* n2 = &mesh_data->normals_direction.buffer[mesh_data->indexs.buffer[i+1] * 3/*every index have 3 coords saved in vertexs*/];
		float* n3 = &mesh_data->normals_direction.buffer[mesh_data->indexs.buffer[i+2] * 3/*every index have 3 coords saved in vertexs*/];

		// average normal of the 3 vertex_normal of each vertex of the face.
		float3 final_vertex;
		final_vertex.x = (n1[0] + n2[0] + n3[0]) / 3.0f; // x coord
		final_vertex.y = (n1[1] + n2[1] + n3[1]) / 3.0f; // y coord
		final_vertex.z = (n1[2] + n2[2] + n3[2]) / 3.0f; // z coord

		mesh_data->normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i]; // start point coord x
		mesh_data->normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 1]; // start point coord y
		mesh_data->normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 2]; // start point coord z

		//the normal_length is to lengthen or shrink the normal.
		mesh_data->normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i] + final_vertex.x * normal_lenght; // end point coord x
		mesh_data->normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 1] + final_vertex.y * normal_lenght; // end point coord y
		mesh_data->normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 2] + final_vertex.z * normal_lenght; // end point coord z
	}

	RELEASE_ARRAY(start_vertexs_of_face_normals);
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