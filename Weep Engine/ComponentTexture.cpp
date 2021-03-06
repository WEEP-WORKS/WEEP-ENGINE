#include "ComponentTexture.h"
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleTexture.h"
#include "ModuleGameObjectManager.h"
#include "ComponentMesh.h"
#include "ResourceManagment.h"
#include "ResourceTexture.h"
void ComponentTexture::ActivateThisTexture()
{
	
	ComponentTexture* activated_texture = object->GetTextureActivated();
	if (activated_texture != nullptr)
	{
		activated_texture->DesactivateTexture();
		LOG("Exist another ComponentTexture which has been desactivated");
		

	}
	texture_active = true;
	if(object->GetMesh())
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
	if (object->GetMesh())
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
	if (object->GetRender2D() == nullptr)
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
	}


	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
		const ResourceTexture* text_info = GetResource(resource_id);
		if(text_info != nullptr)
			ImGui::Text("Texture Path: %s ", text_info->texture_path.c_str());
		else
			ImGui::Text("Don't have texture");

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0, 1.0, 0.1, 1.0), "Texture Size");
		if (text_info != nullptr)
		{
			ImGui::Text("Width: %i px", text_info->texture_width);
			ImGui::Text("Height: %i px", text_info->texture_height);
		}

		if (object->GetRender2D() == nullptr)
			ImGui::Checkbox("Activate Checkers", &activate_checkers);

	}

}

void ComponentTexture::Save(Json::Value& scene) const
{
	Json::Value component_texture;

	component_texture["type"] = (int)type;

	if(GetResource(resource_id) != nullptr)
		component_texture["Texture Path"] = GetResource(resource_id)->texture_path;
	component_texture["Texture Active"] = texture_active;

	scene.append(component_texture);
}


void ComponentTexture::Load(const Json::Value& component)
{
	if (component["Texture Active"].asBool())
		ActivateThisTexture();
	if (!component["Texture Path"].isNull())
	{
		std::string final_path("Assets/Textures/" + component["Texture Path"].asString());
		App->texture->LoadTexture(final_path.c_str(), this);
	}
	else
		LOG("NoTexture");

}

const ResourceTexture* ComponentTexture::GetResource(UID id) const
{
	return (ResourceTexture*)App->resource_managment->GetByID(id);
}

ResourceTexture* ComponentTexture::GetResource(UID id) 
{
	return (ResourceTexture*)App->resource_managment->GetByID(id);
}

const UID ComponentTexture::GetResourceID() const
{
	return resource_id;
}

void ComponentTexture::SetResourceID(UID id)
{
	resource_id = id;

}