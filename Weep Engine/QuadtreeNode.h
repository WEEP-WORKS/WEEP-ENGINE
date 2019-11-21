#ifndef __QUADTREENODE_H__
#define __QUADTREENODE_H__

#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
#include "Globals.h"
class GameObject;

class QuadtreeNode
{

public:
	enum class CollisionType
	{
		INTERSERCT_BORDER,
		FULLY_CONTAINED,
		NONE_COLLISION
	};


	QuadtreeNode(const float3& min_point_AABB, const float3& max_point_AABB, QuadtreeNode* parent = nullptr);

	void Draw();

	void Clear();

	void DeleteGOFromEntities(GameObject*);

	void Divide();
	const bool IsSubdivided() const;

	CollisionType OnCollision(AABB&);

	AABB box;

	CollisionType InsertGOInThis(GameObject* go);

	bool IsThisInsideFrustrum(Frustum& frustrum);

	QuadtreeNode* parent = nullptr;

	const std::vector<GameObject*>* GetEntities() const;

	std::vector<QuadtreeNode*> subdivisions;

	std::vector<GameObject*> entities;

private:
	QuadtreeNode* CreateDivision(const float3& min_point_AABB, const float3& max_point_AABB, QuadtreeNode* parent = nullptr);



};

#endif // !__QUADTREENODE_H__

