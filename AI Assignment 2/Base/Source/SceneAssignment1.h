#ifndef SCENE_ASSIGNMENT_1_H
#define SCENE_ASSIGNMENT_1_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Cleaner\Cleaner.h"
#include "Waiter\Waiter.h"
#include "Chef\Chef.h"
#include "Customer\Customer.h"
#include "EntityManager.h"
#include "AStar\TestGrid.h"

struct Furniture
{
	Furniture(const Vector3& position, const Vector3& scale) 
	{ 
		this->position = position;
		this->scale = scale;
	};

	~Furniture();

	Vector3 position;
	Vector3 scale;
};

class SceneAssignment1 : public SceneBase
{
public:
	SceneAssignment1();
	~SceneAssignment1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	void RenderLocationNames();
	void RenderRestaurant();
	void RenderEntities();
	void RenderEntities_States();
	void RenderSeat(Vector3 position, unsigned int side);
	void AddSeatsToList();
	void InitFurniturePosition();

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go, GameObject *go2, float dt);
	void GenerateCustomers();
	void CloseSeat(int index, Vector3 seatPos, bool &bSeatTaken);
	void FreeSeat(int index, Vector3 seatPos, bool &bSeatTaken);

protected:
	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
    int storage_tables; // Number of tables in storage room
private:
	CEntityManager* entityMgr;

	CCustomer* customer;
	CWaiter* waiter;
	CChef* chef;
	CCleaner* cleaner;

    // a list of customers
    std::vector<CCustomer*> customer_list;

	// List of grids
	std::vector<CTestGrid*> gridList;

	// List of seats
	std::vector<Vector3> seatList;

	// List of furniture
	std::vector<Furniture*> furnitureList;

	// For Debugging
	Vector3 debugPos;

	Furniture* leftWall;
	Furniture* rightWall;
	Furniture* frontWall;
	Furniture* backWall;
	Furniture* kitchen;
	Furniture* divider1;
	Furniture* divider2;
	Furniture* divider3;
};

#endif // SCENE_ASSIGNMENT_1_H