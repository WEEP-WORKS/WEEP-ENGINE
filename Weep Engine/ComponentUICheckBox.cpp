#include "ComponentUICheckBox.h"
//#include "Globals.h"
#include "GameObject.h"
#include "ComponentRender2D.h"
#include "App.h"
#include "ModuleInput.h"

ComponentUICheckBox::ComponentUICheckBox(UIType _type, float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent) : ComponentUIObjectBase(local_pos, rect_spritesheet_original, draggable, parent)
{
	type = _type;
}

void ComponentUICheckBox::PreUpdate()
{
	bool ret = true;
	if (GetVisible())
	{

		switch (state)
		{
		case ComponentUICheckBox::UICheckBoxState::NONE:
			if (MouseInRect() && App->input->GetMouseButton(1) == KEY_DOWN)
			{
				LOG("CheckBox activated");
				state = UICheckBoxState::CLICKED;
				current_texture_id = texture_id_clicked;
				ret = listener->CheckBoxEvent(check_box_type, true);


			}
			break;
		case ComponentUICheckBox::UICheckBoxState::CLICKED:
			if (MouseInRect() && App->input->GetMouseButton(1) == KEY_DOWN)
			{
				state = UICheckBoxState::NONE;
				current_texture_id = texture_id_background;
				ret = listener->CheckBoxEvent(check_box_type, false);

				LOG("CheckBox desactivated");

			}
			break;
		default:
			break;
		}
	}
	return;
}

void ComponentUICheckBox::PostUpdate()
{
	//App->render->DrawQuad(rect_world, 255, 0, 0, 50, true, false);
	if (GetVisible())
		object->GetRender2D()->Render(vertexs_quad, uv_quad, current_texture_id);// App->render->Blit(atlas, world_pos_final.x, world_pos_final.y, &rect_spritesheet_final, 1.0f, SDL_FLIP_NONE, false);


	return;
}

void ComponentUICheckBox::CleanUp()
{

	return;
}
