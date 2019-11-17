#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "App.h"
#include "ModuleGameObjectManager.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include <list>

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{
	LOG("New Game Object created!");
	if (parent != nullptr)
	{
		parent->childrens.push_back(this);
	}
	transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
}

void GameObject::PreUpdate()
{
	if (IsActive())
	{
		isInsideFrustum = false;
	}
}

void GameObject::Update()
{
	if (IsActive())
	{
		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter)->IsActive())
			{
				(*iter)->Update();//RenderMesh and texture in Update or PostUpdate??
			}
		}
	}
}

void GameObject::PostUpdate()
{
	if (IsActive())
	{
		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter)->IsActive())
			{
				(*iter)->PostUpdate();//Render
			}
		}
	}
}

void GameObject::CleanUp()
{

	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		(*iter)->CleanUp();
		RELEASE(*iter);
		
	}
	components.clear();

	//RELEASE(this);
}

Component* GameObject::AddComponent(ComponentType type)
{
	Component* ret = nullptr;

	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == type && type != ComponentType::TEXTURE)
		{
			LOG("This game object already has a component of this type. Create another game object and add this component!");
			return ret;
		}
	}

	switch (type)
	{
	case ComponentType::NONE:
		LOG("Component empty. Not accepted.");
		break;
	case ComponentType::TRANSFORM:
		ret = new ComponentTransform();
		ret->type = type;
		AddToComponentList(ret);
		LOG("Component Transform added correctly.");
		break;
	case ComponentType::MESH:
		ret = new ComponentMesh();
		ret->type = type;
		AddToComponentList(ret);
		LOG("Component Mesh added correctly.")
		break;
	case ComponentType::TEXTURE:
		ret = new ComponentTexture();
		ret->type = type;
		AddToComponentList(ret);
		LOG("Component Texture added correcly.");
		break;
	case ComponentType::CAMERA:
		ret = new ComponentCamera();
		ret->type = type;
		AddToComponentList(ret);
		LOG("Component Camera added correcly.");
		break;
	default:
		LOG("Component not found in the function. Not accepted.");
		break;
	}

	return ret;
}

void GameObject::AddToComponentList(Component * &ret)
{
	ret->object = this;
	components.push_back(ret);
}

void GameObject::SetSelected(const bool& set)
{
	selected = set;
}

void GameObject::SetName(const std::string& set)
{
	name = set;
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

const bool GameObject::GetSelected() const
{
	return selected;
}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(const bool & to_active)
{
	if (to_active)
	{
		DoForAllChildrens(&GameObject::SetActiveTrue);
	}
	else
	{
		DoForAllChildrens(&GameObject::SetActiveFalse);
	}
}

void GameObject::SetActiveFalse()
{
	active = false;
}

void GameObject::SetActiveTrue()
{
	active = true;
}

ComponentTexture* GameObject::GetTextureActivated() const
{
	std::vector<ComponentTexture*> textures = GetTextures();
	for (std::vector<ComponentTexture*>::const_iterator iter = textures.begin(); iter != textures.end(); ++iter)
	{
		if ((*iter)->IsTextureActive())
		{
			return (*iter);
		}
	}
	LOG("There is no texture activated.");
	return nullptr;
}

std::vector<ComponentTexture*> GameObject::GetTextures() const
{
	std::vector<ComponentTexture*> textures;
	for (std::vector<Component*>::const_iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == ComponentType::TEXTURE)
		{
			ComponentTexture* component = (ComponentTexture*)(*iter);
			textures.push_back(component);
		}
	}
	return textures;
}

ComponentMesh* GameObject::GetMesh() const
{
	for (std::vector<Component*>::const_iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == ComponentType::MESH)
		{
			return (ComponentMesh*)(*iter);
		}
	}

	return nullptr;
}

ComponentCamera* GameObject::GetCam() const
{
	for (std::vector<Component*>::const_iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->type == ComponentType::CAMERA)
		{
			return (ComponentCamera*)(*iter);
		}
	}

	return nullptr;
}


int GameObject::DoForAllChildrens(std::function<void(GameObject*)> funct)
{
	int number_childrens = -1; // -1 to not count this game object and only his childrens.
	std::list<GameObject*> all_childrens;

	all_childrens.push_back(this);

	while (!all_childrens.empty())
	{
		GameObject* current = (*all_childrens.begin());
		all_childrens.pop_front();
		if (current != nullptr)
		{
			for (std::vector<GameObject*>::const_iterator iter = current->childrens.cbegin(); iter != current->childrens.cend(); ++iter)
			{
				all_childrens.push_back(*iter);
			}

			funct(current);
			++number_childrens;
		}
	}

	return number_childrens;
}

