#ifndef __MODULEQUADTREE_H__
#define __MODULEQUADTREE_H__

#include "Module.h"
#include "Globals.h"
#include"QuadtreeNode.h"
#include <functional>
class GameObject;

class ModuleQuadtree : public Module
{
public:
	ModuleQuadtree();

	bool Update() override;

	bool CleanUp() override;

	void Insert(GameObject* go);

	void RecalculateAllQuadtree();

	void Draw();

	void Clear();

	void DeleteQuadTreeNode(QuadtreeNode* to_delete);

	const uint GetMaxEntities() const;

	void DoForAllQuadtreeNodes(std::function<QuadtreeNode::CollisionType(QuadtreeNode*, GameObject*)> funct, GameObject* go);
	void DoForAllQuadtreeNodes(std::function<void(QuadtreeNode*)> funct);



	std::vector<GameObject*> GetAllGameObjects();

	QuadtreeNode* root_quadtree;

private:

	uint max_entities = 2;
};

#endif // !__MODULEQUADTREE_H__

