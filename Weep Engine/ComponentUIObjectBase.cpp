#include "App.h"
#include "ModuleInput.h"
//#include "MGui.h"
#include "ComponentUIObjectBase.h"
#include "GameObject.h"
#include "ComponentRender2D.h"
#include "ComponentTexture.h"
#include"ResourceTexture.h"


ComponentUIObjectBase::ComponentUIObjectBase(float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent) : local_pos(local_pos), draggable(draggable), parent(parent)
{
	if (parent != nullptr)
	{
		world_pos = local_pos + parent->world_pos;
		parent->childrens.push_back(this);
	}

	else
		world_pos = local_pos;

	rect_world.left = world_pos.x;
	rect_world.top = world_pos.y;
	rect_world.right = rect_world.left + rect_spritesheet_original.Width();
	rect_world.bottom = rect_world.top + rect_spritesheet_original.Height();

	glGenBuffers(1, &vertexs_quad.id_buffer);

	vertexs_quad.buffer = new float[8];
	SetBufferVertexPos();

	uv_quad.buffer = new float[8];
	//vertice 1
	uv_quad.buffer[0] = 0;
	uv_quad.buffer[1] = 1;

	//vertice 2
	uv_quad.buffer[2] = 0;
	uv_quad.buffer[3] = 0;

	//vertice 3
	uv_quad.buffer[4] = 1;
	uv_quad.buffer[5] = 0;

	//vertice 4
	uv_quad.buffer[6] = 1;
	uv_quad.buffer[7] = 1;

	glGenBuffers(1, &uv_quad.id_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_quad.id_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * /*vertexs.buffer_size*/ 4 * 2, uv_quad.buffer, GL_STATIC_DRAW);

}
void ComponentUIObjectBase::SetBufferVertexPos()
{
	//vertice 1
	vertexs_quad.buffer[0] = rect_world.left;
	vertexs_quad.buffer[1] = rect_world.top;

	//vertice 2
	vertexs_quad.buffer[2] = rect_world.left;
	vertexs_quad.buffer[3] = rect_world.bottom;

	//vertice 3
	vertexs_quad.buffer[4] = rect_world.right;
	vertexs_quad.buffer[5] = rect_world.bottom;

	//vertice 4
	vertexs_quad.buffer[6] = rect_world.right;
	vertexs_quad.buffer[7] = rect_world.top;

	glBindBuffer(GL_ARRAY_BUFFER, vertexs_quad.id_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * /*vertexs.buffer_size*/ 4 * 2, vertexs_quad.buffer, GL_STATIC_DRAW);


}
void ComponentUIObjectBase::PreUpdate()
{

	return;
}
void ComponentUIObjectBase::PostUpdate()
{
	if (GetVisible())
		object->GetRender2D()->Render(vertexs_quad, uv_quad, object->GetTextureActivated()->GetResource(object->GetTextureActivated()->GetResourceID())->id_texture);// App->render->Blit(atlas, world_pos_final.x, world_pos_final.y, &rect_spritesheet_final, 1.0f, SDL_FLIP_NONE, false);

	return;
}

void ComponentUIObjectBase::CleanUp()
{
	parent = nullptr;
	return;
}

