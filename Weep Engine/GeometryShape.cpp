#include "GeometryShape.h"
#include "glew/glew.h"

void GeometrySphere::Render()
{
	glColor3f(1.f, 0.f, 0.f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}