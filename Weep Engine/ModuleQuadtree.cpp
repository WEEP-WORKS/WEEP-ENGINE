#include "ModuleQuadtree.h"
#include "QuadtreeNode.h"
#include "GameObject.h"

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

void ModuleQuadtree::Insert(GameObject* go)
{
	if (!root_quadtree->Intersect(go->local_bbox))
	{
		root_quadtree->box.Enclose(go->local_bbox);
		root_quadtree->entities.push_back(go);// TODO:when delete go, delete from quadtree pointer go too!		
	}
	if (root_quadtree->entities.size() >= max_entities)
	{
		root_quadtree->Divide();
	}
}

void ModuleQuadtree::RecalculateQuadtree()
{
	//root_quadtree->box.
}

