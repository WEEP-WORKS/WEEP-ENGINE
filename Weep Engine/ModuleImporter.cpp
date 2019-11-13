#include "App.h"
#include "ModuleImporter.h"
#include "ModuleGameObjectManager.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleTexture.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/version.h"

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

void ModuleImporter::OnLoadFile(const char* file_path, const char* file_name, const char* file_extension)
{
	if (strcmp("fbx", file_extension) == 0)
	{
		LoadFBX(file_path);
	}
}

bool ModuleImporter::LoadFBX(const char* path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);

	if (scene != nullptr && scene->HasMeshes())
	{
		LoadPath((char*)path);
		LoadAllMeshes(scene);
	}
	else
	{
		LOG("Error loading scene %s.", path);
		ret = false;
	}

	aiReleaseImport(scene);

	return ret;
}

void ModuleImporter::LoadAllMeshes(const aiScene * scene)
{

	std::list<Node<aiNode>> go_to_create;
	std::list<Node<aiNode>> go_created;
	std::string n = "group_"; n += App->GetFileNameWithoutExtension(GetPath()); n += "_"; n += std::to_string(App->game_object_manager->GetAllGameObjectNumber());
	GameObject* root_go = new GameObject(n, nullptr);

	
	go_to_create.push_back(Node<aiNode>(scene->mRootNode, nullptr, root_go));

	while (!go_to_create.empty())
	{
		//Take the first element in list go_to_create and add to the front of the list to_delete.
		go_created.push_front(*go_to_create.begin());

		//Take the first element every time because the push is for the front, and the new element will be in te begin of the list.
		Node<aiNode>* current = &(*go_created.begin());

		//We are evaluating the first node in the list go_to_create, once is evaluated we don't want to reevaluate... so pop it!
		go_to_create.pop_front();

		for (uint i = 0; i < current->current_node->mNumChildren; ++i)
		{
			//add all the childrens of the current node to the list go_to_create, and set the parent as the current node.
			go_to_create.push_back(Node<aiNode>(current->current_node->mChildren[i], current));
		}

		for (uint i = 0; i < current->current_node->mNumMeshes; ++i)
		{
			//load current

			// ignore aiNodes with no game object, all transformation, rotations...
			Node<aiNode>* parent = current->parent;
			while (parent->current_go == nullptr)
			{
				parent = parent->parent;
			}

			//create gameObject.
			string name = current->current_node->mName.C_Str();//App->GetFileNameWithoutExtension(GetPath()); name += "_"; name += std::to_string(parent->current_go->childrens.size() + 1/*plus 1 to start in 1 nad not in 0*/);
			GameObject* object = new GameObject(name.c_str(), parent->current_go);

			ComponentMesh* model = (ComponentMesh*)object->AddComponent(ComponentType::MESH);
			aiMesh* mesh = scene->mMeshes[current->current_node->mMeshes[i]];


			current->current_go = object;

			// Set mesh pos, rot and scale
			float3 position_i(0, 0, 0);
			Quat rotation_i(0, 0, 0, 0);
			float3 scale_i(0, 0, 0);

			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;

			aiNode* node = scene->mRootNode->mChildren[i];
			if (node != nullptr)
			{
				node->mTransformation.Decompose(scaling, rotation, translation);
				position_i = float3 (translation.x, translation.y, translation.z);
				scale_i = float3 (scaling.x, scaling.y, scaling.z);
				rotation_i = Quat (rotation.x, rotation.y, rotation.z, rotation.w);
			}

			object->transform->SetPosition(float3(position_i.x, position_i.y, position_i.z));
			object->transform->SetRotationQuat(Quat(rotation_i.x, rotation_i.y, rotation_i.w, rotation_i.z));
			//go->transform->SetScale(float3(scaling.x, scaling.y, scaling.z));

			if (model != nullptr)
			{
				LoadVertices(model, mesh);

				if (mesh->HasFaces())
				{
					LoadIndexs(model, mesh);
				}

				if (mesh->HasNormals())
				{
					LoadNormals(model, mesh);
				}

				model->num_uvs_channels = mesh->GetNumUVChannels();

				LoadUVs(model, mesh);

				model->SetBuffersWithData();

				if (model->num_uvs_channels > 0 && scene->HasMaterials())
				{

					ComponentTexture* text = (ComponentTexture*)object->AddComponent(ComponentType::TEXTURE);
					model->num_uvs_channels = mesh->GetNumUVChannels();


					LoadMaterials(scene, mesh, text);

					text->ActivateThisTexture();
				}

				
			}
			else
			{
				LOG("The component Mesh was not created correctly, it is possible that such a component already exists in this game objects. Only is posible to have 1 component mesh by Game Object.");
			}
			
			AABB aabb;

			aabb.SetNegativeInfinity();

			aabb.Enclose((float3*)mesh->mVertices, mesh->mNumVertices);

			//aabb.minPoint.x = mesh->mAABB.mMin.x;
			//aabb.minPoint.y = mesh->mAABB.mMin.y;
			//aabb.minPoint.z = mesh->mAABB.mMin.z;

			//aabb.maxPoint.x = mesh->mAABB.mMax.x;
			//aabb.maxPoint.y = mesh->mAABB.mMax.y;
			//aabb.maxPoint.z = mesh->mAABB.mMax.z; 

			model->mesh_data->aabb = aabb;

			// Generate global OBB
			OBB obb = aabb;
			obb.Transform(object->transform->GetGlobalTransform());
			// Generate global AABB
			aabb.SetNegativeInfinity();
			aabb.Enclose(obb);

			//App->game_object_manager->AddObject(object);
		}
		
	}

	//if only load one mesh, add this to he hierarchy.
	if (root_go->childrens.size() == 1)
	{
		root_go->childrens[0]->parent = App->game_object_manager->root;
		App->game_object_manager->root->childrens.push_back(root_go->childrens[0]);
		

		//once we loaded the mesh to the hierarchy, delete the Game Object "group" because we will not use it in this case.
		root_go->childrens.erase(root_go->childrens.cbegin()); // don't delete the children, only the group. The function deletes the childrens too. For this reason erase the game object from the children list.
		App->game_object_manager->Destroy(root_go);
	}
	else //if we load more than one mesh, add the group with the meshes loaded as childrens.
	{
		root_go->parent = App->game_object_manager->root;
		App->game_object_manager->root->childrens.push_back(root_go);
	}

	go_created.clear();
	go_to_create.clear();

}

