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

protected:
	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;

private:
	CEntityManager* entityMgr;

	CCustomer* customer;
	CWaiter* waiter;
	CChef* chef;
	CCleaner* cleaner;

	// For Debugging
	Vector3 debugPos;
};

#endif // SCENE_ASSIGNMENT_1_H