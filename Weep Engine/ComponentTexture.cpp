#include "ComponentTexture.h"
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleTexture.h"
#include "ModuleGameObjectManager.h"
#include "ComponentMesh.h"

void ComponentTexture::ActivateThisTexture()
{

	//for (std::vector<Component*>::iterator iter = object->components.begin(); iter != object->components.end(); ++iter)
	//{
	//	if ((*iter)->type == ComponentType::TEXTURE)
	//	{
	//		ComponentTexture* component = (ComponentTexture*)(*iter);
	//		if (component->IsTextureActive() && component != this)
	//		{
	//			component->DesactivateTexture();  //only one activated. if this is activated but it exist another texture activated, desactivate the other.
	//			LOG("Exist another ComponentTexture which has been desactivated");
	//			return; //return because if it check that another texture has been desactivated, it can't be more than one.
	//		}
	//	}
	//}
	
	ComponentTexture* activated_texture = object->GetTextureActivated();
	if (activated_texture != nullptr)
	{
		activated_texture->DesactivateTexture();
		LOG("Exist another ComponentTexture which has been desactivated");
		

	}
	texture_active = true;
	object->GetMesh()->SetTexture(this);
	LOG("The texture has been activated correctly");

}


bool ComponentTexture::IsTextureActive() const
{
	return texture_active;
}

void ComponentTexture::DesactivateTexture()
{
	texture_active = false;
}

void ComponentTexture::Update()
{
	if (activate_checkers)
	{
		SetCheckersToGOSelected();
		activate_checkers = false;
	}
}

void ComponentTexture::SetCheckersToGOSelected()
{
	for (std::vector<GameObject*>::iterator iter = App->game_object_manager->selected.begin(); iter != App->game_object_manager->selected.end(); ++iter)
	{
		if ((*iter)->GetMesh()->GetTexture() != App->texture->GetCheckersTexture())
		{
			(*iter)->GetMesh()->SetTexture(App->texture->GetCheckersTexture());
		}
		else
		{
			(*iter)->GetMesh()->SetTexture((*iter)->GetTextureActivated());
		}
	}
}

void ComponentTexture::InspectorDraw()
{
	bool to_active = IsActive();
	if (ImGui::Checkbox("", &to_active))
		SetActive(to_active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::Text("Texture Path: %s ", App->texture->GetPathTexture().c_str());
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0, 1.0, 0.1, 1.0), "Texture Size");
		ImGui::Text("Width: %i px", App->texture->Width);
		ImGui::Text("Height: %i px", App->texture->Height);
		ImGui::Checkbox("Activate Checkers", &activate_checkers);

	}

}