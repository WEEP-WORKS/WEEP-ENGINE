#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include <vector>
#include "GameObject.h"
#include "imgui.h"

ComponentMesh::ComponentMesh()
{
	color.r = color.g = color.b = 1.f;
}

void ComponentMesh::SetBuffersWithData()
{
	if (vertexs.has_data)
	{
		glGenBuffers(1, &vertexs.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexs.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexs.buffer_size, vertexs.buffer, GL_STATIC_DRAW);
	}
	if (indexs.has_data)
	{
		glGenBuffers(1, &indexs.id_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexs.id_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indexs.buffer_size, indexs.buffer, GL_STATIC_DRAW);
	}

	if (normals_direction.has_data)
	{
		glGenBuffers(1, &normals_direction.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, normals_direction.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals_direction.buffer_size, normals_direction.buffer, GL_STATIC_DRAW);
	}

	if (normal_vertexs.has_data)
	{
		glGenBuffers(1, &normal_vertexs.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, normal_vertexs.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normal_vertexs.buffer_size, normal_vertexs.buffer, GL_STATIC_DRAW);
	}

	if (normal_faces.has_data)
	{
		glGenBuffers(1, &normal_faces.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, normal_faces.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normal_faces.buffer_size, normal_faces.buffer, GL_STATIC_DRAW);
	}

	if (uvs.has_data)
	{
		glGenBuffers(1, &uvs.id_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvs.id_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*uvs.buffer_size, uvs.buffer, GL_STATIC_DRAW);
	}
}

void ComponentMesh::Update()
{
	Render();
}

void ComponentMesh::CleanUp()
{
	RELEASE_ARRAY(vertexs.buffer);
	RELEASE_ARRAY(indexs.buffer);
	RELEASE_ARRAY(normals_direction.buffer);
	RELEASE_ARRAY(normal_vertexs.buffer);
	RELEASE_ARRAY(normal_faces.buffer);
	RELEASE_ARRAY(uvs.buffer);

	glDeleteBuffers(1, &vertexs.id_buffer);
	glDeleteBuffers(1, &indexs.id_buffer);
	glDeleteBuffers(1, &normals_direction.id_buffer);
	glDeleteBuffers(1, &normal_vertexs.id_buffer);
	glDeleteBuffers(1, &normal_faces.id_buffer);
	glDeleteBuffers(1, &uvs.id_buffer);
}