void ComponentUIObjectBase::Update()
{
	if (draggable)
	{
		if (!dragging && MouseInRect() && App->input->GetMouseButton(1) == KEY_DOWN)
		{
			dragging = true;
		}

		if (dragging)
		{
			float2 mouse_move;
			mouse_move.x = App->input->GetMouseXMotion();
			mouse_move.y = App->input->GetMouseYMotion();


			if (!childrens.empty())
			{
				list<ComponentUIObjectBase*> all_childrens;
				all_childrens.push_back(this);

				while (!all_childrens.empty())
				{
					list<ComponentUIObjectBase*>::iterator current_ui_object = all_childrens.begin();

					for (list<ComponentUIObjectBase*>::iterator item = (*current_ui_object)->childrens.begin(); item != (*current_ui_object)->childrens.end(); ++item)
					{
						all_childrens.push_back(*item);
					}

					(*current_ui_object)->SetPos(mouse_move);
					all_childrens.remove(*current_ui_object);
				}
			}
			else
				SetPos(mouse_move);




			if (App->input->GetMouseButton(1) == KEY_UP)
			{
				dragging = false;
			}
		}

		if (parent)
		{
			/*if (world_pos_original.x < parent->world_pos_original.x)
			{
				rect_world.left = rect_world.left + (parent->world_pos_original.x - world_pos_original.x);
				rect_spritesheet_final.w = rect_spritesheet_original.w - (parent->world_pos_original.x - world_pos_original.x);
				world_pos_final.x = world_pos_original.x + (parent->world_pos_original.x - world_pos_original.x);
			}
			else if (world_pos_original.x + rect_spritesheet_original.w > parent->world_pos_original.x + parent->rect_spritesheet_final.w)
			{
				rect_spritesheet_final.w = rect_spritesheet_original.w - (world_pos_original.x + rect_spritesheet_original.w - (parent->world_pos_original.x + parent->rect_spritesheet_final.w));
			}
			else
			{
				world_pos_final.x = world_pos_original.x;
				rect_spritesheet_final.x = rect_spritesheet_original.x;
				rect_spritesheet_final.w = rect_spritesheet_original.w;
			}

			if (world_pos_original.y < parent->world_pos_original.y)
			{
				rect_spritesheet_final.y = rect_spritesheet_original.y + (parent->world_pos_original.y - world_pos_original.y);
				rect_spritesheet_final.h = rect_spritesheet_original.h - (parent->world_pos_original.y - world_pos_original.y);
				world_pos_final.y = world_pos_original.y + (parent->world_pos_original.y - world_pos_original.y);
			}
			else if (world_pos_original.y + rect_spritesheet_original.h > parent->world_pos_original.y + parent->rect_spritesheet_final.h)
			{
				rect_spritesheet_final.h = rect_spritesheet_original.h - (world_pos_original.y + rect_spritesheet_original.h - (parent->world_pos_original.y + parent->rect_spritesheet_final.h));
			}
			else
			{
				world_pos_final.y = world_pos_original.y;
				rect_spritesheet_final.y = rect_spritesheet_original.y;
				rect_spritesheet_final.h = rect_spritesheet_original.h;
			}*/
		}
	}
	return;
}


const bool ComponentUIObjectBase::MouseInRect() 
{
	float2 mouse_pos = App->input->GetMouse();
	

	return !(mouse_pos.x >= (rect_world.left + rect_world.Width()) || mouse_pos.x <= rect_world.left || mouse_pos.y >= (rect_world.top + rect_world.Height()) || mouse_pos.y <= rect_world.top);
}

const bool ComponentUIObjectBase::GetVisible() const
{
	return visible;
}

void ComponentUIObjectBase::SetAllVisible(const bool visible)
{

	list<ComponentUIObjectBase*> all_childrens;
	all_childrens.push_back(this);

	while (!all_childrens.empty())
	{
		list<ComponentUIObjectBase*>::iterator current_ui_object = all_childrens.begin();

		for (list<ComponentUIObjectBase*>::iterator item = (*current_ui_object)->childrens.begin(); item != (*current_ui_object)->childrens.end(); ++item)
		{
			all_childrens.push_back(*item);
		}

		(*current_ui_object)->SetVisible(visible);
		all_childrens.remove(*current_ui_object);
	}
}

void ComponentUIObjectBase::SetVisible(const bool visible)
{
	this->visible = visible;
}

void ComponentUIObjectBase::SetPos(float2 mouse_move)
{
	local_pos += mouse_move;
	world_pos += mouse_move;
	rect_world.left += mouse_move.x;
	rect_world.top += mouse_move.y;
	rect_world.right += mouse_move.x;
	rect_world.bottom += mouse_move.y;
	SetBufferVertexPos();

}