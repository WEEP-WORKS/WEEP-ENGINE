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
	bool PreUpdate() override;
	bool Update() override;
	bool PostUpdate() override;
	bool CleanUp() override;

	//GameObject* CreateGeometryShape(int sides);

	void CreateCube();
	void CreateSphere();

	void LoadGeometryShapeInfo(ComponentMesh * cmesh, par_shapes_mesh * mesh);

	void AddGameObjectToSelected(GameObject * go);

	void ReleaseGameObject(GameObject* object);

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
	bool						create_go_empty = false;

	GameObject* root;

	//should be private
	//std::list<GameObject*>		objects; //Vector or list?

	vector<GameObject*>			selected;

	list<GameObject*>			printed_hierarchy;

	list<GameObject*> to_delete;




private:
	//std::list<GameObject*> objects; //Vector or list?
	bool PrintGoList(GameObject * object);

	void AddGameObjectsSelectedToDestroy();

	void AddGameObjectToDestroy(GameObject* go);

	void DrawBBox(GameObject * object);




};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



