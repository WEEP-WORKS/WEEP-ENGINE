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
	s->vertexs_buffer = s->mesh->points;
	s->indexs_buffer = (uint*)s->mesh->triangles;

	s->num_vertex = s->mesh->npoints;
	s->num_indexs = s->mesh->ntriangles;

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
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexs_buffer);
	

	glDrawElements(GL_TRIANGLES, num_indexs * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void GeometryShape::SetBuffersWithData()
{
	glGenBuffers(1, &id_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertex * 3, vertexs_buffer, GL_STATIC_DRAW);

	glGenBuffers(1, &id_indexs_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexs_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indexs*3, indexs_buffer, GL_STATIC_DRAW);

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
	vertexs_buffer_size = num_vertex * 3/*each coordinate. Every vertex have x, y, z in the vertexs buffer.*/;
	glGenBuffers(1, &id_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexs_buffer_size, vertexs_buffer, GL_STATIC_DRAW);

	//id_indexs_buffer size == number of indexs
	glGenBuffers(1, &id_indexs_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexs_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indexs, indexs_buffer, GL_STATIC_DRAW);

	if (has_normals)
	{
		glGenBuffers(1, &id_vertex_normals_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex_normals_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex_normals_buffer_size, vertex_normals_buffer, GL_STATIC_DRAW);

		glGenBuffers(1, &id_face_normals_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, id_face_normals_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_indexs * 2, face_normals_buffer, GL_STATIC_DRAW);
	}

}

void FBXShape::CalculateNormals()
{
	CalculateVertexsNormals();

	CalculateFacesNormals();

}

void FBXShape::CalculateVertexsNormals()
{
	//vertex_normals
	vertex_normals_buffer_size = num_vertex * 3/*x, y, z*/ * 2 /* start point, end point*/;
	vertex_normals_buffer = new float[vertex_normals_buffer_size];
	uint new_array_pos = 0u;

	//iterate vertexs array to get the normal of each vertex.
	for (uint i = 0; i < vertexs_buffer_size; i += 3/*each vertex have 3 coords. Every 3 increment we hace another new vertex.x*/)
	{
		//---Start Points:---
		vertex_normals_buffer[new_array_pos++] = vertexs_buffer[i];//current_vertex.x
		vertex_normals_buffer[new_array_pos++] = vertexs_buffer[i + 1];//current_vertex.y
		vertex_normals_buffer[new_array_pos++] = vertexs_buffer[i + 2];//current_vertex.z

		//---End points:---
		//the normal_length is to lengthen or shrink the normal.
		//the normals[i] is to get the coordinate of the normals direction of each corresponding vertex coordinate.
		vertex_normals_buffer[new_array_pos++] = vertexs_buffer[i] + normals_direction_buffer[i] * normal_lenght;
		vertex_normals_buffer[new_array_pos++] = vertexs_buffer[i + 1] + normals_direction_buffer[i + 1] * normal_lenght;
		vertex_normals_buffer[new_array_pos++] = vertexs_buffer[i + 2] + normals_direction_buffer[i + 2] * normal_lenght;
	}
}

void FBXShape::CalculateFacesNormals()
{
	//face_normals--

	//start point of each face_normal. buffer and size
	uint start_vertexs_of_face_normals_size = num_faces * 3;/*1 start point by face. each start point have 3 coordinates (x, y, z).*/
	float * start_vertexs_of_face_normals = new float[start_vertexs_of_face_normals_size]; //aray with start points calculated of the normals. Could be use to render it.
	
	//face_normals_buffer and size. each normal have start and end point.
	face_normals_buffer_size = start_vertexs_of_face_normals_size * 2 /*every start point will have their end point on the face_normals_buffer.*/;
	face_normals_buffer = new float[face_normals_buffer_size];

	uint new_array_pos = 0u;

	//iterate indexs of each face to get the face_normal.
	for (uint i = 0; i < num_indexs/*iterate indexs to find the normals*/; i += 3/*every 3 indexs we have another face*/) // num_indices -> cada cara guarda 3 valores. numero de caras * 3 = num_indices;
	{
		//---Start point:---
		// i = first index of the current face. They are ordered, so i+1 is the 2nd index of the current face, and i+3 the final index of the face
		Vector3<float>* v1 = ReturnVertexByIndex(i);
		Vector3<float>* v2 = ReturnVertexByIndex(i + 1);
		Vector3<float>* v3 = ReturnVertexByIndex(i + 2);


		//average point of the plane and start point of the normal
		start_vertexs_of_face_normals[i] = (v1->x + v2->x + v3->x) / 3.0f; // x coord
		start_vertexs_of_face_normals[i + 1] = (v1->y + v2->y + v3->y) / 3.0f; // y coord
		start_vertexs_of_face_normals[i + 2] = (v1->z + v2->z + v3->z) / 3.0f; // z coord

		//---End point:---
		 //same as the vertex but with the normals. the vertex_normal of each vertex of the face
		Vector3<float>* n1 = ReturnNormalByIndex(i);
		Vector3<float>* n2 = ReturnNormalByIndex(i + 1);
		Vector3<float>* n3 = ReturnNormalByIndex(i + 2);

		// average normal of the 3 vertex_normal of each vertex of the face.
		Vector3<float> final_vertex;
		final_vertex.x = (n1->x + n2->x + n3->x) / 3.0f; // x coord
		final_vertex.y = (n1->y + n2->y + n3->y) / 3.0f; // y coord
		final_vertex.z = (n1->z + n2->z + n3->z) / 3.0f; // z coord

		face_normals_buffer[new_array_pos++] = start_vertexs_of_face_normals[i]; // start point coord x
		face_normals_buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 1]; // start point coord y
		face_normals_buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 2]; // start point coord z

		//the normal_length is to lengthen or shrink the normal.
		face_normals_buffer[new_array_pos++] = start_vertexs_of_face_normals[i] + final_vertex.x * normal_lenght; // end point coord x
		face_normals_buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 1] + final_vertex.y * normal_lenght; // end point coord y
		face_normals_buffer[new_array_pos++] = start_vertexs_of_face_normals[i + 2] + final_vertex.z * normal_lenght; // end point coord z

	}
}



Vector3<float>* FBXShape::ReturnVertexByIndex(const uint &index) const
{
	Vector3<float>* ret = new Vector3<float>();
	float* first_vertex = &vertexs_buffer[indexs_buffer[index] * 3/*every index have 3 coords saved in vertexs*/];

	ret->x = first_vertex[0];
	ret->y = first_vertex[1];
	ret->z = first_vertex[2];

	return ret;
}

Vector3<float>* FBXShape::ReturnNormalByIndex(const uint &index) const
{
	Vector3<float>* ret = new Vector3<float>();
	float* first_vertex = &normals_direction_buffer[indexs_buffer[index] * 3/*every index have 3 coords saved in vertexs*/];

	ret->x = first_vertex[0];
	ret->y = first_vertex[1];
	ret->z = first_vertex[2];

	return ret;
}



void FBXShape::Render()
{
	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);

	RenderVertexsWithIndices();

//	RenderVertexNormals();

	RenderFaceNormals();

	glDisableClientState(GL_VERTEX_ARRAY);
	
}

void FBXShape::RenderVertexsWithIndices()
{
	
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indexs_buffer);

	glDrawElements(GL_TRIANGLES, num_indexs, GL_UNSIGNED_INT, NULL);
}

void FBXShape::RenderVertexNormals()
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex_normals_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, num_vertex_normals * 3);

}

void FBXShape::RenderFaceNormals()
{
	glBindBuffer(GL_ARRAY_BUFFER, id_face_normals_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, num_indexs);
}
