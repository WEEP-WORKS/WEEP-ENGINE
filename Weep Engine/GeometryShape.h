#ifndef __GEOMETRYSHAPE_H__
#define __GEOMETRYSHAPE_H__

#include "Globals.h"
#include "par_shapes.h"

class GeometryShape
{
public:
	par_shapes_mesh* mesh = nullptr;
	uint id_vertex = 0;
	uint id_index = 0;

	GeometryShape() {};
	~GeometryShape()
	{
		par_shapes_free_mesh(mesh);
	};

	virtual void Render() = 0;
};

class GeometrySphere : public GeometryShape
{
public:
	GeometrySphere() {}

	void Render() override;
};

#endif // !__GEOMETRYSHAPE_H__

