#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"
#include "Globals.h"
#include<typeinfo>

#include "ImGuizmo.h"
#include "MathGeoLib/include/MathGeoLib.h"


class GameObject;
class ResourceMesh;

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
	void Save(Json::Value&) const override;
	void Load(const Json::Value&) override;

	void SetGuizmoOperation(ImGuizmo::OPERATION op);

	//GameObject* CreateGeometryShape(int sides);

	void CreateCube();
	void CreateSphere();

	void LoadGeometryShapeInfo(ResourceMesh * cmesh, par_shapes_mesh * mesh) const;

	void AddGameObjectToSelected(GameObject * go) const;

	void ReleaseGameObject(GameObject* object);

	void Destroy(GameObject * go);

	void ClearSelection();

	void Hierarchy();
	// vector<GameObject*> GetSelectedGameObjects() const

	int DoForAllChildrens(std::function<void(GameObjectManager*, GameObject*)>, GameObject* start = nullptr);

	int DoForAllChildrensVertical(std::function<void(GameObjectManager*, GameObject*)>);

	void DoForFirstChildrens(std::function<void(GameObjectManager*, GameObject*)>, GameObject* start = nullptr);

	const uint GetAllGameObjectNumber() const;

	GameObject* GetGOById(const uint& id) const;

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
	void PrintGoList(GameObject * object);

	void AddGameObjectsSelectedToDestroy();

	void AddGameObjectToDestroy(GameObject* go);

	void DrawBBox(const GameObject * object) const;

	void MousePick();

	ImGuizmo::OPERATION current_gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
	float3 last_moved_transformation = float3::zero;
};

#endif // !__MODULEGAMEOBJECTMANAGER_H__



