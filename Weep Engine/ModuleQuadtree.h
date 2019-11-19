#ifndef __MODULEQUADTREE_H__
#define __MODULEQUADTREE_H__

#include "Module.h"
#include "MathGeoLib/include/Geometry/AABB.h"


class ModuleQuadtree : public Module
{
public:
	ModuleQuadtree();

	bool Update() override;

	void Draw();

private:
	AABB* root_quadtree;
};

#endif // !__MODULEQUADTREE_H__

