#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"
#include "Globals.h"
struct aiMesh;
class ComponentMesh;
class ResourceMesh;
class ComponentTexture;
struct aiScene;
class GameObject;

template<class my_node>
class Node
{
public:
	my_node* current_node;
	Node<my_node>* parent = nullptr;
	GameObject* current_go = nullptr;
public:

	Node(my_node* current, Node<my_node>* parent, GameObject* go = nullptr)
	{
		current_node = current;
		this->parent = parent;
		this->current_go = go;
	}
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	virtual ~ModuleImporter() {};

	bool Start() override;

	bool CleanUp() override;
	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);

	bool LoadFBX(const char* path);
	void LoadAllMeshes(const aiScene * scene);
	void LoadMaterials(const aiScene * scene, aiMesh * mesh, ComponentTexture * model);
	void LoadUVs(ResourceMesh * model, aiMesh * mesh);
	void LoadNormals(ResourceMesh * model, aiMesh * mesh);
	void LoadVertices(ResourceMesh * model, aiMesh * mesh);
	void LoadIndexs(ResourceMesh * model, aiMesh * mesh);

	void CreateOwnFile(const ResourceMesh* mesh, const string name_to_file);
	void LoadOwnFile(string name_file, ComponentMesh* mesh);

	const char* GetPath() const;
	void LoadPath(char* path);

private:

	char* path = "";
};

#endif // !__MODULEIMPORTER_H__

