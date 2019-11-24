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
#include "ModuleFileSystem.h"
#include "ModuleQuadtree.h"

#include "ResourceManagment.h"
#include "ResourceMesh.h"


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

const char* ModuleImporter::GetPath() const
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

	const std::vector<ResourceMesh*> meshes = App->resource_managment->GetAllMeshes();
	std::vector<ResourceMesh*> meshes_of_this_model_in_memory;

	for (std::vector<ResourceMesh*>::const_iterator citer = meshes.cbegin(); citer != meshes.cend(); ++citer)
	{
		if ((*citer)->imported_file == App->GetFileNameWithoutExtension(path))
			meshes_of_this_model_in_memory.push_back(*citer);
	}

	if (!meshes_of_this_model_in_memory.empty())
	{
		GameObject* root_go = nullptr;

		if (meshes_of_this_model_in_memory.size() > 1)
			root_go = new GameObject(std::string(std::string("group_") + App->GetFileNameWithoutExtension(path)), App->game_object_manager->root);
		else
			root_go = App->game_object_manager->root;

		for (std::vector<ResourceMesh*>::const_iterator citer = meshes_of_this_model_in_memory.cbegin(); citer != meshes_of_this_model_in_memory.cend(); ++citer)
		{
			GameObject* object = new GameObject((*citer)->name.c_str(), root_go);//HIERARCHY NOT IMPLEMENTED!
			ComponentMesh* model = (ComponentMesh*)object->AddComponent(ComponentType::MESH);
			model->SetResourceID((*citer)->GetResourceID());
			if ((*citer)->texture_binded_id != 0)
			{
				ComponentTexture* text = (ComponentTexture*)object->AddComponent(ComponentType::TEXTURE);
				text->has_texture = true;
				text->SetResourceID((*citer)->texture_binded_id);
				text->ActivateThisTexture();
			}

			object->local_bbox = model->GetResource()->GetBbox();
			App->quadtree->Insert(object);
			LOG("Returning mesh in memory");
		}
	}
	else
	{

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
	}

	return ret;
}

