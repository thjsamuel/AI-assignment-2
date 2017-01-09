#ifndef SCENE_ASSIGNMENT_1_H
#define SCENE_ASSIGNMENT_1_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "../Source/Rayner/Cleaner/Cleaner.h"
#include "../Source/Rayner/Waiter/Waiter.h"
#include "../Source/Rayner/Chef/Chef.h"
#include "../Source/Rayner/Customer/Customer.h"
#include "../Source/Rayner/EntityManager.h"

/*AI*/
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string>
//#include <GL/glew.h>
//#include <GLFw/glfw3.h>
//#include <ft2build.h>
#include "../Source/Samuel/S_Chef.h"
#include "../Source/Samuel/Object.h"
#include "../Source/Samuel/S_Customer.h"
#include "../Source/Samuel/S_Waiter.h"
#include <map>
using std::vector;
/**/

class SceneAssignment1 : public SceneBase
{
public:
	SceneAssignment1();
	~SceneAssignment1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

    void RenderRayner();
    void RenderSamuel();
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

    /*AI*/
    void initFSM();
    int createObject(const unsigned int ID = -1, MyVector pos = MyVector(0, 0), int existingID = -1, Obj::OBJTYPE type = Obj::DOOR); // returns ID if successful and 0 if not
    void renderObjects();
    Chef* ramsay;
    Waiter* s_waiter;
    vector<S_Customer*> customerList;
    map<const int, vector<Obj>> GOMap;
    const static int LatestID = 0;
    void createCustomer(float x, float y, float patienceMeter = 100);
    void CollaborateWaiterCustomer();
    void CollaborateWaiterCook();
    void CollaborateWaiterCustomerCook();
    /**/

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