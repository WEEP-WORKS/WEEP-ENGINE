#ifndef __GEOMETRYSHAPE_H__
#define __GEOMETRYSHAPE_H__

#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"
#include "glew/glew.h"
#include "Color.h"

//----------------------------------------------------------------------------------
//-------------------------------GEOMETRY SHAPE BASIC-------------------------------
//----------------------------------------------------------------------------------

class GeometryShape
{
public:
	par_shapes_mesh* mesh = nullptr;

	GLuint id_vertex = 0;
	uint num_vertex = 0;
	float* vertexs;

	GLuint id_triangle_indices = 0;
	uint num_triangle_indices = 0;
	uint* triangle_indices;

	bool has_normals = false;
	GLuint id_normals = 0;
	uint num_normals = 0;
	float* normals;

	Color color;

	GeometryShape() { color.r = color.g = color.b = 1.f;};
	~GeometryShape()
	{
		if (mesh != nullptr)
		{
			par_shapes_free_mesh(mesh);
		}
	};


	void Start();
	virtual void Render();
	void MoveShape(float, float, float);
	void SetColor(float, float, float);

protected:

	virtual void SetBuffersWithData(); 
};

//----------------------------------------------------------------------------------
//--------------------------------------SPHERE--------------------------------------
//----------------------------------------------------------------------------------

class GeometrySphere : public GeometryShape
{
public:
	GeometrySphere() {}
};


//----------------------------------------------------------------------------------
//-------------------------------------FBX SHAPE------------------------------------
//----------------------------------------------------------------------------------
class FBXShape : public GeometryShape
{
public:
	FBXShape() {};

	void Render() override;

	void SetBuffersWithData() override;
};



//----------------------------------------------------------------------------------
//---------------------------------GEOMETRY MANAGER---------------------------------
//----------------------------------------------------------------------------------



class ShapeManager : public Module
{
public:
	ShapeManager(bool start_enabled = true);
	virtual ~ShapeManager() {};

	bool Update() override;
	bool CleanUp() override;

	GeometrySphere* CreateSphere(const int &);
	void AddShape(GeometryShape*);

private:

	void DrawAll();

private:
	std::list<GeometryShape*> shapes;
	   	 
};





#endif // !__GEOMETRYSHAPE_H__

