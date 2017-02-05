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
#include "AStar\Pathfinding.h"
#include "SeatArranger.h"

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
	void GenerateGroups();
	void CloseSeat(int index, Vector3 seatPos, bool &bSeatTaken);
	void FreeSeat(int index, Vector3 seatPos, bool &bSeatTaken);

    // temporary function to make sure customer reach queue before they flock together
    bool CheckIfCustomerReachDestination();
    // Takes the position of all entities in the flock except for the entity calling this function and then divides by the number of entities in that flock
    void calculateCOM(std::vector<CCustomer*> list, CCustomer& entity, Group group);
    // Calculates distance of entity from the rest of the flock, if close enough to be seperated, pass a direction vector 180 degrees opposite to the entity
    void calculateRepelVec(std::vector<CCustomer*> list, CCustomer& entity, float dt);
    // Calculates distance of customer from waiter to seperate from waiter
    void calculateRepelVec(CCustomer& customer, CWaiter& waiter, float dt);
    void CreateFlock(Vector3 seat_pos); // Creates a leader and his flock, seat_pos is for the leader to sit, which is temporary since they are supposed to sit together in one table
    // Container for all the different flocks of customers
    std::vector<std::vector<CCustomer*>> flock_list;
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
    CWaiter* usher;
	CChef* chef;
	CCleaner* cleaner;

    // Which flock can be joined
    int flock_to_join;

	// AStar
	//CGrid* m_grid;
	// Testing here, should be in waiter, cleaner and customer classes
	Pathfinding* pathfinder;

    // a list of customers
    std::vector<CCustomer*> customer_list;

	// List of nodes
	std::vector<CNode*> gridList;

	// List of seats
	std::vector<Vector3> seatList;

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
