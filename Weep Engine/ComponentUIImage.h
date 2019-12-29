#ifndef __COMPONENTUIIMAGE_H__
#define __COMPONENTUIIMAGE_H__

#include "ComponentUIObjectBase.h"
class ComponentUIImage : public ComponentUIObjectBase
{
public:
	ComponentUIImage(UIType _type, float2 local_pos, Rect rect_spritesheet_original, bool draggable, ComponentUIObjectBase* parent) : ComponentUIObjectBase(local_pos, rect_spritesheet_original, draggable, parent) { type = _type; }

};

#endif // !__COMPONENTUIIMAGE_H__
