#include "ComponentUIButton.h"
//#include "Globals.h"
#include "GameObject.h"
#include "ComponentRender2D.h"
#include "App.h"
#include "ModuleInput.h"
//#include "ModuleWindow.h"

ComponentUIButton::ComponentUIButton(UIType _type, float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent) : ComponentUIObjectBase(local_pos, rect_spritesheet_original, draggable, parent)
{
	type = _type;
}

void ComponentUIButton::PreUpdate()
{
	bool ret = true;
	if (GetVisible())
	{
		switch (state)
		{
		case ComponentUIButton::UIButtonState::NONE:
			if (MouseInRect())
			{
				state = UIButtonState::HOVER;
				current_texture_id = texture_id_hover;
				LOG("Mouse Enter!");

			}
			break;
		case ComponentUIButton::UIButtonState::HOVER:
			if (!MouseInRect())
			{
				state = UIButtonState::NONE;
				current_texture_id = texture_id_background;
				LOG("Mouse Exit!");
			}
			else if (App->input->GetMouseButton(1) == KEY_DOWN)
			{
				state = UIButtonState::CLICKED;
				current_texture_id = texture_id_clicked;
			}
			break;
		case ComponentUIButton::UIButtonState::CLICKED:
			if (App->input->GetMouseButton(1) == KEY_UP)
			{
				if (MouseInRect())
				{
					state = UIButtonState::HOVER;
					current_texture_id = texture_id_hover;
					ret = listener->ButtonEvent(button_type);
				}
				else
				{
					state = UIButtonState::NONE;
					current_texture_id = texture_id_background;
				}
			}

			break;
		default:
			break;
		}
	}
	return;
}

void ComponentUIButton::PostUpdate()
{
	//App->render->DrawQuad(rect_world, 255, 0, 0, 50, true, false);
	if(GetVisible())
		object->GetRender2D()->Render(vertexs_quad, uv_quad, current_texture_id);// App->render->Blit(atlas, world_pos_final.x, world_pos_final.y, &rect_spritesheet_final, 1.0f, SDL_FLIP_NONE, false);

	//Rect window = App->window->GetWindowRect();
	//float2 position = local_pos;
	//float2{ (float)window.right / 2 - 110 / 2, (float)window.bottom / 2
	//SetPos(position - local_pos);

	return;
}

void ComponentUIButton::CleanUp()
{

	return;
}

void ComponentUIButton::InspectorDraw()
{
	if (ImGui::CollapsingHeader("UIButton", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (parent != nullptr)
		{
			ImGui::Text("Parent: %s ", parent->object->GetName());
			ImGui::Text("Has Parent, be careful when drag");
		}
		else
			ImGui::Text("Don't have parent");

		ImGui::Separator();

		float2 position = local_pos;

		if (ImGui::DragFloat2("Position", (float*)&position, 0.1f))
			SetPos(position- local_pos);

	}
}