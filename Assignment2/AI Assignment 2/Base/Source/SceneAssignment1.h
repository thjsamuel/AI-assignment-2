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

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go, GameObject *go2, float dt);
	void GenerateCustomers();
	void CloseSeat(int index, Vector3 seatPos, bool &bSeatTaken);
	void FreeSeat(int index, Vector3 seatPos, bool &bSeatTaken);

    // temporary function to make sure customer reach queue before they flock together
    bool CheckIfCustomerReachDestination();
    // Takes the position of all entities in the flock except for the entity calling this function and then divides by the number of entities in that flock
    void calculateCOM(std::vector<CCustomer*> list, CCustomer& entity);
    // Calculates distance of entity from the rest of the flock, if close enough to be seperated, pass a direction vector 180 degrees opposite to the entity
    void calculateRepelVec(std::vector<CCustomer*> list, CCustomer& entity);
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

    // a list of customers
    std::vector<CCustomer*> customer_list;

	// For Debugging
	Vector3 debugPos;
    bool generateCus; // temporary
};

#endif // SCENE_ASSIGNMENT_1_H