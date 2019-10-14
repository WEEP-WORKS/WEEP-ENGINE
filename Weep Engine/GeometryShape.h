#ifndef __GEOMETRYSHAPE_H__
#define __GEOMETRYSHAPE_H__

#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"
#include "glew/glew.h"
#include "Color.h"
#include "Vector3.h"

enum class ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH,
	TEXTURE
};


class Component
{
public:
	ComponentType type = ComponentType::NONE;

public:
	virtual void Update() {};
};

class ComponentMesh : public Component
{
public:
	template <typename BufferType>
	struct BuffersData
	{
		bool has_data = false;	// If exist the data type.
		uint num = 0u;		// Number of the data type.

		GLuint id_buffer = 0u;		// Id of the buffer.
		BufferType* buffer = nullptr;	// The buffer with data.
		uint buffer_size = 0u;		// The size of the buffer.

	};



	//Vertexs:------
	BuffersData<float> vertexs;


	//Indexs:------
	BuffersData<uint> indexs;


	//Normals:------
	uint num_faces = 0u;		// Number of faces of the mesh.
	float normal_lenght = 0.5f;		// Lenght of all normals.

	BuffersData<float> normals_direction;

	BuffersData<float> normal_vertexs;

	BuffersData<float> normal_faces;

	Color color;

public:
	ComponentMesh();
	~ComponentMesh() {};

	void Update() override;

	void SetBuffersWithData();

	void CalculateNormals();

private:

	void Render();

	void RenderModel();

	void CalculateFacesNormals();

	void CalculateVertexsNormals();

	void RenderVertexNormals();

	void RenderFaceNormals();

	inline Vector3<float>* ReturnVertexByIndex(const uint&) const;

	inline Vector3<float>* ReturnNormalDirectionByIndex(const uint&) const;
};


class GameObject
{
public:
	GameObject();

	void Update(); //this is not from the module class. This function will be called from objectManager and will call Components update or something... I don't know yet.

	Component* AddComponent(ComponentType);
public:
	std::vector<Component*> components;
};



class GameObjectManager : public Module
{
public:
	GameObjectManager(bool start_enabled = true);
	virtual ~GameObjectManager() {};

	bool Update() override;
	bool CleanUp() override;

	//GameObject* CreateSphere(const int &);
	void AddObject(GameObject*);

private:
	std::list<GameObject*> objects; //Vector or list?
};

//----------------------------------------------------------------------------------
//-------------------------------GEOMETRY SHAPE BASIC-------------------------------
//----------------------------------------------------------------------------------




//---------------
 //All old things like geometry things don't erase yet. (but it's unused)
//-------------


class GeometryShape
{
public:
	template <typename BufferType>
	struct BuffersData
	{
		bool has_data = false;	// If exist the data type.
		uint num = 0u;		// Number of the data type.

		GLuint id_buffer = 0u;		// Id of the buffer.
		BufferType* buffer = nullptr;	// The buffer with data.
		uint buffer_size = 0u;		// The size of the buffer.

	};

	

	//Vertexs:------
	BuffersData<float> vertexs;


	//Indexs:------
	BuffersData<uint> indexs;


	//Normals:------
	uint num_faces = 0u;		// Number of faces of the mesh.
	float normal_lenght = 0.5f;		// Lenght of all normals.

	BuffersData<float> normals_direction;

	BuffersData<float> normal_vertexs;

	BuffersData<float> normal_faces;

	//----
	par_shapes_mesh* mesh = nullptr;

	//UVs
	uint num_uvs_channels				= 0u; //local var?
	uint channel_buffer_size			= 0u; //local var?

	BuffersData<float> uvs;

	//Textures
	bool has_texture = false;
	GLuint id_texture = 0;
	


	Color color;

	GeometryShape() { color.r = color.g = color.b = 1.f;};
	~GeometryShape()
	{
		par_shapes_free_mesh(mesh);
	
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

	void RenderModel();

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

