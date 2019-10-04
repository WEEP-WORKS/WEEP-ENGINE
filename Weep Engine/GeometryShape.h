#ifndef __GEOMETRYSHAPE_H__
#define __GEOMETRYSHAPE_H__

#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"
#include "glew/glew.h"
#include "Color.h"


class GeometryShape
{
public:
	par_shapes_mesh* mesh = nullptr;
	GLuint id_vertex = 0;
	uint num_vertex = 0;
	float* vertexs;

	GLuint id_index = 0;
	uint num_index = 0;
	uint* indexs;
	Color color;

	GeometryShape() { color.r = color.g = color.b = 1.f;};
	~GeometryShape()
	{
		par_shapes_free_mesh(mesh);
	};


	void Start();
	virtual void Render() {};
	void MoveShape(float, float, float);
	void SetColor(float, float, float);

protected:

	void SetBuffersWithData(); 
};

class GeometrySphere : public GeometryShape
{
public:
	GeometrySphere() {}

	void Render() override;

};

class GeometryShapeManager : public Module
{
public:
	GeometryShapeManager(bool start_enabled = true) : Module(start_enabled) {};
	~GeometryShapeManager() {};

	bool Update() override;
	bool CleanUp() override;

	GeometrySphere* CreateSphere(const int &);

private:

	void DrawAll();

private:
	std::list<GeometryShape*> shapes;



	
};





#endif // !__GEOMETRYSHAPE_H__

