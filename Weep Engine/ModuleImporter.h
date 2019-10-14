#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"
#include "Globals.h"
class aiMesh;
class ComponentMesh;
class aiScene;
class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	virtual ~ModuleImporter() {};

	bool Start() override;

	bool CleanUp() override;

	bool LoadFBX(char* path);
	void LoadAllMeshes(const aiScene * scene);
	//void LoadMaterials(const aiScene * scene, aiMesh * mesh, ComponentMesh * model);
	//void LoadUVs(ComponentMesh * model, aiMesh * mesh);
	void LoadNormals(ComponentMesh * model, aiMesh * mesh);
	void LoadVertices(ComponentMesh * model, aiMesh * mesh);
	void LoadIndexs(ComponentMesh * model, aiMesh * mesh);

	char* GetPath() const;
	void LoadPath(char* path);

private:

	char* path;
};

#endif // !__MODULEIMPORTER_H__

