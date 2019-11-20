#ifndef __MODULEQUADTREE_H__
#define __MODULEQUADTREE_H__

#include "Module.h"
#include "Globals.h"
#include"QuadtreeNode.h"
class GameObject;

class ModuleQuadtree : public Module
{
public:
	ModuleQuadtree();

	bool Update() override;

	void Insert(GameObject* go);

	void RecalculateQuadtree();

	void Draw();

	QuadTreeNode* root_quadtree;

private:

	uint max_entities = 2;
};

#endif // !__MODULEQUADTREE_H__

