#include "App.h"
#include "ModuleImporter.h"
#include "GeometryShape.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled) 
{
	SetName("Importer");
}

bool ModuleImporter::Start()
{
	bool ret = true;

	//?
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	

	return ret;
}

void ModuleImporter::LoadPath(char* path)
{
	this->path = path;
}

char* ModuleImporter::GetPath() const
{
	return path;
}

bool ModuleImporter::LoadFBX(char* path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		LoadAllMeshes(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}

	aiReleaseImport(scene);

	return ret;
}

void ModuleImporter::LoadAllMeshes(const aiScene * scene)
{
	for (uint i = 0; i < scene->mNumMeshes; ++i)
	{
		GeometryShape* model = new FBXShape();
		aiMesh* mesh = scene->mMeshes[i];

		LoadVertices(model, mesh);

		if (mesh->HasFaces())
		{
			LoadIndices(model, mesh);
		}

		if (mesh->HasNormals())
		{
			model->has_normals = true;

			model->num_normals = model->num_vertex;
			model->normals = new float[model->num_normals * 3];
			memcpy(model->normals, mesh->mNormals, sizeof(float) * model->num_normals * 3); //It could be QNaN?

			model->CalculateNormals();
		}

		App->shape_manager->AddShape(model);

	}
}

void ModuleImporter::LoadVertices(GeometryShape * model, aiMesh * mesh)
{
	model->num_vertex = mesh->mNumVertices; // get number of Vertices
	model->vertexs = new float[mesh->mNumVertices * 3]; // create array of Vertices with the correct size
	memcpy(model->vertexs, mesh->mVertices, sizeof(float) * model->num_vertex * 3); // copy the vertices of the mesh to the arrey of vertices

	LOG("New mesh with %d vertices", model->num_vertex);
}

void ModuleImporter::LoadIndices(GeometryShape * model, aiMesh * mesh)
{
	model->num_indices = mesh->mNumFaces * 3; // get number of indices. Every face has 3 indices, assuming each face is a triangle
	model->indices = new uint[model->num_indices]; // create array of indices with the correct size
	for (uint i = 0; i < mesh->mNumFaces; ++i)
	{
		if (mesh->mFaces[i].mNumIndices != 3) // if the face is not a triangle don't load it.
		{
			LOG("This face don't have 3 index, only can load faces with 3 indexs");
		}
		else
		{
			// Every face have 3 indices. 
			// take the first 3 slots, 
			//then the next 3 slots, 
			//then the same ...                                               3 indices * their var type, only copy 1 face (3 indices) every time
			memcpy(&model->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint)); // Copy the Indices of the mesh to the array of indices.
		}
	}
}

bool ModuleImporter::CleanUp()
{
	bool ret = true;

	aiDetachAllLogStreams();

	return ret;

}