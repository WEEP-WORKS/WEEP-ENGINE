#ifndef __RESOURCEMANAGMENT_H__
#define __RESOURCEMANAGMENT_H__

#include "Module.h"

//typedef unsigned long long UID;
#include "Globals.h"
#include "Resource.h"
#include "ResourceMesh.h"
//class Resource { public:	enum class Type; };


class ResourceManagment : public Module
{
public:
	ResourceManagment();
	//UID Find(const char* file_in_assets) const;
	//UID ImportFile(const char* new_file_in_assets, bool force = false);
	UID GenerateNewUID();

	const Resource* GetByID(UID uid) const;
	Resource* GetByID(UID uid);
	const std::vector<ResourceMesh*> GetAllMeshes() const;

	UID CreateNewResource(Resource::Type type);
private:
	std::map<UID, Resource*> resources;

	UID last_uid = 0u;
};

#endif // !__RESOURCEMANAGMENT_H__

