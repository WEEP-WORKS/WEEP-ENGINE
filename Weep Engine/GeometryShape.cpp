#include "GeometryShape.h"
#include "Globals.h"

//----------------------------------------------------------------------------------
//---------------------------------GEOMETRY MANAGER---------------------------------
//----------------------------------------------------------------------------------

ShapeManager::ShapeManager(bool start_enabled) : Module(start_enabled)
{
	SetName("ShapeManager");
}

GeometrySphere* ShapeManager::CreateSphere(const int& sphere_subdivisions)
{
	GeometryShape* s = new GeometrySphere();
	s->mesh = par_shapes_create_subdivided_sphere(sphere_subdivisions);
	s->vertexs = s->mesh->points;
	s->indices = (uint*)s->mesh->triangles;

	s->num_vertex = s->mesh->npoints;
	s->num_indices = s->mesh->ntriangles;

	s->Start();

	shapes.push_back(s);

	return (GeometrySphere*)s;
}

bool ShapeManager::Update()
{
	DrawAll();

	return true;
}

void ShapeManager::DrawAll() 
{
	for (list<GeometryShape*>::iterator item = shapes.begin(); item != shapes.end(); ++item)
	{
		(*item)->Render();
	}
}

bool ShapeManager::CleanUp()
{
	for (list<GeometryShape*>::iterator item = shapes.begin(); item != shapes.end(); ++item)
	{
		RELEASE(*item);
	}
	return true;
}

void ShapeManager::AddShape(GeometryShape* shape)
{
	shape->Start();
	shapes.push_back(shape);
}



//----------------------------------------------------------------------------------
//-------------------------------GEOMETRY SHAPE BASIC-------------------------------
//----------------------------------------------------------------------------------




void GeometryShape::Render()
{
	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	

	glDrawElements(GL_TRIANGLES, num_indices * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void GeometryShape::SetBuffersWithData()
{
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertex * 3, vertexs, GL_STATIC_DRAW);

	glGenBuffers(1, &id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices*3, indices, GL_STATIC_DRAW);

}


void GeometryShape::MoveShape(float x, float y, float z)
{
	par_shapes_translate(mesh, x, y, z);
	SetBuffersWithData(); // Load the changes in the buffer. All functions with changes the mesh hay have this function at the end
}

void GeometryShape::Start()
{
	SetBuffersWithData();
}

void GeometryShape::SetColor(float red, float green, float blue)
{
	color.r = red;
	color.g = green;
	color.b = blue;
}



//----------------------------------------------------------------------------------
//-------------------------------------FBX SHAPE------------------------------------
//----------------------------------------------------------------------------------


void FBXShape::SetBuffersWithData()
{
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertex * 3, vertexs, GL_STATIC_DRAW);

	glGenBuffers(1, &id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);

	if (has_normals)
	{
		glGenBuffers(1, &id_vertex_normals);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_normals * 3, vertexs_normals, GL_STATIC_DRAW);

		glGenBuffers(1, &id_face_normals);
		glBindBuffer(GL_ARRAY_BUFFER, id_face_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_indices * 2, face_normals, GL_STATIC_DRAW);
	}

}

void FBXShape::CalculateNormals()
{
	//vertex_normals
	vertexs_normals = new float[num_normals * 3/*x, y, z*/ * 2 /* start point, end point*/];
	uint new_array_pos = 0u;
	
	for (uint i = 0; i < num_normals * 3; i += 3)
	{
		//Start Points
		vertexs_normals[new_array_pos++] = vertexs[i];
		vertexs_normals[new_array_pos++] = vertexs[i + 1];
		vertexs_normals[new_array_pos++] = vertexs[i + 2];

		//end points
		vertexs_normals[new_array_pos++] = vertexs[i]     + normals[i]     * normal_lenght;
		vertexs_normals[new_array_pos++] = vertexs[i + 1] + normals[i + 1] * normal_lenght;
		vertexs_normals[new_array_pos++] = vertexs[i + 2] + normals[i + 2] * normal_lenght;
	}

	//face_normals
	face_normals = new float[num_indices/*x, y, z*/ *2 /* start point, end point*/];
	new_array_pos = 0u;
	float * debug_f_vertices = new float[num_indices];
	for (uint i = 0; i < num_indices; i += 3) // num_indices -> cada cara guarda 3 valores. numero de caras * 3 = num_indices;
	{
		/*face_normals[new_array_pos++] = vertexs[i] + vertexs[i + 3] + vertexs[i + 6] / 3.f;
		face_normals[new_array_pos++] = vertexs[i+1] + vertexs[i + 4] + vertexs[i + 7] / 3.f;
		face_normals[new_array_pos++] = vertexs[i+2] + vertexs[i + 5] + vertexs[i + 8] / 3.f;

		float final_normal_point[3];
		final_normal_point[0] = normals[i] + normals[i + 3] + normals[i + 6] / 3.f;
		final_normal_point[1] = normals[i+1] + normals[i + 4] + normals[i + 7] / 3.f;
		final_normal_point[2] = normals[i+2] + normals[i + 5] + normals[i + 8] / 3.f;

		face_normals[new_array_pos++] = face_normals[i] + final_normal_point[0] * normal_lenght;
		face_normals[new_array_pos++] = face_normals[i+1] + final_normal_point[1] * normal_lenght;
		face_normals[new_array_pos++] = face_normals[i+2] + final_normal_point[2] * normal_lenght;
		*/
		

		Vector3<float>* v1 = ReturnVertexByIndex(i);
		Vector3<float>* v2 = ReturnVertexByIndex(i + 1);
		Vector3<float>* v3 = ReturnVertexByIndex(i + 2);
		//float* v1 = &vertexs[indices[i] * 3];     // get first face vertex
		//float* v2 = &vertexs[indices[i + 1] * 3]; // get second face vertex
		//float* v3 = &vertexs[indices[i + 2] * 3]; // get third face vertex

		//average point of the plane
		debug_f_vertices[i] = (v1->x + v2->x + v3->x) / 3.0f; // x coord
		debug_f_vertices[i + 1] = (v1->y + v2->y + v3->y) / 3.0f; // y coord
		debug_f_vertices[i + 2] = (v1->z + v2->z + v3->z) / 3.0f; // z coord

		// compute the averaged normal of the 3 vertex of each face

		Vector3<float>* n1 = ReturnNormalByIndex(i);
		Vector3<float>* n2 = ReturnNormalByIndex(i + 1);
		Vector3<float>* n3 = ReturnNormalByIndex(i + 2);

		Vector3<float> final_vertex;
		final_vertex.x = (n1->x + n2->x + n3->x) / 3.0f; // x coord
		final_vertex.y = (n1->y + n2->y + n3->y) / 3.0f; // y coord
		final_vertex.z = (n1->z + n2->z + n3->z) / 3.0f; // z coord

		face_normals[new_array_pos++] = debug_f_vertices[i]; // x
		face_normals[new_array_pos++] = debug_f_vertices[i + 1]; // y
		face_normals[new_array_pos++] = debug_f_vertices[i + 2]; //z

		face_normals[new_array_pos++] = debug_f_vertices[i] + final_vertex.x * normal_lenght; // x
		face_normals[new_array_pos++] = debug_f_vertices[i + 1] + final_vertex.y * normal_lenght; // y
		face_normals[new_array_pos++] = debug_f_vertices[i + 2] + final_vertex.z * normal_lenght; // z*/


	}

}

