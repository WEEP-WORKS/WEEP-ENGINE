#include "QuadtreeNode.h"
#include "glew/glew.h"


QuadTreeNode::QuadTreeNode(const float3& min_point_AABB, const float3& max_point_AABB)
{
	box.minPoint = min_point_AABB;
	box.maxPoint = max_point_AABB;
}

void QuadTreeNode::Draw()
{
	static float3 corners[8];
	box.GetCornerPoints(corners);

	GLint previous[2];
	glGetIntegerv(GL_POLYGON_MODE, previous);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(255.f, 45.f, 26.f);

	glLineWidth(4.0);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, previous[0]);

	glLineWidth(1.0f);

	glColor3f(255, 255, 255);
}

bool QuadTreeNode::Intersect(AABB& aabb)
{
	return box.Intersects(aabb);
}

void QuadTreeNode::Divide()
{
	if (!IsSubdivided())
	{
		//bottom layer
		QuadTreeNode* division1 = CreateDivision(box.minPoint, box.CenterPoint());
		QuadTreeNode* division2 = CreateDivision(box.PointInside(0.5f, 0.f, 0.f) , box.PointInside(1.f, 0.5f, 0.5f));
		QuadTreeNode* division3 = CreateDivision(box.PointInside(0.f, 0.f, 0.5f), box.PointInside(0.5f, 0.5f, 1.f));
		QuadTreeNode* division4 = CreateDivision(box.PointInside(0.5f, 0.f, 0.5f), box.PointInside(1.f, 0.5f, 1.f));

		//top layer
		QuadTreeNode* division5 = CreateDivision(box.PointInside(0.f, 0.5f, 0.f), box.PointInside(0.5f, 1.f, 0.5f));
		QuadTreeNode* division6 = CreateDivision(box.PointInside(0.5f, 0.5f, 0.f), box.PointInside(1.f, 1.f, 0.5f));
		QuadTreeNode* division7 = CreateDivision(box.PointInside(0.f, 0.5f, 0.5f), box.PointInside(0.5f, 1.f, 1.f));
		QuadTreeNode* division8 = CreateDivision(box.CenterPoint(), box.maxPoint);

	}
}

const bool QuadTreeNode::IsSubdivided() const
{
	return !subdivisions.empty();
}

QuadTreeNode* QuadTreeNode::CreateDivision(const float3& min_point_AABB, const float3& max_point_AABB)
{
	QuadTreeNode* division = new QuadTreeNode(min_point_AABB, max_point_AABB);
	subdivisions.push_back(division);
	return division;

}