// ----------------------------Vertexs----------------------------

void ModuleImporter::LoadVertices(ComponentMesh * model, aiMesh * mesh)
{
	model->mesh_data->vertexs.has_data = true;

	model->mesh_data->vertexs.num = mesh->mNumVertices; // get number of Vertices

	model->mesh_data->vertexs.buffer_size = model->mesh_data->vertexs.num * 3;
	model->mesh_data->vertexs.buffer = new float[model->mesh_data->vertexs.buffer_size]; // create array of Vertices with the correct size

	memcpy(model->mesh_data->vertexs.buffer, mesh->mVertices, sizeof(float) * model->mesh_data->vertexs.buffer_size); // copy the vertices of the mesh to the arrey of vertices

	LOG("New mesh with %i vertices.", model->mesh_data->vertexs.num);
}

// ----------------------------Indexs----------------------------

void ModuleImporter::LoadIndexs(ComponentMesh * model, aiMesh * mesh)
{
	model->mesh_data->indexs.has_data = true;

	model->num_faces = mesh->mNumFaces;
	model->mesh_data->indexs.num = model->num_faces * 3; // get number of indices. Every face has 3 indices, assuming each face is a triangle

	model->mesh_data->indexs.buffer_size = model->mesh_data->indexs.num; // don't have coordinates, so the number of indexs == buffer_size.
	model->mesh_data->indexs.buffer = new uint[model->mesh_data->indexs.buffer_size]; // create array of indices with the correct size

	for (uint i = 0; i < mesh->mNumFaces; ++i)
	{
		if (mesh->mFaces[i].mNumIndices != 3) // if the face is not a triangle don't load it.
		{
			LOG("This face don't have 3 index, only can load faces with 3 indexs.");
			memset(&model->mesh_data->indexs.buffer[i * 3], 0, sizeof(uint) * 3);
		}
		else
		{
			// Every face have 3 indices. 
			// take the first 3 slots, 
			//then the next 3 slots, 
			//then the same ...                                               3 indices * their var type, only copy 1 face (3 indices) every time
			memcpy(&model->mesh_data->indexs.buffer[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint)); // Copy the Indices of the mesh to the array of indices.
		}
	}

	LOG("New mesh with %i faces and %i indexs.", model->num_faces, model->mesh_data->indexs.num);
}

