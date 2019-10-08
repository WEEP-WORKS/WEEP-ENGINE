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

	GLuint id_indices = 0;
	uint num_indices = 0;
	uint* indices;

	bool has_normals = false;
	GLuint id_vertex_normals = 0;
	GLuint id_face_normals = 0;
	uint num_normals = 0;
	uint num_faces = 0;
	float* normals = nullptr;

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

	virtual void CalculateNormals() {};

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

	float* vertexs_normals = nullptr;

	float* face_normals = nullptr;

	float normal_lenght = 1.f;

	FBXShape() {};

	void Render() override;

	void RenderVertexsWithIndices();

	void RenderVertexNormals();

	void SetBuffersWithData() override;

	void CalculateNormals() override;
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

