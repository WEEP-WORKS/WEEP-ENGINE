#ifndef __GEOMETRYSHAPE_H__
#define __GEOMETRYSHAPE_H__

#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"
#include "glew/glew.h"
#include "Color.h"
#include "Vector3.h"


//----------------------------------------------------------------------------------
//-------------------------------GEOMETRY SHAPE BASIC-------------------------------
//----------------------------------------------------------------------------------
class GeometryShape
{
public:
	par_shapes_mesh* mesh = nullptr;

	//TODO: all repeat values in a struct? values repeat: array, size,array,id,num of x.
	//Vertexs:------
	uint num_vertex						= 0;		// Number of vertexs of the mesh without count each coordinate. Only how many indexs are there. (indexed)
	GLuint id_vertex_buffer				= 0;		// Id of the vertexs_buffer.
	float* vertexs_buffer				= nullptr;	// Buffer of vertexs coordinates of the mesh.
	uint vertexs_buffer_size			= 0u;		// Size of the vertexs_buffer.



	//Indexs:------
	uint num_indexs						= 0;		// The number of indexs of the mesh. The same of the indexs_buffer_size, for this reason indexs don't have indexs_buffer_size!!.
	GLuint id_indexs_buffer				= 0;		// Id of the indexs_buffer.
	uint* indexs_buffer					= nullptr;	// Buffer of indexs of the mesh.



	//Normals:------
	bool has_normals					= false;	// If the mesh have normals.
	uint num_faces						= 0u;		// Number of faces of the mesh.
	float normal_lenght					= 0.5f;		// Lenght of all normals.

	uint normals_direction_buffer_size	= 0u;		// Buffer size of the normals_direction_buffer.
	float* normals_direction_buffer		= nullptr;	// Buffer with the direction of the normals of vertex_normals.

	uint num_vertex_normals				= 0u;		// The number of vertexs normals. The same as the number of vertexs because every vertexs have a vertex_normal.
	GLuint id_vertex_normals_buffer		= 0u;		// Id of the vertex_normals_buffer.
	float* vertex_normals_buffer		= nullptr;	// Buffer with the vertexs_normals coordinates of the mesh. 
	uint vertex_normals_buffer_size		= 0u;		// Size of the vertexs_normals_buffer.

	uint num_face_normals				= 0u;		// Number of face_normals of the mesh.	
	GLuint id_face_normals_buffer		= 0u;		// Id of the face_normals_buffer.
	float* face_normals_buffer			= nullptr;	// Buffer with the faces_normals coordinates of the mesh.
	uint face_normals_buffer_size		= 0u;		// size of the buffer.


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

	FBXShape() {};

	void Render() override;

	void RenderVertexsWithIndices();

	void RenderVertexNormals();

	void RenderFaceNormals();

	void SetBuffersWithData() override;

	void CalculateNormals() override;

	void CalculateFacesNormals();

	void CalculateVertexsNormals();

	inline Vector3<float>* ReturnVertexByIndex(const uint&) const;

	inline Vector3<float>* ReturnNormalByIndex(const uint&) const;

	 
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

