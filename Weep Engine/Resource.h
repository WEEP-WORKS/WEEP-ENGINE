#ifndef __RESOURCE_H__
#define __RESOURCE_H__


typedef unsigned long long UID;

class Resource
{
public:
	enum class Type {
		TEXTURE,
		MESH,
		SCENE,
		UNKNOWN
	};


	Resource(UID uid, Resource::Type type) : uid(uid), type(type) {};
	//virtual ~Resource();
	//Resource::Type GetType() const;
	//const UID GetResourceID() const { return uid; };
	//const char* GetFile() const;
	//const char* GetImportedFile() const;
	//bool IsLoadedToMemory() const;
	//bool LoadToMemory();
//	uint CountReferences() const;
	//virtual void Save(Json::Value& config) const;
	//virtual void Load(const Json::Value& config);
	//virtual bool LoadInMemory() = 0;protected:
	UID uid = 0u;
	Type type = Type::UNKNOWN;
	//std::string file;
	//std::string imported_file;

	//uint loaded = 0;
};

#endif // !__RESOURCE_H__
