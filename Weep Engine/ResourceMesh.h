#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__

#include "Resource.h"


class ResourceMesh : public Resource
{
public:
	ResourceMesh(UID uid, Resource::Type type) : Resource(uid, type) {};
	//virtual ~ResourceTexture();
	//bool LoadInMemory() override;
	//void Save(Config& config) const override;
	//void Load(const Config& config) override;
public:
	//uint width = 0;
	//uint height = 0;
	//uint depth = 0;
	//uint mips = 0;
	//uint bytes = 0;
	//uint gpu_id = 0;
	//Format format = unknown;
};
#endif // !__RESOURCEMESH_H__