// ----------------------------Normals----------------------------

void ModuleImporter::LoadNormals(ComponentMesh * model, aiMesh * mesh)
{
	//load normals direction of the vertex_normals.
	model->mesh_data->normals_direction.has_data = true;
	model->mesh_data->normal_vertexs.has_data = true;
	model->mesh_data->normal_faces.has_data = true;

	model->mesh_data->normals_direction.num = model->mesh_data->vertexs.num;
	model->mesh_data->normal_vertexs.num = model->mesh_data->vertexs.num;
	model->mesh_data->normal_faces.num = model->num_faces;

	model->mesh_data->normals_direction.buffer_size = model->mesh_data->normal_vertexs.num * 3/*every vertex_normal have 3 coordinates (x, y, z).*/;
	model->mesh_data->normals_direction.buffer = new float[model->mesh_data->normals_direction.buffer_size];
	memcpy(model->mesh_data->normals_direction.buffer, mesh->mNormals, sizeof(float) * model->mesh_data->normals_direction.buffer_size); //It could be QNaN?

	LOG("Normals loaded correcly.");

	model->CalculateNormals();
}

 //----------------------------UVs----------------------------

void ModuleImporter::LoadUVs(ComponentMesh * model, aiMesh * mesh)
{
	model->mesh_data->uvs.has_data = true;

	model->mesh_data->uvs.num = mesh->mNumVertices; //every vertex have one vector (only 2 dimensions will considerate) of uvs.

	model->mesh_data->uvs.buffer_size = model->num_uvs_channels * model->mesh_data->uvs.num * 2/*only save 2 coordinates, the 3rt coordinate will be always 0, so don't save it*/; // number of uvs * number of components of the vector (2) * number of channels of the mesh
	model->mesh_data->uvs.buffer = new float[model->mesh_data->uvs.buffer_size];

	model->channel_buffer_size = model->mesh_data->uvs.num * 2;//the same as uvs_buffer_size without the multiplication by the number of channels because we want to save only the size of 1 channel.
	for (uint channel = 0; channel < model->num_uvs_channels; ++channel)
	{
		if (mesh->HasTextureCoords(channel)) // if this channel have texture coords...
		{

			if (mesh->mNumUVComponents[channel] == 2) //the channel have vectors of 2 components
			{
				for (uint j = 0; j < model->mesh_data->uvs.num; ++j)
				{								//start index of the current channel.  start index of the current channel of the mesh.  Only copy the values in 1 channel size.
					memcpy(&model->mesh_data->uvs.buffer[(channel * model->channel_buffer_size) + j*2], &mesh->mTextureCoords[channel][j], sizeof(float) * 2);

				}
			}
			else // if the channel don't have 2 components by vector, don't save it and fill it with 0.
			{
				LOG("This channel of the UVs don't have 2 components by vector.");
				memset(&model->mesh_data->uvs.buffer[channel * model->channel_buffer_size], 0, sizeof(float) * model->channel_buffer_size);
			}
		}
	}
	LOG("UVs loaded correcly.")
}

 //----------------------------Materials----------------------------

void ModuleImporter::LoadMaterials(const aiScene * scene, aiMesh * mesh, ComponentTexture * model)
{
	
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE); //only load DIFFUSE textures.

	if (numTextures > 0)
	{
		model->has_texture = true;
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		std::string name = App->GetFileName(path.C_Str());
		std::string dir = "Models/Textures/";
		std::string f_path = dir + name;
		model->id_texture = App->texture->LoadTexture(f_path.c_str(), model->texture_width, model->texture_height );
		model->texture_path = f_path.c_str();
	}
}