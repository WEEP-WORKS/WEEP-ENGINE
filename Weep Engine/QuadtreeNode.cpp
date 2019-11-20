#include "QuadtreeNode.h"
#include "GameObject.h"
#include "glew/glew.h"

#include "App.h"
#include "ModuleQuadtree.h"


QuadtreeNode::QuadtreeNode(const float3& min_point_AABB, const float3& max_point_AABB, QuadtreeNode* parent) : parent(parent)
{
	box.minPoint = min_point_AABB;
	box.maxPoint = max_point_AABB;
}

void QuadtreeNode::Draw()
{
	static float3 corners[8];
	box.GetCornerPoints(corners);

	GLint previous[2];
	glGetIntegerv(GL_POLYGON_MODE, previous);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glLineWidth(4.0);

	glBegin(GL_QUADS);
	glColor3f(2.f, 0.f, 2.f);
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

void QuadtreeNode::Clear()
{

	subdivisions.clear();
	entities.clear();

	if (this != App->quadtree->root_quadtree)
		App->quadtree->DeleteQuadTreeNode(this);

}

QuadtreeNode::CollisionType QuadtreeNode::OnCollision(AABB& aabb)
{
	QuadtreeNode::CollisionType ret = QuadtreeNode::CollisionType::NONE_COLLISION;
	if(box.Intersects(aabb))
	{
		ret = QuadtreeNode::CollisionType::INTERSERCT_BORDER;
		if (box.Contains(aabb))
		{
			ret = QuadtreeNode::CollisionType::FULLY_CONTAINED;
		}
	}

	return ret;
}

const std::vector<GameObject*>* QuadtreeNode::GetEntities() const
{
	return &entities;
}


void QuadtreeNode::Divide()
{

		//bottom layer
		QuadtreeNode* division1 = CreateDivision(box.minPoint, box.CenterPoint(), this);
		QuadtreeNode* division2 = CreateDivision(box.PointInside(0.5f, 0.f, 0.f) , box.PointInside(1.f, 0.5f, 0.5f), this);
		QuadtreeNode* division3 = CreateDivision(box.PointInside(0.f, 0.f, 0.5f), box.PointInside(0.5f, 0.5f, 1.f), this);
		QuadtreeNode* division4 = CreateDivision(box.PointInside(0.5f, 0.f, 0.5f), box.PointInside(1.f, 0.5f, 1.f), this);

		//top layer
		QuadtreeNode* division5 = CreateDivision(box.PointInside(0.f, 0.5f, 0.f), box.PointInside(0.5f, 1.f, 0.5f), this);
		QuadtreeNode* division6 = CreateDivision(box.PointInside(0.5f, 0.5f, 0.f), box.PointInside(1.f, 1.f, 0.5f), this);
		QuadtreeNode* division7 = CreateDivision(box.PointInside(0.f, 0.5f, 0.5f), box.PointInside(0.5f, 1.f, 1.f), this);
		QuadtreeNode* division8 = CreateDivision(box.CenterPoint(), box.maxPoint, this);

		std::vector<GameObject*> new_entities;

		for (std::vector<GameObject*>::iterator iter_go = entities.begin(); iter_go != entities.end(); )
		{
			bool check = true;
			for (std::vector<QuadtreeNode*>::iterator iter_quadtree = subdivisions.begin(); iter_quadtree != subdivisions.end(); ++iter_quadtree)
			{
				if (check)
				{
					QuadtreeNode::CollisionType col_type = (*iter_quadtree)->OnCollision((*iter_go)->local_bbox);
					switch (col_type)
					{
					case QuadtreeNode::CollisionType::INTERSERCT_BORDER:
						new_entities.push_back(*iter_go);
						check = false;
						break;
					case QuadtreeNode::CollisionType::FULLY_CONTAINED:
						(*iter_quadtree)->entities.push_back(*iter_go);
						check = false;
						break;
					case QuadtreeNode::CollisionType::NONE_COLLISION:
						break;
					default:
						break;
					}
				}
			}

			iter_go = entities.erase(iter_go);
		}

		entities = new_entities;

	
}

const bool QuadtreeNode::IsSubdivided() const
{
	return !subdivisions.empty();
}

QuadtreeNode* QuadtreeNode::CreateDivision(const float3& min_point_AABB, const float3& max_point_AABB, QuadtreeNode* parent)
{
	QuadtreeNode* division = new QuadtreeNode(min_point_AABB, max_point_AABB, parent);
	subdivisions.push_back(division);
	return division;

}

QuadtreeNode::CollisionType QuadtreeNode::InsertGOInThis(GameObject* go)
{

	QuadtreeNode::CollisionType col_type = OnCollision(go->local_bbox);
	switch (col_type)
	{
	case QuadtreeNode::CollisionType::INTERSERCT_BORDER:
		parent->entities.push_back(go);
		break;
	case QuadtreeNode::CollisionType::FULLY_CONTAINED:
		if (!IsSubdivided())
		{
			if (entities.size() >= App->quadtree->GetMaxEntities())
				Divide(); //add this entities of this in their children.

			else
				entities.push_back(go);	
			
		}

		break;
	case QuadtreeNode::CollisionType::NONE_COLLISION:
		break;
	default:
		LOG("Collision Type incorrect");
		break;
	}
	return col_type;
}