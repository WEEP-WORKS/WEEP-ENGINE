#ifndef __MODULEQUADTREE_H__
#define __MODULEQUADTREE_H__

#include "Module.h"
#include"QuadtreeNode.h"


class ModuleQuadtree : public Module
{
public:
	ModuleQuadtree();

	bool Update() override;

	

	void Draw();

	QuadTreeNode* root_quadtree;

private:

	
};

#endif // !__MODULEQUADTREE_H__

