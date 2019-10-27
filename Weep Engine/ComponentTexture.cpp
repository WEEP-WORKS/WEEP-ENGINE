#include "ComponentTexture.h"
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleTexture.h"
#include "ModuleGameObjectManager.h"
#include "ComponentMesh.h"

void ComponentTexture::ActivateThisTexture()
{
	
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
	object->GetMesh()->SetTexture(nullptr);
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
	bool to_active = IsTextureActive();
	if (ImGui::Checkbox("", &to_active))
	if (to_active == true)
	{
		ActivateThisTexture();
	}
	else
	{
		DesactivateTexture();
	}

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::Text("Texture Path: %s ", texture_path.c_str());
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0, 1.0, 0.1, 1.0), "Texture Size");
		ImGui::Text("Width: %i px", texture_width);
		ImGui::Text("Height: %i px", texture_height);
		ImGui::Checkbox("Activate Checkers", &activate_checkers);

	}

}