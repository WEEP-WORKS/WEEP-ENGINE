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
	template <typename BufferType>
	struct BuffersData
	{
		bool has_data		= false;	// If exist the data type.
		uint num			= 0u;		// Number of the data type.

		GLuint id_buffer	= 0u;		// Id of the buffer.
		BufferType* buffer		= nullptr;	// The buffer with data.
		uint buffer_size	= 0u;		// The size of the buffer.

	};

	par_shapes_mesh* mesh = nullptr;

	//Vertexs:------
	BuffersData<float> vertexs;
	

	//Indexs:------
	BuffersData<uint> indexs;


	//Normals:------
	uint num_faces						= 0u;		// Number of faces of the mesh.
	float normal_lenght					= 0.5f;		// Lenght of all normals.

	BuffersData<float> normals_direction;

	BuffersData<float> normal_vertexs;

	BuffersData<float> normal_faces;


	//UVs
	uint num_uvs_channels				= 0u; //local var?
	uint channel_buffer_size			= 0u; //local var?

	BuffersData<float> uvs;


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

	inline Vector3<float>* ReturnNormalDirectionByIndex(const uint&) const;

	 
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
