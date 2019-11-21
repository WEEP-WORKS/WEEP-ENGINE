#ifndef __MODULEQUADTREE_H__
#define __MODULEQUADTREE_H__

#include "Module.h"
#include "Globals.h"
#include"QuadtreeNode.h"
#include <functional>
#include <vector>
#include "MathGeoLib/include/Geometry/Frustum.h"


class GameObject;


class ModuleQuadtree : public Module
{
public:
	ModuleQuadtree();

	bool PreUpdate() override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void Insert(GameObject* go);

	void RecalculateAllQuadtree();

	void Draw();

	void Clear();

	void DeleteQuadTreeNode(QuadtreeNode* to_delete);

	void DeleteGOFromQuadtree(GameObject*);

	bool IsRootReset() const;

	void ResetRoot();

	const uint GetMaxEntities() const;

	std::vector<GameObject*> GetAllGameObjectsinsideFrustrum(Frustum& frustrum);

	QuadtreeNode* GetQuadtreeNodeWithThisGO(GameObject*);

	void DoForAllQuadtreeNodes(std::function<QuadtreeNode::CollisionType(QuadtreeNode*, GameObject*)> funct, GameObject* go);
	void DoForAllQuadtreeNodes(std::function<void(QuadtreeNode*)> funct);



	std::vector<GameObject*> GetAllGameObjects();

	QuadtreeNode* root_quadtree;

	bool to_recalculate = false;

	bool quadtree_dynamic = false;

private:

	uint max_entities = 2;
};

#endif // !__MODULEQUADTREE_H__

