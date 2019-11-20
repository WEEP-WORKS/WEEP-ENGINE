#ifndef __QUADTREENODE_H__
#define __QUADTREENODE_H__

#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
class GameObject;

class QuadTreeNode
{
public:
	QuadTreeNode(const float3& min_point_AABB, const float3& max_point_AABB);

	void Draw();


	void Divide();
	const bool IsSubdivided() const;

	bool Intersect(AABB&);

	AABB box;

	std::vector<QuadTreeNode*> subdivisions;

	std::vector<GameObject*> entities;

private:
	QuadTreeNode* CreateDivision(const float3& min_point_AABB, const float3& max_point_AABB);



};

#endif // !__QUADTREENODE_H__

