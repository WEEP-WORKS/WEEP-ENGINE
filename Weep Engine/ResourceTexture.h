#ifndef __RESOURCETEXTURE_H__
#define __RESOURCETEXTURE_H__

#include "Resource.h"


class ResourceTexture : public Resource
{
public:
	ResourceTexture(UID uid, Resource::Type type) : Resource(uid, type) {};
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

	uint			id_texture = 0;
	std::string		texture_path = "";
	int				texture_width = 0;
	int				texture_height = 0;
};
#endif // !__RESOURCETEXTURE_H__

