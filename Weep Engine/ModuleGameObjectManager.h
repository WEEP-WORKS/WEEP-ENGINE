#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"

class GameObject;

class GameObjectManager : public Module
{
public:
	GameObjectManager(bool start_enabled = true);
	virtual ~GameObjectManager() {};

	bool Update() override;
	void SetCheckersToGOSelected();
	bool CleanUp() override;

	//GameObject* CreateSphere(const int &);
	void AddObject(GameObject*);

	GameObject* CreateGeometryShape(int sides);

	void CreateCube();

	void AddGameObjectToSelected(GameObject * go);

	void ClearSelection();

	// vector<GameObject*> GetSelectedGameObjects() const;

	//should be private
	std::list<GameObject*> objects; //Vector or list?
	void Hierarchy();
	vector<GameObject*> selected;


private:
	//std::list<GameObject*> objects; //Vector or list?
	void PrintGoList(GameObject * object);



};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



