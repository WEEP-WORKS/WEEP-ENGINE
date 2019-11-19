#include "ModuleQuadtree.h"
#include "QuadtreeNode.h"
ModuleQuadtree::ModuleQuadtree()
{
	root_quadtree = new QuadTreeNode(float3(0.f, 0.f, 0.f), float3(20.f, 10.f, 10.f));
}

bool ModuleQuadtree::Update()
{
	Draw();

	return true;
}

void ModuleQuadtree::Draw()
{
	root_quadtree->Draw();
	for (std::vector<QuadTreeNode*>::const_iterator citer = root_quadtree->subdivisions.cbegin(); citer != root_quadtree->subdivisions.cend(); ++citer)
	{
		(*citer)->Draw();
	}
}

