#include "ComponentTexture.h"
#include "GameObject.h"
void ComponentTexture::ActivateThisTexture()
{
	texture_active = true;

	LOG("The texture has been activated correctly");

	for (std::vector<Component*>::iterator iter = object->components.begin(); iter != object->components.end(); ++iter)
	{
		if ((*iter)->type == ComponentType::TEXTURE)
		{
			ComponentTexture* component = (ComponentTexture*)(*iter);
			if (component->IsTextureActive() && component != this)
			{
				component->DesactivateTexture();  //only one activated. if this is activated but it exist another texture activated, desactivate the other.
				LOG("Exist another ComponentTexture which has been desactivated");
				return; //return because if it check that another texture has been desactivated, it can't be more than one.
			}
		}
	}
}

bool ComponentTexture::IsTextureActive() const
{
	return texture_active;
}

void ComponentTexture::DesactivateTexture()
{
	texture_active = false;
}