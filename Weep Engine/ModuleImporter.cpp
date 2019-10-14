#include "App.h"
#include "ModuleImporter.h"
#include "ModuleGameObjectManager.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleTexture.h"

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

bool ModuleImporter::CleanUp()
{
	bool ret = true;

	aiDetachAllLogStreams();

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
		GameObject* object = new GameObject();
		ComponentMesh* model = (ComponentMesh*)object->AddComponent(ComponentType::MESH);
		aiMesh* mesh = scene->mMeshes[i];

		LoadVertices(model, mesh);

		if (mesh->HasFaces())
		{
			LoadIndexs(model, mesh);
		}

		if (mesh->HasNormals())
		{
			LoadNormals(model, mesh);
		}

		ComponentTexture* text = (ComponentTexture*)object->AddComponent(ComponentType::TEXTURE);
		//model->num_uvs_channels = mesh->GetNumUVChannels(); 
		if (mesh->GetNumUVChannels() > 0)
		{
			LoadUVs(text, mesh);
		}

		if (scene->HasMaterials())
		{
			LoadMaterials(scene, mesh, text);
		}
		model->SetBuffersWithData();
		text->SetBuffersWithData();//this could be in the function of load uvs?
		App->game_object_manager->AddObject(object);

	}
}

// ----------------------------Vertexs----------------------------

void ModuleImporter::LoadVertices(ComponentMesh * model, aiMesh * mesh)
{
	model->vertexs.has_data = true;

	model->vertexs.num = mesh->mNumVertices; // get number of Vertices

	model->vertexs.buffer_size = model->vertexs.num * 3;
	model->vertexs.buffer = new float[model->vertexs.buffer_size]; // create array of Vertices with the correct size

	memcpy(model->vertexs.buffer, mesh->mVertices, sizeof(float) * model->vertexs.buffer_size); // copy the vertices of the mesh to the arrey of vertices

	LOG("New mesh with %d vertices", model->vertexs.num);
}

// ----------------------------Indexs----------------------------

void ModuleImporter::LoadIndexs(ComponentMesh * model, aiMesh * mesh)
{
	model->indexs.has_data = true;

	model->num_faces = mesh->mNumFaces;
	model->indexs.num = model->num_faces * 3; // get number of indices. Every face has 3 indices, assuming each face is a triangle

	model->indexs.buffer_size = model->indexs.num; // don't have coordinates, so the number of indexs == buffer_size.
	model->indexs.buffer = new uint[model->indexs.buffer_size]; // create array of indices with the correct size

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
			memcpy(&model->indexs.buffer[i * 3], mesh->mFaces[i].mIndices, /*TODO Change 3 by a var*/3 * sizeof(uint)); // Copy the Indices of the mesh to the array of indices.
		}
	}
}

// ----------------------------Normals----------------------------

void ModuleImporter::LoadNormals(ComponentMesh * model, aiMesh * mesh)
{
	//load normals direction of the vertex_normals.
	model->normals_direction.has_data = true;
	model->normal_vertexs.has_data = true;
	model->normal_faces.has_data = true;

	model->normals_direction.num = model->vertexs.num;
	model->normal_vertexs.num = model->vertexs.num;
	model->normal_faces.num = model->num_faces;

	model->normals_direction.buffer_size = model->normal_vertexs.num * 3/*every vertex_normal have 3 coordinates (x, y, z).*/;
	model->normals_direction.buffer = new float[model->normals_direction.buffer_size];
	memcpy(model->normals_direction.buffer, mesh->mNormals, sizeof(float) * model->normals_direction.buffer_size); //It could be QNaN?

	model->CalculateNormals();
}

 //----------------------------UVs----------------------------

void ModuleImporter::LoadUVs(ComponentTexture * text, aiMesh * mesh)
{
	text->uvs.has_data = true;

	text->uvs.num = mesh->mNumVertices; //every vertex have one vector (only 2 dimensions will considerate) of uvs.

	text->uvs.buffer_size = text->num_uvs_channels * text->uvs.num * 2/*only save 2 coordinates, the 3rt coordinate will be always 0, so don't save it*/; // number of uvs * number of components of the vector (2) * number of channels of the mesh
	text->uvs.buffer = new float[text->uvs.buffer_size];

	text->channel_buffer_size = text->uvs.num * 2;//the same as uvs_buffer_size without the multiplication by the number of channels because we want to save only the size of 1 channel.
	for (uint channel = 0; channel < text->num_uvs_channels; ++channel)
	{
		if (mesh->HasTextureCoords(channel)) // if this channel have texture coords...
		{

			if (mesh->mNumUVComponents[channel] == 2) //the channel have vectors of 2 components
			{
				for (uint j = 0; j < text->uvs.num; ++j)
				{								//start index of the current channel.  start index of the current channel of the mesh.  Only copy the values in 1 channel size.
					memcpy(&text->uvs.buffer[(channel * text->channel_buffer_size) + j*2], &mesh->mTextureCoords[channel][j], sizeof(float) * 2);

				}
			}
			else // if the channel don't have 2 components by vector, don't save it and fill it with 0.
			{
				memset(&text->uvs.buffer[channel * text->channel_buffer_size], 0, sizeof(float) * text->channel_buffer_size);
			}
		}
	}
}

 //----------------------------Materials----------------------------

void ModuleImporter::LoadMaterials(const aiScene * scene, aiMesh * mesh, ComponentTexture * model)
{
	model->has_texture = true;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE); //only load DIFFUSE textures.

	if (numTextures > 0)
	{
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		std::string dir = "Models/Textures/";

		model->id_texture = App->texture->LoadTexture(path.C_Str());


	
	}
}