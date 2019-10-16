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
	bool CleanUp() override;

	//GameObject* CreateSphere(const int &);
	void AddObject(GameObject*);

	void AddGameObjectToSelected(GameObject * go);

	void ClearSelection();

	const vector<GameObject*> GetSelectedGameObjects() const;

	//should be private
	std::list<GameObject*> objects; //Vector or list?
	void Hierarchy();


private:
	//std::list<GameObject*> objects; //Vector or list?
	void PrintGoList(GameObject * object);
	vector<GameObject*> selected;

};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



