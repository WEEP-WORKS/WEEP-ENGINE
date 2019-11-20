#include "ModuleQuadtree.h"
#include "QuadtreeNode.h"
#include "GameObject.h"

ModuleQuadtree::ModuleQuadtree()
{
	root_quadtree = new QuadtreeNode(float3(0.f, 0.f, 0.f), float3(10.f, 10.f, 10.f));
}

bool ModuleQuadtree::Update()
{
	Draw();
	return true;
}

bool ModuleQuadtree::CleanUp()
{
	Clear();

	//Clear() function do not Delete the root_quadtree! Only clear subdivisions and entities. only Delete the root_quadtre in the cleanUp(). 
	DeleteQuadTreeNode(root_quadtree);

	return true;
}

void ModuleQuadtree::Draw()
{
	DoForAllQuadtreeNodes(&QuadtreeNode::Draw);

}

void ModuleQuadtree::Clear()
{
	DoForAllQuadtreeNodes(&QuadtreeNode::Clear);
}

void ModuleQuadtree::DeleteQuadTreeNode(QuadtreeNode* to_delete)
{
	RELEASE(to_delete);
}

void ModuleQuadtree::Insert(GameObject* go)
{
	QuadtreeNode::CollisionType col_type = root_quadtree->OnCollision(go->local_bbox);
	if (col_type != QuadtreeNode::CollisionType::FULLY_CONTAINED)
	{
		root_quadtree->box.Enclose(go->local_bbox);
		RecalculateAllQuadtree();

	}

	DoForAllQuadtreeNodes(&QuadtreeNode::InsertGOInThis, go);
}

void ModuleQuadtree::RecalculateAllQuadtree()
{
	std::vector<GameObject*> game_objects = GetAllGameObjects();
	Clear();
	while (!game_objects.empty())
	{
		GameObject* current = (*game_objects.rbegin());
		game_objects.pop_back();
		Insert(current);
	}
}

std::vector<GameObject*> ModuleQuadtree::GetAllGameObjects()
{
	std::vector<GameObject*> ret;


	std::list<QuadtreeNode*> all_quadtree_nodes;
	all_quadtree_nodes.push_back(root_quadtree);

	while (!all_quadtree_nodes.empty())
	{
		QuadtreeNode* current = (*all_quadtree_nodes.begin());
		all_quadtree_nodes.pop_front();
		if (current != nullptr)
		{
			const std::vector<GameObject*>* current_entities = current->GetEntities();
			for (std::vector<GameObject*>::const_iterator citer = current_entities->begin(); citer != current_entities->end(); ++citer)
			{
				ret.push_back(*citer);
			}
			current_entities = nullptr;

			for (std::vector<QuadtreeNode*>::const_iterator iter = current->subdivisions.cbegin(); iter != current->subdivisions.cend(); ++iter)
			{
				all_quadtree_nodes.push_back(*iter);
			}
		}
	}

	return ret;
}

const uint ModuleQuadtree::GetMaxEntities() const
{
	return max_entities;
}






void ModuleQuadtree::DoForAllQuadtreeNodes(std::function<QuadtreeNode::CollisionType(QuadtreeNode*, GameObject*)> funct, GameObject* go)
{
	std::list<QuadtreeNode*> all_quadtree_nodes;
	all_quadtree_nodes.push_back(root_quadtree);

	while (!all_quadtree_nodes.empty())
	{
		QuadtreeNode* current = (*all_quadtree_nodes.begin());
		all_quadtree_nodes.pop_front();
		if (current != nullptr)
		{
			QuadtreeNode::CollisionType ret = funct(current, go);
			bool check_subdivisions = true;

			switch (ret)
			{
			case QuadtreeNode::CollisionType::INTERSERCT_BORDER:
				all_quadtree_nodes.clear();
				return;//The object has been added to the parent in this case.
				break;
			case QuadtreeNode::CollisionType::FULLY_CONTAINED:
				if (!current->IsSubdivided()) //the object has been added in this node
				{
					all_quadtree_nodes.clear();
					return;
				}
				else
				{
					//reset the list and will add only the subdivisions of current.
					all_quadtree_nodes.clear();
				}
				break;
			case QuadtreeNode::CollisionType::NONE_COLLISION:
				check_subdivisions = false;
				break;
			default:
				break;
			}

			if (check_subdivisions)
			{
				for (std::vector<QuadtreeNode*>::const_iterator iter = current->subdivisions.cbegin(); iter != current->subdivisions.cend(); ++iter)
				{
					all_quadtree_nodes.push_back(*iter);
				}
			}

		}
	}

}

void ModuleQuadtree::DoForAllQuadtreeNodes(std::function<void(QuadtreeNode*)> funct)
{
	std::list<QuadtreeNode*> all_quadtree_nodes;
	all_quadtree_nodes.push_front(root_quadtree);

	while (!all_quadtree_nodes.empty())
	{
		QuadtreeNode* current = (*all_quadtree_nodes.begin());
		all_quadtree_nodes.pop_front();
		if (current != nullptr)
		{
			for (std::vector<QuadtreeNode*>::const_iterator iter = current->subdivisions.cbegin(); iter != current->subdivisions.cend(); ++iter)
			{
				all_quadtree_nodes.push_back(*iter); 
			}

			funct(current);
		}
	}

}