int GameObject::DoForAllSelected(std::function<bool(GameObject* /*from*/, GameObject*/*target*/ )> funct)
{
	int number_of_selected = -1; // -1 to not count this game object and only his childrens.
	bool ret = true;
		for (std::vector<GameObject*>::const_iterator iter = App->game_object_manager->selected.cbegin(); iter != App->game_object_manager->selected.cend() && ret; ++iter)
		{
			ret = funct(this,*iter);
			++number_of_selected;
		}

	return number_of_selected;
}

void GameObject::SelectThis()
{
	App->game_object_manager->selected.push_back(this);
	SetSelected(true);
}



bool GameObject::IsMyBrother(GameObject* object) const
{

	for (std::vector<GameObject*>::const_iterator iter = parent->childrens.cbegin(); iter != parent->childrens.cend(); ++iter)
	{
		if (object == (*iter))
		{
			return true;
		}
	}

	return false;
}

bool GameObject::HasChildrens() const
{
	return childrens.empty();
}


bool GameObject::SetAsNewChildren(GameObject* new_children)
{
	if (std::find(childrens.begin(), childrens.end(), new_children) == childrens.end() && !IsParentOfMyParents(new_children)) //if it isn't a children of this GameObject and is not a parent o parents of this game object.
	{
		new_children->parent->childrens.erase(std::find(new_children->parent->childrens.begin(), new_children->parent->childrens.end(), new_children));
		new_children->parent = this;
		childrens.push_back(new_children);
		return true;
	}
	else
		return false;
}

bool GameObject::IsParentOfMyParents( GameObject* possible_parent)
{
	if (parent == nullptr)
		return false;

	GameObject* current_go = parent;
	while (current_go->parent != nullptr)
	{
		if (possible_parent == current_go)
			return true;

		current_go = current_go->parent;
	}
	return false;
}

void GameObject::SetGoSelectedAsChildrenFromThis()
{
	DoForAllSelected(&GameObject::SetAsNewChildren);
}

void GameObject::CalcGlobalTransform()
{
	float4x4 global;
	if (parent != nullptr)
	{
		global = parent->transform->GetGlobalTransform() * transform->GetLocalTransform();
		transform->SetGlobalTransform(global);
	}
}

void GameObject::CalcBBox()
{
	local_bbox.SetNegativeInfinity();

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
		(*it)->OnGetBoundingBox(local_bbox);

	if (local_bbox.IsFinite())
		local_bbox.Transform(transform->GetGlobalTransform());
}

bool PointInRect(float2 point_xy, Rect rect_xywh)
{
	if (point_xy.x >= rect_xywh.left && point_xy.x <= rect_xywh.right && point_xy.y > rect_xywh.top && point_xy.y < rect_xywh.bottom)
		return true;

	return false;
}

void GameObject::TestRay()
{
	// Check if intersects with bbox
	if (local_bbox.IsFinite())
	{
		Rect rect = App->window->GetWindowRect();
		float2 mouse_pos = App->input->GetMouse();

		if (PointInRect(mouse_pos, rect))
		{
			// The point (1, 1) corresponds to the top-right corner of the near plane
			// (-1, -1) is bottom-left

			float first_normalized_x = (mouse_pos.x - rect.left) / (rect.right - rect.left);
			float first_normalized_y = (mouse_pos.y - rect.top) / (rect.bottom - rect.top);

			float normalized_x = (first_normalized_x * 2) - 1;
			float normalized_y = 1 - (first_normalized_y * 2);

			LineSegment picking = App->camera->GetCurrentCamera()->GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);

			float distance1 = 99999999999;

			if (picking.Intersects(local_bbox))
			{
				// Get mesh
				ComponentMesh* cmesh = GetMesh();

				if (cmesh != nullptr)
				{
					// Transform segment to match mesh transform
					LineSegment segment_local_space(picking);
					segment_local_space.Transform(transform->GetGlobalTransform().Inverted());

					// Check every triangle
					Triangle tri;
					uint* indices = cmesh->mesh_data->indexs.buffer;
					float* vertices = cmesh->mesh_data->vertexs.buffer;
					for (int i = 0; i < cmesh->mesh_data->indexs.num;)
					{
						tri.a.Set(vertices[(indices[i])], vertices[(indices[i] + 1)], vertices[(indices[i] + 2)]); ++i;
						tri.b.Set(vertices[(indices[i])], vertices[(indices[i] + 1)], vertices[(indices[i] + 2)]); ++i;
						tri.c.Set(vertices[(indices[i])], vertices[(indices[i] + 1)], vertices[(indices[i] + 2)]); ++i;

						float distance;
						float3 hit_point;
						if (segment_local_space.Intersects(tri, &distance, &hit_point))
						{
							if (distance < distance1)
							{
								distance1 = distance;
								App->game_object_manager->closest = this;
							}
						}
					}
				}
			}
		}
	}
}