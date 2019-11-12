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
	bool PostUpdate();
	void DoUpdateIfActivated(GameObject * go);
	bool CleanUp() override;

	//GameObject* CreateGeometryShape(int sides);

	void CreateCube();
	void CreateSphere();

	void LoadGeometryShapeInfo(ComponentMesh * cmesh, par_shapes_mesh * mesh);

	void AddGameObjectToSelected(GameObject * go);

	void DeleteGameObject(GameObject* object);

	void DestroySelectedGameObjects();

	void Destroy(GameObject * go);

	void ClearSelection();

	void Hierarchy();
	// vector<GameObject*> GetSelectedGameObjects() const

	int DoForAllChildrens(std::function<void(GameObjectManager*, GameObject*)>, GameObject* start = nullptr);

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
	bool PrintGoList(GameObject * object);

	void DrawBBox(GameObject * object);
	void AllTreePop(GameObject* object);


	list<GameObject*> to_delete;

};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



