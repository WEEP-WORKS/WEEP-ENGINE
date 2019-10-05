#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"
#include "Globals.h"
class aiMesh;
class GeometryShape;
class aiScene;
class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	virtual ~ModuleImporter() {};

	bool Start() override;

	bool CleanUp() override;

	bool LoadFBX(std::string &path);
	void LoadAllMeshes(const aiScene * scene);
	void LoadVertices(GeometryShape * model, aiMesh * mesh);
	void LoadIndices(GeometryShape * model, aiMesh * mesh);

	std::string GetPath() const;
	void LoadPath(const std::string &path);

private:

	std::string path;
};

#endif // !__MODULEIMPORTER_H__

