#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"
#include "Globals.h"


class GameObject;
class ComponentMesh;

typedef struct par_shapes_mesh_s par_shapes_mesh;

class GameObjectManager : public Module
{
public:
	GameObjectManager(bool start_enabled = true);
	virtual ~GameObjectManager() {};

	bool Update() override;
	bool CleanUp() override;

	//GameObject* CreateSphere(const int &);
	void AddObject(GameObject*);

	GameObject* CreateGeometryShape(int sides);

	void CreateCube();
	void CreateSphere();

	void LoadGeometryShapeInfo(ComponentMesh * cmesh, par_shapes_mesh * mesh);

	void AddGameObjectToSelected(GameObject * go);

	void ClearSelection();

	void Hierarchy();
	// vector<GameObject*> GetSelectedGameObjects() const;

	void print1() { 
		LOG("FUNCT 1");
	};

	void print2() { LOG("FUNCT 2"); }

	void printThisFunction(std::function<void(GameObject*)> functVar)
	{
		//std::list<GameObject*>::iterator GO = objects.begin();
		//std::function<void()> functionbinded = std::bind(&GameObject::printGO, (*GO));
		for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
		{
			functVar(*item);
		}
	}

public:
	bool						create_cube		= false;
	bool						create_sphere	= false;

	//should be private
	std::list<GameObject*>		objects; //Vector or list?

	vector<GameObject*>			selected;



private:
	//std::list<GameObject*> objects; //Vector or list?
	void PrintGoList(GameObject * object);



};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



