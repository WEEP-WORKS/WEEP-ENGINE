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

	bool Awake() override;
	bool Update() override;
	void DoUpdateIfActivated(GameObject * go);
	bool CleanUp() override;

	//GameObject* CreateSphere(const int &);
	void AddObject(GameObject*);

	//GameObject* CreateGeometryShape(int sides);

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
		/*for (list<GameObject*>::iterator item = objects.begin(); item != objects.end(); ++item)
		{
			functVar(*item);
		}*/
	}

	int DoForAllChildrens(std::function<void(GameObjectManager*, GameObject*)>);

	int DoForAllChildrensVertical(std::function<void(GameObjectManager*, GameObject*)>);

	uint GetAllGameObjectNumber();




public:
	bool						create_cube		= false;
	bool						create_sphere	= false;

	GameObject* root;

	//should be private
	//std::list<GameObject*>		objects; //Vector or list?

	vector<GameObject*>			selected;

	list<GameObject*>			printed_hierarchy;





private:
	//std::list<GameObject*> objects; //Vector or list?
	void PrintGoList(GameObject * object);
	void AllTreePop(GameObject* object);



};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



