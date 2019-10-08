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
		glGenBuffers(1, &id_normals);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_normals * 3, vertexs_normals, GL_STATIC_DRAW);

	}

}

void FBXShape::CalculateNormals()
{
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
}

void FBXShape::Render()
{
	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);



	

	glBindBuffer(GL_ARRAY_BUFFER, id_normals);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, num_normals*6);


	glDisableClientState(GL_VERTEX_ARRAY);


	
}