void ComponentMesh::Render()
{
	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);


	RenderModel();

	if (normal_vertexs.has_data && activate_vertex_normals)
	{
		RenderVertexNormals();
	}

	if (normal_faces.has_data && activate_face_normals)
	{
		RenderFaceNormals();
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentMesh::RenderModel()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexs.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates.


	//bind normals direction for the ilumination
	if (normals_direction.has_data)
	{
		glEnableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, normals_direction.id_buffer);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	if (texture != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//bind UVs
		glBindBuffer(GL_ARRAY_BUFFER, uvs.id_buffer);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL); //every texCoord have 2 coordinates.


		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture->id_texture);
	}

	//indexs final
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexs.id_buffer);


	//Draw
	glDrawElements(GL_TRIANGLES, indexs.buffer_size, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void ComponentMesh::RenderVertexNormals()
{
	glColor3f(0, 255.f, 255.f);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vertexs.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates

	glDrawArrays(GL_LINES, 0, normal_vertexs.num * 2/*number of vertexs. each normal have 2 points.*/);

}

void ComponentMesh::RenderFaceNormals()
{
	glColor3f(255.f, 255.f, 0.f);
	glBindBuffer(GL_ARRAY_BUFFER, normal_faces.id_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL); //every vertex have 3 coordinates

	glDrawArrays(GL_LINES, 0, normal_faces.num * 2/*number of vertexs. each normal have 2 points.*/);
}


void ComponentMesh::CalculateNormals()
{
	CalculateVertexsNormals();

	CalculateFacesNormals();

}

void ComponentMesh::CalculateVertexsNormals()
{
	//vertex_normals
	normal_vertexs.buffer_size = vertexs.num * 3/*x, y, z*/ * 2 /* start point, end point*/;
	normal_vertexs.buffer = new float[normal_vertexs.buffer_size];
	uint new_array_pos = 0u;

	//iterate vertexs array to get the normal of each vertex.
	for (uint i = 0; i < vertexs.buffer_size; i += 3/*each vertex have 3 coords. Every 3 increment we hace another new vertex.x*/)
	{
		//---Start Points:---
		normal_vertexs.buffer[new_array_pos++] = vertexs.buffer[i];//current_vertex.x
		normal_vertexs.buffer[new_array_pos++] = vertexs.buffer[i + 1];//current_vertex.y
		normal_vertexs.buffer[new_array_pos++] = vertexs.buffer[i + 2];//current_vertex.z

		//---End points:---
		//the normal_length is to lengthen or shrink the normal.
		//the normals[i] is to get the coordinate of the normals direction of each corresponding vertex coordinate.
		normal_vertexs.buffer[new_array_pos++] = vertexs.buffer[i] + normals_direction.buffer[i] * normal_lenght;
		normal_vertexs.buffer[new_array_pos++] = vertexs.buffer[i + 1] + normals_direction.buffer[i + 1] * normal_lenght;
		normal_vertexs.buffer[new_array_pos++] = vertexs.buffer[i + 2] + normals_direction.buffer[i + 2] * normal_lenght;
	}
}

void ComponentMesh::CalculateFacesNormals()
{
	//face_normals--

	//start point of each face_normal. buffer and size
	uint start_vertexs_of_face_normals_size = num_faces * 3;/*1 start point by face. each start point have 3 coordinates (x, y, z).*/
	float * start_vertexs_of_face_normals = new float[start_vertexs_of_face_normals_size]; //aray with start points calculated of the normals. Could be use to render it.

	//face_normals_buffer and size. each normal have start and end point.
	normal_faces.buffer_size = start_vertexs_of_face_normals_size * 2 /*every start point will have their end point on the face_normals_buffer.*/;
	normal_faces.buffer = new float[normal_faces.buffer_size];

	uint new_array_pos = 0u;

	//iterate indexs of each face to get the face_normal.
	for (uint i = 0; i < indexs.buffer_size/*iterate indexs to find the normals*/; i += 3/*every 3 indexs we have another face*/) // num_indices -> cada cara guarda 3 valores. numero de caras * 3 = num_indices;
	{
		//---Start point:---
		// i = first index of the current face. They are ordered, so i+1 is the 2nd index of the current face, and i+3 the final index of the face
		float3* v1 = ReturnVertexByIndex(i);
		float3* v2 = ReturnVertexByIndex(i + 1);
		float3* v3 = ReturnVertexByIndex(i + 2);


		//average point of the plane and start point of the normal
		start_vertexs_of_face_normals[i] = (v1->x + v2->x + v3->x) / 3.0f; // x coord
		start_vertexs_of_face_normals[i + 1] = (v1->y + v2->y + v3->y) / 3.0f; // y coord
		start_vertexs_of_face_normals[i + 2] = (v1->z + v2->z + v3->z) / 3.0f; // z coord

		//---End point:---
		 //same as the vertex but with the normals. the vertex_normal of each vertex of the face
		float3* n1 = ReturnNormalDirectionByIndex(i);
		float3* n2 = ReturnNormalDirectionByIndex(i + 1);
		float3* n3 = ReturnNormalDirectionByIndex(i + 2);

		// average normal of the 3 vertex_normal of each vertex of the face.
		float3 final_vertex;
		final_vertex.x = (n1->x + n2->x + n3->x) / 3.0f; // x coord
		final_vertex.y = (n1->y + n2->y + n3->y) / 3.0f; // y coord
		final_vertex.z = (n1->z + n2->z + n3->z) / 3.0f; // z coord

		normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i]; // start point coord x
		normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 1]; // start point coord y
		normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 2]; // start point coord z

		//the normal_length is to lengthen or shrink the normal.
		normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i] + final_vertex.x * normal_lenght; // end point coord x
		normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 1] + final_vertex.y * normal_lenght; // end point coord y
		normal_faces.buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 2] + final_vertex.z * normal_lenght; // end point coord z

	}
}

float3* ComponentMesh::ReturnVertexByIndex(const uint &index) const
{
	float3* ret = new float3;
	float* first_vertex = &vertexs.buffer[indexs.buffer[index] * 3/*every index have 3 coords saved in vertexs*/];

	ret->x = first_vertex[0];
	ret->y = first_vertex[1];
	ret->z = first_vertex[2];

	return ret;
}

float3* ComponentMesh::ReturnNormalDirectionByIndex(const uint &index) const
{
	float3* ret = new float3;
	float* first_vertex = &normals_direction.buffer[indexs.buffer[index] * 3/*every index have 3 coords saved in vertexs*/];

	ret->x = first_vertex[0];
	ret->y = first_vertex[1];
	ret->z = first_vertex[2];

	return ret;
}


void ComponentMesh::SetTexture(ComponentTexture* texture)
{
	this->texture = texture;	
}

ComponentTexture* ComponentMesh::GetTexture() const
{
	return texture;
}

void ComponentMesh::InspectorDraw() {

	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("FACE NORMALS", &activate_face_normals);
		ImGui::SameLine();
		ImGui::Checkbox("VERTEX NORMALS", &activate_vertex_normals);
	}

}