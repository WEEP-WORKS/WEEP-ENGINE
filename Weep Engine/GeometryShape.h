#ifndef __GEOMETRYSHAPE_H__
#define __GEOMETRYSHAPE_H__

#include "Module.h"
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

	virtual void Render() const {};
};

class GeometrySphere : public GeometryShape
{
public:
	GeometrySphere() {}

	void Render() const override;

};

class GeometryShapeManager : public Module
{
public:
	GeometryShapeManager(bool start_enabled = true) : Module(start_enabled) {};
	~GeometryShapeManager() {};

	bool Update() override;

	GeometrySphere* CreateSphere(const int &);

private:

	void DrawAll();

private:
	std::list<GeometryShape*> shapes;
};





#endif // !__GEOMETRYSHAPE_H__

