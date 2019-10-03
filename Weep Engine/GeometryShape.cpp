#include "GeometryShape.h"
#include "glew/glew.h"

void GeometrySphere::Render() const
{
	glColor3f(1.f, 0.f, 0.f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

GeometrySphere* GeometryShapeManager::CreateSphere(const int& sphere_subdivisions)
{
	GeometryShape* s = new GeometrySphere();
	s->mesh = par_shapes_create_subdivided_sphere(sphere_subdivisions);

	glGenBuffers(1, &s->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, s->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * s->mesh->npoints * 3, s->mesh->points, GL_STATIC_DRAW);

	glGenBuffers(1, &s->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*s->mesh->ntriangles * 3, s->mesh->triangles, GL_STATIC_DRAW);

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