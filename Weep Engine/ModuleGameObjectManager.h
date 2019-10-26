#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"

class GameObject;
class ComponentMesh;

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

	// vector<GameObject*> GetSelectedGameObjects() const;

	//should be private
	std::list<GameObject*> objects; //Vector or list?
	void Hierarchy();
	vector<GameObject*> selected;
	bool create_cube = false;
	bool create_sphere = false;

private:
	//std::list<GameObject*> objects; //Vector or list?
	void PrintGoList(GameObject * object);



};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



