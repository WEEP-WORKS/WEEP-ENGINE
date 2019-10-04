#include "GeometryShape.h"
#include "Globals.h"

GeometrySphere* GeometryShapeManager::CreateSphere(const int& sphere_subdivisions)
{
	GeometryShape* s = new GeometrySphere();
	s->mesh = par_shapes_create_subdivided_sphere(sphere_subdivisions);
	s->vertexs = s->mesh->points;
	s->triangle_indices = (uint*)s->mesh->triangles;

	s->Start();

	shapes.push_back(s);

	return (GeometrySphere*)s;
}

bool GeometryShapeManager::Update()
{
	DrawAll();

	return true;
}

void GeometryShapeManager::DrawAll() 
{
	for (list<GeometryShape*>::iterator item = shapes.begin(); item != shapes.end(); ++item)
	{
		(*item)->Render();
	}
}

bool GeometryShapeManager::CleanUp()
{
	for (list<GeometryShape*>::iterator item = shapes.begin(); item != shapes.end(); ++item)
	{
		RELEASE(*item);
	}
	return true;
}

void GeometryShapeManager::AddShape(GeometryShape* shape)
{
	shape->Start();
	shapes.push_back(shape);
}








void GeometryShape::Render()
{
	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_triangle_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, num_triangle_indices * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void GeometryShape::SetBuffersWithData()
{
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertex * 3, vertexs, GL_STATIC_DRAW);

	glGenBuffers(1, &id_triangle_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_triangle_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*num_triangle_indices * 3, triangle_indices, GL_STATIC_DRAW);
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

