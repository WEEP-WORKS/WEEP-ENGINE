#ifndef __QUADTREENODE_H__
#define __QUADTREENODE_H__

#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>


class QuadTreeNode
{
public:
	QuadTreeNode(const float3& min_point_AABB, const float3& max_point_AABB);

	void Draw();


	void Divide();
	const bool IsSubdivided() const;

	AABB box;

	std::vector<QuadTreeNode*> subdivisions;

private:
	QuadTreeNode* CreateDivision(const float3& min_point_AABB, const float3& max_point_AABB);

};

#endif // !__QUADTREENODE_H__

