#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"
#include "Globals.h"
class aiMesh;
class GeometryShape;
class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	virtual ~ModuleImporter() {};

	bool Start() override;
	bool LoadFBX(std::string &path);
	void LoadIndices(GeometryShape * model, aiMesh * mesh);
	void LoadVertices(GeometryShape * model, aiMesh * mesh);
	bool CleanUp() override;

	std::string GetPath() const;
	void LoadPath(const std::string &path);

private:

	std::string path;
};

#endif // !__MODULEIMPORTER_H__

