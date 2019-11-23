#include "ResourceManagment.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"

#include "Globals.h"



ResourceManagment::ResourceManagment()
{
	SetName("Texture");
}
UID ResourceManagment::CreateNewResource(Resource::Type type)
{
	Resource* ret = nullptr;
	UID uid = GenerateNewUID();
	switch (type) {
	case Resource::Type::TEXTURE: ret = (Resource*) new ResourceTexture(uid, type); break;
	case Resource::Type::MESH: ret = (Resource*) new ResourceMesh(uid, type); break;
	//case Resource::Type::SCENE: ret = (Resource*) new ResourceScene(uid); break;
	}
	if (ret != nullptr)
		resources[uid] = ret;

	return uid;
}

UID ResourceManagment::GenerateNewUID()
{
	return ++last_uid;//This has to be 1 or greater. in id 0 there should be no resource.
}

Resource* ResourceManagment::GetByID(UID id)
{
	std::map<UID, Resource*>::iterator it = resources.find(id);
	if (it != resources.end())
		return it->second;
	return nullptr;

}

const Resource* ResourceManagment::GetByID(UID id) const
{
	std::map<UID, Resource*>::const_iterator it = resources.find(id);
	if (it != resources.end())
		return it->second;
	return nullptr;

}

const std::vector<ResourceMesh*> ResourceManagment::GetAllMeshes() const
{
	std::vector<ResourceMesh*> meshes;
	for (std::map<UID, Resource*>::const_iterator iter = resources.cbegin(); iter != resources.cend(); ++iter)
	{
		if ((*iter).second->GetType() == Resource::Type::MESH)
			meshes.push_back((ResourceMesh*)(*iter).second);
	}

	return meshes;
}