void ModuleImporter::LoadAllMeshes(const aiScene * scene)
{

	std::list<Node<aiNode>> go_to_create;
	std::list<Node<aiNode>> go_created;
	std::string n = "group_"; n += App->GetFileNameWithoutExtension(GetPath()); n += "_"; n += std::to_string(App->game_object_manager->GetAllGameObjectNumber());
	GameObject* root_go = new GameObject(n, nullptr);

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	if (scene->mRootNode != nullptr)
	{
		scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);
		float3 pos(translation.x, translation.y, translation.z);
		float3 scale(scaling.x, scaling.y, scaling.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	}

	root_go->GetTransform()->SetPosition(float3(translation.x, translation.y, translation.z));
	root_go->GetTransform()->SetRotationQuat(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
	
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
			string name = current->current_node->mName.C_Str();

			// ignore aiNodes with no game object, all transformation, rotations...
			Node<aiNode>* parent = current->parent;
			while (parent->current_go == nullptr)
			{
				parent = parent->parent;
			}


			//create gameObject.
			
			GameObject* object = new GameObject(name.c_str(), parent->current_go);

			ComponentMesh* model = (ComponentMesh*)object->AddComponent(ComponentType::MESH);
			if (App->resource_managment->GetByNameMesh(name.c_str()) != 0)
				model->SetResourceID(App->resource_managment->GetByNameMesh(name.c_str())); //ove


			ResourceMesh* res_mesh = nullptr;


			// Set mesh pos, rot and scale
			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;


			if (current->current_node != nullptr)
			{
				current->current_node->mTransformation.Decompose(scaling, rotation, translation);
				float3 pos(translation.x, translation.y, translation.z);
				float3 scale(scaling.x, scaling.y, scaling.z);
				Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
			}

			object->GetTransform()->SetPosition(float3(translation.x, translation.y, translation.z));
			object->GetTransform()->SetRotationQuat(Quat(rotation.x, rotation.y, rotation.w, rotation.z));

			
				aiMesh* mesh = scene->mMeshes[current->current_node->mMeshes[i]];
				if (App->resource_managment->GetByNameMesh(name.c_str()) == 0 && App->file_system->Exists(std::string(LIBRARY_MESH_FOLDER + name + ".mesh").c_str()))
				{
					LOG("Loading file with own format");
					LoadOwnFile(name + ".mesh", model);


				}
				else
				{
					if (model->GetResource() == nullptr)
						model->SetResourceID(App->resource_managment->CreateNewResource(Resource::Type::MESH));

					res_mesh = model->GetResource();
					res_mesh->imported_file = App->GetFileNameWithoutExtension(GetPath());
					res_mesh->name = "";
					if (model != nullptr)
					{
						LoadVertices(res_mesh, mesh);

						if (mesh->HasFaces())
						{
							LoadIndexs(res_mesh, mesh);
						}

						if (mesh->HasNormals())
						{
							LoadNormals(res_mesh, mesh);
						}

						res_mesh->num_uvs_channels = mesh->GetNumUVChannels();

						LoadUVs(res_mesh, mesh);

						res_mesh->SetBuffersWithData();

						CreateOwnFile(res_mesh, name);
						//LoadOwnFile(string(name + ".mesh"),);

					}
					else
					{
						LOG("The component Mesh was not created correctly, it is possible that such a component already exists in this game objects. Only is posible to have 1 component mesh by Game Object.");
					}
				}

				if (scene->HasMaterials())
				{
					res_mesh = model->GetResource();

					ComponentTexture* text = (ComponentTexture*)object->AddComponent(ComponentType::TEXTURE);
					res_mesh->num_uvs_channels = mesh->GetNumUVChannels();


					LoadMaterials(scene, mesh, text);

					res_mesh->texture_binded_id = text->GetResourceID();


				}
				res_mesh->name = name;

				//AABB
				AABB aabb;
				aabb.SetNegativeInfinity();
				aabb.Enclose((float3*)res_mesh->mesh_data->vertexs.buffer, res_mesh->mesh_data->vertexs.num);
				res_mesh->mesh_data->aabb = aabb;
				object->AddAABB(aabb);

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

void ModuleImporter::LoadVertices(ResourceMesh * model, aiMesh * mesh)
{
	model->mesh_data->vertexs.has_data = true;

	model->mesh_data->vertexs.num = mesh->mNumVertices; // get number of Vertices

	model->mesh_data->vertexs.buffer_size = model->mesh_data->vertexs.num * 3;
	model->mesh_data->vertexs.buffer = new float[model->mesh_data->vertexs.buffer_size]; // create array of Vertices with the correct size

	memcpy(model->mesh_data->vertexs.buffer, mesh->mVertices, sizeof(float) * model->mesh_data->vertexs.buffer_size); // copy the vertices of the mesh to the arrey of vertices

	LOG("New mesh with %i vertices.", model->mesh_data->vertexs.num);
}

// ----------------------------Indexs----------------------------

void ModuleImporter::LoadIndexs(ResourceMesh * model, aiMesh * mesh)
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

void ModuleImporter::LoadNormals(ResourceMesh * model, aiMesh * mesh)
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

void ModuleImporter::LoadUVs(ResourceMesh * model, aiMesh * mesh)
{
	model->mesh_data->uvs.has_data = true;

	model->mesh_data->uvs.num = mesh->mNumVertices; //every vertex have one vector (only 2 dimensions will considerate) of uvs.

	model->mesh_data->uvs.buffer_size = model->num_uvs_channels * model->mesh_data->uvs.num * 2/*only save 2 coordinates, the 3rt coordinate will be always 0, so don't save it*/; // number of uvs * number of components of the vector (2) * number of channels of the mesh
	model->mesh_data->uvs.buffer = new float[model->mesh_data->uvs.buffer_size];

	uint channel_buffer_size = model->mesh_data->uvs.num * 2;//the same as uvs_buffer_size without the multiplication by the number of channels because we want to save only the size of 1 channel.
	for (uint channel = 0; channel < model->num_uvs_channels; ++channel)
	{
		if (mesh->HasTextureCoords(channel)) // if this channel have texture coords...
		{

			if (mesh->mNumUVComponents[channel] == 2) //the channel have vectors of 2 components
			{
				for (uint j = 0; j < model->mesh_data->uvs.num; ++j)
				{								//start index of the current channel.  start index of the current channel of the mesh.  Only copy the values in 1 channel size.
					memcpy(&model->mesh_data->uvs.buffer[(channel * channel_buffer_size) + j*2], &mesh->mTextureCoords[channel][j], sizeof(float) * 2);

				}
			}
			else // if the channel don't have 2 components by vector, don't save it and fill it with 0.
			{
				LOG("This channel of the UVs don't have 2 components by vector.");
				memset(&model->mesh_data->uvs.buffer[channel * channel_buffer_size], 0, sizeof(float) * channel_buffer_size);
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
		App->texture->LoadTexture(f_path.c_str(), model);
	}
}

void ModuleImporter::CreateOwnFile(const ResourceMesh* mesh, const string name_to_file)
{

	uint size_vertexs = sizeof(float) * mesh->mesh_data->vertexs.buffer_size;
	uint size_indexs = sizeof(uint)  * mesh->mesh_data->indexs.buffer_size;
	uint size_normals_dir = sizeof(float) * mesh->mesh_data->normals_direction.buffer_size;
	uint size_normal_vertexs = sizeof(float) * mesh->mesh_data->normal_vertexs.buffer_size;
	uint size_normal_faces = sizeof(float) * mesh->mesh_data->normal_faces.buffer_size;
	uint size_uvs = sizeof(float) * mesh->mesh_data->uvs.buffer_size;
	


	const char* file_name = mesh->name.c_str();

	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint header[13] = {
		strlen(file_name),

		mesh->mesh_data->vertexs.num,
		size_vertexs,

		mesh->mesh_data->indexs.num,
		size_indexs,

		mesh->mesh_data->normals_direction.num,
		size_normals_dir,

		mesh->mesh_data->normal_vertexs.num,
		size_normal_vertexs,

		mesh->mesh_data->normal_faces.num,
		size_normal_faces,

		mesh->mesh_data->uvs.num,
		size_uvs

	};



	//TODO AABB??

	// size----------------
	// Set the size of the entire file size
	uint all_size = sizeof(header) +
		strlen(file_name) +
		size_vertexs +
		size_indexs +
		size_normals_dir +
		size_normal_vertexs +
		size_normal_faces +
		size_uvs;


	char* data = new char[all_size]; // Allocate the entire file size.
	char* cursor = data;


	// Save the buffers----------
	// First store header
	uint size = sizeof(header); 
	memcpy(cursor, header, size);

	cursor += size;
	size = strlen(file_name);
	memcpy(cursor, file_name, size);

	// Store vertexs
	cursor += size;
	size = size_vertexs;
	memcpy(cursor, mesh->mesh_data->vertexs.buffer, size);

	// Store indices
	cursor += size; 
	size = size_indexs;
	memcpy(cursor, mesh->mesh_data->indexs.buffer, size);

	// Store normals_dir
	cursor += size;
	size = size_normals_dir;
	memcpy(cursor, mesh->mesh_data->normals_direction.buffer, size);

	// Store normal_vertexs
	cursor += size;
	size = size_normal_vertexs;

	memcpy(cursor, mesh->mesh_data->normal_vertexs.buffer, size);
	// Store normal_faces
	cursor += size;
	size = size_normal_faces;
	memcpy(cursor, mesh->mesh_data->normal_faces.buffer, size);

	// Store uvs
	cursor += size;
	size = size_uvs;
	memcpy(cursor, mesh->mesh_data->uvs.buffer, size);

	string path_to_save(LIBRARY_MESH_FOLDER + string(name_to_file) + string(".mesh"));
	if (App->file_system->Exists(path_to_save.c_str()))
	{
		/*string s = App->file_system->GetWritePath();
		App->file_system->SetWritePath("");*/
		App->file_system->Remove(path_to_save.c_str());
		//App->file_system->SetWritePath(s.c_str());

	}
	
		App->file_system->Save(path_to_save.c_str(), data, all_size);
	RELEASE_ARRAY(data);

}

void ModuleImporter::LoadOwnFile(string name_file, ComponentMesh* mesh)
{
	//relative_path with extension of the own format.


	string full_path(LIBRARY_MESH_FOLDER + name_file);

	char* data;

	App->file_system->Load(full_path.c_str(), &data);

	char* cursor = data;



	//Load Header----------
	uint ranges[13];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	cursor += bytes;
	bytes = ranges[0];
	char* file_name = new char[ranges[0]];
	memcpy(file_name, cursor, bytes);
	std::string name = App->GetStringByLength(file_name, ranges[0]);

	const std::vector<ResourceMesh*> meshes = App->resource_managment->GetAllMeshes();
	std::vector<ResourceMesh*> meshes_of_this_model_in_memory;

	for (std::vector<ResourceMesh*>::const_iterator citer = meshes.cbegin(); citer != meshes.cend(); ++citer)
	{
		if ((*citer)->name == name)
		{
			mesh->SetResourceID((*citer)->GetResourceID());
			LOG("The model to load was in memory... Returning the model in memory.");
			RELEASE_ARRAY(data);
			return;
		}
	}

	mesh->SetResourceID(App->resource_managment->CreateNewResource(Resource::Type::MESH));
	ResourceMesh* res_mesh = mesh->GetResource();
	


	res_mesh->mesh_data->vertexs.num = ranges[1];
	res_mesh->mesh_data->vertexs.buffer_size = ranges[1] * 3;
	if (res_mesh->mesh_data->vertexs.num > 0)
		res_mesh->mesh_data->vertexs.has_data = true;

	res_mesh->mesh_data->indexs.num = ranges[3];
	res_mesh->mesh_data->indexs.buffer_size = ranges[3];
	if (res_mesh->mesh_data->indexs.num > 0)
		res_mesh->mesh_data->indexs.has_data = true;

	res_mesh->mesh_data->normals_direction.num = ranges[5];
	res_mesh->mesh_data->normals_direction.buffer_size = ranges[5]*3;
	if (res_mesh->mesh_data->normals_direction.num > 0)
		res_mesh->mesh_data->normals_direction.has_data = true;

	res_mesh->mesh_data->normal_vertexs.num = ranges[7];
	res_mesh->mesh_data->normal_vertexs.buffer_size = ranges[7]*3;
	if (res_mesh->mesh_data->normal_vertexs.num > 0)
		res_mesh->mesh_data->normal_vertexs.has_data = true;

	res_mesh->mesh_data->normal_faces.num = ranges[9];
	res_mesh->mesh_data->normal_faces.buffer_size = ranges[9]*3;
	if (res_mesh->mesh_data->normal_faces.num > 0)
		res_mesh->mesh_data->normal_faces.has_data = true;

	res_mesh->mesh_data->uvs.num = ranges[11];
	res_mesh->mesh_data->uvs.buffer_size = ranges[11]*2;
	if (res_mesh->mesh_data->uvs.num > 0)
		res_mesh->mesh_data->uvs.has_data = true;


	//Load Buffers----------
	// Load vertexs
	cursor += bytes;
	bytes = sizeof(float) * res_mesh->mesh_data->vertexs.buffer_size;
	res_mesh->mesh_data->vertexs.buffer = new float[res_mesh->mesh_data->vertexs.buffer_size];
	memcpy(res_mesh->mesh_data->vertexs.buffer, cursor, bytes);

	// Load indexs
	cursor += bytes;
	bytes = sizeof(uint) * res_mesh->mesh_data->indexs.buffer_size;
	res_mesh->mesh_data->indexs.buffer = new uint[res_mesh->mesh_data->indexs.buffer_size];
	memcpy(res_mesh->mesh_data->indexs.buffer, cursor, bytes);

	// Load normal_dir
	cursor += bytes;
	bytes = sizeof(float) * res_mesh->mesh_data->normals_direction.buffer_size;
	res_mesh->mesh_data->normals_direction.buffer = new float[res_mesh->mesh_data->normals_direction.buffer_size];
	memcpy(res_mesh->mesh_data->normals_direction.buffer, cursor, bytes);

	// Load normal_vertexs
	cursor += bytes;
	bytes = sizeof(float) * res_mesh->mesh_data->normal_vertexs.buffer_size;
	res_mesh->mesh_data->normal_vertexs.buffer = new float[res_mesh->mesh_data->normal_vertexs.buffer_size];
	memcpy(res_mesh->mesh_data->normal_vertexs.buffer, cursor, bytes);

	// Load normal_faces
	cursor += bytes;
	bytes = sizeof(float) * res_mesh->mesh_data->normal_faces.buffer_size;
	res_mesh->mesh_data->normal_faces.buffer = new float[res_mesh->mesh_data->normal_faces.buffer_size];
	memcpy(res_mesh->mesh_data->normal_faces.buffer, cursor, bytes);

	// Load uvs
	cursor += bytes;
	bytes = sizeof(float) * res_mesh->mesh_data->uvs.buffer_size;
	res_mesh->mesh_data->uvs.buffer = new float[res_mesh->mesh_data->uvs.buffer_size];
	memcpy(res_mesh->mesh_data->uvs.buffer, cursor, bytes);

	res_mesh->SetBuffersWithData();

	RELEASE_ARRAY(data);

}