Vector3<float>* FBXShape::ReturnVertexByIndex(const uint &index) const
{
	Vector3<float>* ret = new Vector3<float>();
	float* first_vertex = &vertexs[indices[index] * 3/*every index have 3 coords saved in vertexs*/];

	ret->x = first_vertex[0];
	ret->y = first_vertex[1];
	ret->z = first_vertex[2];

	return ret;
}

Vector3<float>* FBXShape::ReturnNormalByIndex(const uint &index) const
{
	Vector3<float>* ret = new Vector3<float>();
	float* first_vertex = &normals[indices[index] * 3/*every index have 3 coords saved in vertexs*/];

	ret->x = first_vertex[0];
	ret->y = first_vertex[1];
	ret->z = first_vertex[2];

	return ret;
}



void FBXShape::Render()
{
	glColor3f(color.r, color.g, color.b);

	RenderVertexsWithIndices();

	RenderVertexNormals();
	
}

void FBXShape::RenderVertexsWithIndices()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
}

void FBXShape::RenderVertexNormals()
{
	/*glBindBuffer(GL_ARRAY_BUFFER, id_vertex_normals);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, num_normals * 3);*/

	glBindBuffer(GL_ARRAY_BUFFER, id_face_normals);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, num_indices);



	glDisableClientState(GL_VERTEX_ARRAY);
}

/*
Vector3<float>* FBXShape::ReturnVertexByIndice(const uint &index) const
{
	Vector3<float>* ret = new Vector3<float>();
	float* first_vertex = &vertexs[indices[index] * 3/*every index have 3 coords saved in vertexs*///];

	/*ret->x = first_vertex[0];
	ret->y = first_vertex[1];
	ret->z = first_vertex[2];*/


	/*	Vector3<float>* v1 = ReturnVertexByIndice(i);
		Vector3<float>* v2 = ReturnVertexByIndice(i + 1);
		Vector3<float>* v3 = ReturnVertexByIndice(i + 2);
		//float* v1 = &vertexs[indices[i] * 3];     // get first face vertex
		//float* v2 = &vertexs[indices[i + 1] * 3]; // get second face vertex
		//float* v3 = &vertexs[indices[i + 2] * 3]; // get third face vertex

		//average point of the plane
		debug_f_vertices[i] = (v1->x + v2->x + v3->x) / 3.0f; // x coord
		debug_f_vertices[i + 1] = (v1->y + v2->y + v3->y) / 3.0f; // y coord
		debug_f_vertices[i + 2] = (v1->z + v2->z + v3->z) / 3.0f; // z coord

		// compute the averaged normal of the 3 vertex of each face

		Vector3<float>* n1 = ReturnVertexByIndice(i);
		Vector3<float>* n2 = ReturnVertexByIndice(i + 1);
		Vector3<float>* n3 = ReturnVertexByIndice(i + 2);

		Vector3<float> final_vertex;
		final_vertex.x = (n1->x + n2->x + n3->x) / 3.0f; // x coord
		final_vertex.y = (n1->y + n2->y + n3->y) / 3.0f; // y coord
		final_vertex.z = (n1->z + n2->z + n3->z) / 3.0f; // z coord

		face_normals[new_array_pos++] = debug_f_vertices[i]; // x
		face_normals[new_array_pos++] = debug_f_vertices[i + 1]; // y
		face_normals[new_array_pos++] = debug_f_vertices[i + 2]; //z

		face_normals[new_array_pos++] = debug_f_vertices[i] + final_vertex.x * normal_lenght; // x
		face_normals[new_array_pos++] = debug_f_vertices[i + 1] + final_vertex.y * normal_lenght; // y
		face_normals[new_array_pos++] = debug_f_vertices[i + 2] + final_vertex.z * normal_lenght; // z*/


	/*return ret;
}*/