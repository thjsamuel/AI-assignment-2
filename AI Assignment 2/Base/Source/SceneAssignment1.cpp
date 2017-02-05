#include "SceneAssignment1.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "MyTimer.h"
#include "EntityNames.h"
#include "Messaging\MessageDispatcher.h"
#include "Locations.h"

CMyTimer* clock_ = CMyTimer::GetInstance();

SceneAssignment1::SceneAssignment1()
{
}

SceneAssignment1::~SceneAssignment1()
{
}

// Takes two diagonal vertice representing min and max and fill the area between min and max with waypoints, used by waiter to place tables randomly only in a certain area
// The container you pass in is the waiter's waypoints to follow, so basically you just make him go random places
void SetAreaWithWaypoint(const Vector3 min, const Vector3 max, std::map<const int, Vector3>& container, const int containerSize)
{
    float xmin = min.x; int xmax = max.x; int ymin = min.y; int ymax = max.y; // get X and Y coordinate values of min and max points on the screen
    for (int i = 0; i < containerSize; ++i)
    {
        container[i].Set(Math::RandFloatMinMax(xmin, xmax), Math::RandFloatMinMax(ymin, ymax)); // Set positions inside the map structures with random points in the allocated area
        for (int j = 0; j < i; ++j)
            while (container[i] != container[j] == false) // This just reduces the chances of two waypoints being placed in the same spot
            {
                container[i].Set(Math::RandFloatMinMax(xmin, xmax), Math::RandFloatMinMax(ymin, ymax));
            }
    }
    // Increase min and max for more space
}

void SceneAssignment1::Init()
{
	srand(time(NULL));

	SceneBase::Init();

	entityMgr = CEntityManager::GetInstance();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    flock_to_join = Group::GROUP_1;
    for (int i = 0; i < Group::GROUP_MAX; ++i)
    {
        flock_list.push_back(*new std::vector<CCustomer*>());
    }

	// AStar
	//m_grid = new CGrid();
	//m_grid->CreateGrid();
	pathfinder = new Pathfinding();
	pathfinder->GetGrid()->CreateGrid();
    // yeah it is trashy hardcode, working on it
    storage_tables = 5;
	waiter = new CWaiter(ENT_WAITER);
    usher = new CWaiter(ENT_WAITER_OUTSIDE);
    /*The following is waiter arrange state code*/
    SetAreaWithWaypoint(Vector3(35, 30, 0), Vector3(100, 70, 0), waiter->waypoints, storage_tables);
    waiter->tables_left = storage_tables; // waiter knows how many tables there are
    usher->tables_left = storage_tables; // usher doesn't
    usher->waypoints[0] = USHERING;
    /**/
	entityMgr->RegisterEntity(waiter);
    entityMgr->RegisterEntity(usher);

	chef = new CChef(ENT_CHEF);
	entityMgr->RegisterEntity(chef);

	cleaner = new CCleaner(ENT_CLEANER);
	entityMgr->RegisterEntity(cleaner);

	entityMgr->SetLatestID(entityMgr->GetEntityMap().size());

    Vector3 pos = ENTRANCE;
    //for (int i = 0; i < 5; ++i)
    {
        //CCustomer* customer;
        //if (i == 0)
            customer = new CCustomer(entityMgr->GetNextID(), SEAT_1, true);
        //else if (i == 1)
        //    customer = new CCustomer(entityMgr->GetNextID(), SEAT_2, false);
        //else if (i == 2)
        //    customer = new CCustomer(entityMgr->GetNextID(), SEAT_3, false);
        //else if (i == 3)
        //    customer = new CCustomer(entityMgr->GetNextID(), SEAT_4, false);
        //else if (i == 4)
        //    customer = new CCustomer(entityMgr->GetNextID(), SEAT_5, false);

        pos.x += 5;
        customer->waypoints[0] = ENTRANCE;
        entityMgr->RegisterEntity(customer);
        entityMgr->SetStillHereID(customer->GetID());
        customer_list.push_back(customer);
    }

    //for (int i = 0; i < customer_list.size(); ++i)
    //{
    //    const int away_dist = 20;
    //    Vector3 behind_pos = customer_list.back()->waypoints[0]; // this assumes that the latest customer is queueing up as well and that the new customer should line up behind him by away_distance
    //    behind_pos.x += away_dist;
    //    customer_list[i]->waypoints[0] = behind_pos;
    //}

	// Set starting size of entity map
	entityMgr->SetStartingSize(entityMgr->GetEntityMap().size());

	// For Debugging
	debugPos = Vector3(70, 50, 0);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;

	InitFurniturePosition();
	AddSeatsToList(); 
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	for (int x = 0; x < pathfinder->GetGrid()->GetGridSizeX(); x++)
	{
		for (int y = 0; y < pathfinder->GetGrid()->GetGridSizeY(); y++)
		{
			for (std::vector<Furniture*>::iterator it = entityMgr->GetFurnitureList()->begin(); it < entityMgr->GetFurnitureList()->end(); it++)
			{
				Vector3 w0 = (*it)->position;//go2->pos;
				Vector3 b1 = pathfinder->GetGrid()->Get()[x][y].GetPosition();
				Vector3 N = Vector3(1, 0, 0); //go2->normal;
				Vector3 dir = w0 - b1;
				if (dir.Dot(N) < 0)
					N = -N;

				float r = 0.5f; // go->scale.x;
				float h = (*it)->scale.x;
				float l = (*it)->scale.y;
				Vector3 NP = Vector3(-N.y, N.x);

				if (abs((dir).Dot(N)) < r + h * 0.5f
					&& abs((dir).Dot(NP)) < l * 0.5f)
				{
					pathfinder->GetGrid()->Get()[x][y].SetWalkable(false);
				}
			}
		}
	}
}

void SceneAssignment1::InitFurniturePosition()
{
	leftWall = new Furniture(Vector3(1.5, 60, 0), Vector3(3, 85, 0));
	rightWall = new Furniture(Vector3(132, 60, 0), Vector3(3, 85, 0));
	frontWall = new Furniture(Vector3(66, 99, 0), Vector3(130, 3, 0));
	backWall = new Furniture(Vector3(81, 19, 0), Vector3(100, 3, 0));
	kitchen = new Furniture(Vector3(25, 75, 0), Vector3(58, 3, 0));
	divider1 = new Furniture(Vector3(55, 87, 0), Vector3(3, 27, 0));
	divider2 = new Furniture(Vector3(80, 87, 0), Vector3(3, 27, 0));
	divider3 = new Furniture(Vector3(105, 87, 0), Vector3(3, 27, 0));

	entityMgr->GetFurnitureList()->push_back(leftWall);
	entityMgr->GetFurnitureList()->push_back(rightWall);
	entityMgr->GetFurnitureList()->push_back(frontWall);
	entityMgr->GetFurnitureList()->push_back(backWall);
	entityMgr->GetFurnitureList()->push_back(kitchen);
	entityMgr->GetFurnitureList()->push_back(divider1);
	entityMgr->GetFurnitureList()->push_back(divider2);
	entityMgr->GetFurnitureList()->push_back(divider3);
}

GameObject* SceneAssignment1::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	GameObject *go = m_goList.back();
	go->active = true;
	++m_objectCount;
	return go;
}

bool SceneAssignment1::CheckCollision(GameObject *go, GameObject *go2, float dt)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		float distSquared = (((go->pos + go->vel * dt) - (go2->pos + go2->vel * dt))).LengthSquared();
		float combinedRadius = go->scale.x + go2->scale.x;

		Vector3 relativeVelocity = go->vel - go2->vel;
		Vector3 relativeDisplacement = go->pos - go2->pos;

		return distSquared <= combinedRadius * combinedRadius && relativeVelocity.Dot(relativeDisplacement) < 0;
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go->pos + go->vel * dt;
		Vector3 N = go2->normal;
		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
			N = -N;

		float r = go->scale.x;
		float h = go2->scale.x;
		float l = go2->scale.y;
		Vector3 NP = Vector3(-N.y, N.x); //Vector3 NP = N.Cross(Vector3(0, 0, 1));
		return abs((dir).Dot(N)) < r + h * 0.5f
			&& abs((dir).Dot(NP)) < /*r +*/ l * 0.5f
			&& go->vel.Dot(N) > 0;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go->pos;
		float r1 = go->scale.x;
		Vector3 p2 = go2->pos;
		float r2 = go2->scale.x;
		Vector3 u = go->vel;
		float combinedRadius = r1 + r2;
		return abs(((p2 - p1).LengthSquared() < combinedRadius * combinedRadius) && (p2 - p1).Dot(u) > 0);
	}
	case GameObject::GO_HOLE:
	{
		return true;
	}
	}
}

void SceneAssignment1::CloseSeat(int index, Vector3 seatPos, bool &bSeatTaken)
{
	if (entityMgr->GetEntityMap()[index]->GetSeatPosition() == seatPos)
		bSeatTaken = true;
}

void SceneAssignment1::FreeSeat(int index, Vector3 seatPos, bool &bSeatTaken)
{
	if (entityMgr->GetEntityMap()[index]->GetSeatPosition() == seatPos)
		bSeatTaken = false;
}

void SceneAssignment1::CreateFlock(Vector3 seat_pos)
{
    const int MAX_CUSTOMERS = 3;  // max in a group, can randomise later
    Vector3 start_pos(145, 10, 0);
    for (int i = 0; i < MAX_CUSTOMERS; ++i)
    {
        const int away_distance = 20; // distance away from the previous group
        CCustomer* theCustomer;
        if (i == 0)
            theCustomer = new CCustomer(entityMgr->GetNextID(), seat_pos, true, start_pos);
        else
        {
            start_pos.x += i * i;
            theCustomer = new CCustomer(entityMgr->GetNextID(), 0, false, start_pos);
        }
        if (customer_list.size() > 0)
        {
            if (customer_list[0]->GetStateInText() == "Queue up") // if first customer is queueing up, line up at back of queue
            {
                Vector3 behind_pos = customer_list.back()->waypoints[0]; // this assumes that the latest customer is queueing up as well and that the new customer should line up behind him by away_distance
                behind_pos.x += away_distance;
                theCustomer->waypoints[0] = behind_pos;
            }
            else // nobody is queueing up
                theCustomer->waypoints[0] = ENTRANCE;
        }
        
        theCustomer->group_num = static_cast<Group>(flock_to_join); // customer is assigned a flock to join
        theCustomer->num_in_group = MAX_CUSTOMERS;

        entityMgr->RegisterEntity(theCustomer);
        customer_list.push_back(theCustomer);
        flock_list.at(flock_to_join).push_back(theCustomer);
    }
    
    flock_to_join++; // the max number of customers in a flock has been reached, the next time this function is called, the new customers will join the next flock
    if (flock_to_join > Group::GROUP_MAX)
        flock_to_join = 0; // Sorta loops but doesnt check back to see if the previous groups has left, so i am going to make the customers stops coming if flock to join is MAX
}

void SceneAssignment1::GenerateCustomers()
{
	/*
	Eg. 4 customers in a group
	The first customer to be inserted into the entity map is assigned the leader role
	The leader is assigned an ID, the rest are assigned an ID of -1
	The other customers will also be inserted into the entity map but they will just follow
	the leader
	Only the leader can send messages and return trays
	Need to call SetLeaderStatus(true) for leaders
	*/

	static bool bSeat1Taken = false;
	static bool bSeat2Taken = false;
	static bool bSeat3Taken = false;
	static bool bSeat4Taken = false;
	static bool bSeat5Taken = false;
	static bool bSeat6Taken = false;
	static bool bSeat7Taken = false;
	static bool bSeat8Taken = false;
	static bool bSeat9Taken = false;

	static unsigned int seatNum = 0;
	static bool bSeatSelected = false;
	Vector3 theSeatPos;

	for (int i = entityMgr->GetStillHereID(); i <= entityMgr->GetLatestID(); i++)
	{
		if (entityMgr->GetEntityFromID(i)->GetDoneStatus() == false)
		{
			CloseSeat(i, SEAT_1, bSeat1Taken);
			CloseSeat(i, SEAT_2, bSeat2Taken);
			CloseSeat(i, SEAT_3, bSeat3Taken);
			CloseSeat(i, SEAT_4, bSeat4Taken);
			CloseSeat(i, SEAT_5, bSeat5Taken);
			CloseSeat(i, SEAT_6, bSeat6Taken);
			CloseSeat(i, SEAT_7, bSeat7Taken);
			CloseSeat(i, SEAT_8, bSeat8Taken);
			CloseSeat(i, SEAT_9, bSeat9Taken);
		}
		else
		{
			FreeSeat(i, SEAT_1, bSeat1Taken);
			FreeSeat(i, SEAT_2, bSeat2Taken);
			FreeSeat(i, SEAT_3, bSeat3Taken);
			FreeSeat(i, SEAT_4, bSeat4Taken);
			FreeSeat(i, SEAT_5, bSeat5Taken);
			FreeSeat(i, SEAT_6, bSeat6Taken);
			FreeSeat(i, SEAT_7, bSeat7Taken);
			FreeSeat(i, SEAT_8, bSeat8Taken);
			FreeSeat(i, SEAT_9, bSeat9Taken);
		}
	}
    

    if (!bSeat1Taken || !bSeat2Taken || !bSeat3Taken || !bSeat4Taken || !bSeat5Taken || !bSeat6Taken || !bSeat7Taken || !bSeat8Taken || !bSeat9Taken)
    {
        if ((rand() % 500 + 1) == 1)
        {
            if (bSeat1Taken == false)
            {
                theSeatPos = SEAT_1;
            }
            else if (bSeat2Taken == false)
            {
                theSeatPos = SEAT_2;
            }
            else if (bSeat3Taken == false)
            {
                theSeatPos = SEAT_3;
            }
            else if (bSeat4Taken == false)
            {
                theSeatPos = SEAT_4;
            }
            else if (bSeat5Taken == false)
            {
                theSeatPos = SEAT_5;
            }
            else if (bSeat6Taken == false)
            {
                theSeatPos = SEAT_6;
            }
            else if (bSeat7Taken == false)
            {
                theSeatPos = SEAT_7;
            }
            else if (bSeat8Taken == false)
            {
                theSeatPos = SEAT_8;
            }
            else if (bSeat9Taken == false)
            {
                theSeatPos = SEAT_9;
            }

            if (flock_to_join < Group::GROUP_MAX)
                CreateFlock(theSeatPos);

			// Previous way
			const int away_distance = 20;
			CCustomer* theCustomer = new CCustomer(entityMgr->GetNextID(), theSeatPos, true);
			if (customer_list.size() > 0)
			{
				if (customer_list[0]->GetStateInText() == "Queue up") // if first customer is queueing up, line up at back of queue
				{
					Vector3 behind_pos = customer_list.back()->waypoints[0]; // this assumes that the latest customer is queueing up as well and that the new customer should line up behind him by away_distance
					behind_pos.x += away_distance;
					theCustomer->waypoints[0] = behind_pos;
				}
				else // nobody is queueing up
					theCustomer->waypoints[0] = ENTRANCE;
			}

			entityMgr->RegisterEntity(theCustomer);
			customer_list.push_back(theCustomer);
        }
    }


	//std::cout << "theCustomer's ID: " << theCustomer->GetID() << std::endl;
	//std::cout << seatNum << std::endl;
}

bool SceneAssignment1::CheckIfCustomerReachDestination()
{
    bool hasReached = true;
    for (int i = 0; i < customer_list.size(); ++i)
    {
        if (customer_list[i]->GetPosition() != customer_list[i]->waypoints[0])
            hasReached = false;
    }
    return hasReached;
}

void SceneAssignment1::calculateCOM(std::vector<CCustomer*> list, CCustomer& entity, Group group)
{
    float total_x = 0.0f; // summation of all x coordinates of flock // this are the numbers to change for neural net, aka (w1, w2) from lecture
    float total_y = 0.0f; // summation of all y coordinates of flock
    for (int i = 0; i < list.size(); ++i)
    {
        if (list[i] != &entity && list[i]->group_num == group)
        {
            if ((list[i]->GetPosition() - entity.GetPosition()).LengthSquared() < 25)
            {
                total_x += list[i]->GetPosition().x;
                total_y += list[i]->GetPosition().y;
            }
        }
    }
    entity.centre_of_mass.x = total_x;
    entity.centre_of_mass.y = total_y;

    entity.centre_of_mass.x /= list.size();
    entity.centre_of_mass.y /= list.size();

    //entity.centre_of_mass = entity.centre_of_mass - entity.position;
    //entity.centre_of_mass.Normalize();
}

void SceneAssignment1::calculateRepelVec(std::vector<CCustomer*> list, CCustomer& entity, float dt)
{
    float radiusSquared = 5 * 5; // hardcoded, scale value is from RenderEntities(), scale of customer sprite
    Vector3 repelVec;
    float dist_away; 
        for (int i = 0; i < list.size(); ++i)
        {
            if (list[i] != &entity && list[i]->group_num == entity.group_num)
            {
                repelVec = entity.GetPosition() - list[i]->GetPosition();
                dist_away = repelVec.LengthSquared();
                //repelVec.x = (entity.GetPosition().x - list[i]->GetPosition().x) * (entity.GetPosition().x - list[i]->GetPosition().x);
                //repelVec.y = (entity.GetPosition().y - list[i]->GetPosition().y) * (entity.GetPosition().y - list[i]->GetPosition().y);
                bool isTooClose = (dist_away < radiusSquared);
                //if (dist_away == 0)
                //{
                //    entity.position = (entity.position - (radiusSquared * dt));
                //    repelVec = entity.GetPosition() - list[i]->GetPosition();
                //}
                if (isTooClose)
                {
                    if (repelVec != Vector3(0, 0, 0))
                    {
                        //repelVec *= -1;
                        //repelVec.Normalize();
                        entity.m_repelVec = repelVec;
                    }
                }
                //else if (dist_away != 0)
                //{
                //    entity.GetFSM()->ChangeState(CState_Customer_Idle::GetInstance(), dt);
                //}
            }
        }
}

void calculateRepelVec(CCustomer& customer, CWaiter& waiter, float dt)
{

}

void SceneAssignment1::Update(double dt)
{
	SceneBase::Update(dt);
	dt *= m_speed;

	//std::cout << "size: "<< entityMgr->GetEntityMap().size() << std::endl;

	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	CMessageDispatcher::GetInstance()->DispatchDelayedMessages();

	// Generate customers
	/*static bool vButtonState = false;
	if (!vButtonState && Application::IsKeyPressed('V'))
	{
		vButtonState = true;
		GenerateCustomers();
	}
	else if (vButtonState && !Application::IsKeyPressed('V'))
		vButtonState = false;*/

    GenerateCustomers();

    if (/*CheckIfCustomerReachDestination() &&*/ customer_list.size() >= 5) // if more than 5 customers are present in the scene, then flock together
    {
        for (int i = 0; i < customer_list.size(); ++i)
        {
            //if (customer_list[i]->m_repelVec == Vector3(0, 0, 0))
            calculateCOM(customer_list, *customer_list[i], customer_list[i]->group_num);
            calculateRepelVec(customer_list, *customer_list[i], dt);

        }
    }
    //for (int i = 0; i < customer_list.size(); ++i)
    {
        //if (customer_list[i]->GetLeaderStatus() == false)
    }

    for (int i = 0; i < customer_list.size(); ++i)
    {
        if (customer_list[i]->position.x > m_worldWidth)
            customer_list[i]->position.x -= (10 * dt);
        else if (customer_list[i]->position.x < 0)
            customer_list[i]->position.x += (10 * dt);
        if (customer_list[i]->position.y > m_worldHeight)
            customer_list[i]->position.y -= (10 * dt);
        else if (customer_list[i]->position.y < 0)
            customer_list[i]->position.y += (10 * dt);
    }

	// Update customers
	for (int i = entityMgr->GetStillHereID(); i <= entityMgr->GetLatestID(); i++)
	{
		// other way: when exited, remove from entity manager
		// bad way: make a virtual function to set and get exited status just for customer's use, since cannot use dynamic_cast
		// when exited == true, don't update and render
		if (entityMgr->GetEntityFromID(i)->GetExitStatus() == false)
			entityMgr->GetEntityFromID(i)->Update(dt);
        else // customer has left, therefore kick him out of customer list, not working currently cuz this will never be called, customer list must be passed to state_leave
        {
            if (customer_list[i]->GetID() == entityMgr->GetEntityFromID(i)->GetID())
            {
                std::vector<CCustomer*>::iterator newEnd = std::remove(customer_list.begin(), customer_list.end(), dynamic_cast<CCustomer*>(entityMgr->GetEntityFromID(i)));
                customer_list.erase(newEnd, customer_list.end());
            }
        }
	}

	if (customer_list.size() > 5)
		waiter->need_help = true;
	else
		waiter->need_help = false;

	//customer->Update(dt);
	waiter->Update(dt);
    usher->Update(dt);
	chef->Update(dt);
	cleaner->Update(dt);

	// For Debugging
	if (Application::IsKeyPressed(VK_UP))
		debugPos.y += 20 * dt;
	if (Application::IsKeyPressed(VK_DOWN))
		debugPos.y -= 20 * dt;
	if (Application::IsKeyPressed(VK_LEFT))
		debugPos.x -= 20 * dt;
	if (Application::IsKeyPressed(VK_RIGHT))
		debugPos.x += 20 * dt;

	// Test
	//m_Grid->GetNodeFromWorldPoint(debugPos)->SetWalkable(false);
	//pathfinder->FindPath(Vector3(10, 2), Vector3(50, 50));

	for (int x = 0; x < pathfinder->GetGrid()->GetGridSizeX(); x++)
	{
		for (int y = 0; y < pathfinder->GetGrid()->GetGridSizeY(); y++)
		{
			if (pathfinder->GetGrid()->Contains(pathfinder->GetGrid()->GetPath(), &pathfinder->GetGrid()->Get()[x][y]))
			{
				pathfinder->GetGrid()->Get()[x][y].SetWalkable(false);
			}
		}
	}
}

void SceneAssignment1::AddSeatsToList()
{
	seatList.push_back(SEAT_1);
	seatList.push_back(SEAT_2);
	seatList.push_back(SEAT_3);
	seatList.push_back(SEAT_4);
	seatList.push_back(SEAT_5);
	seatList.push_back(SEAT_6);
	seatList.push_back(SEAT_7);
	seatList.push_back(SEAT_8);
	seatList.push_back(SEAT_9);
}

void SceneAssignment1::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_WALL:
	{
		float degree = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(degree, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_PILLAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PILLAR], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HOLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAssignment1::RenderLocationNames()
{
	std::ostringstream ss;
	ss.str("");
	ss << "Kitchen";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 13, 50);

	ss.str("");
	ss << "Store";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 37, 52);
	ss.str("");
	ss << "room";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 37.5, 50);

	ss.str("");
	ss << "Trash bin";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 50, 50);

	ss.str("");
	ss << "Toilet";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 68, 50);

	ss.str("");
	ss << "Dining Area";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 35, 28);
}

void SceneAssignment1::RenderSeat(Vector3 position, unsigned int side)
{
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);

	if (side == 0)
		modelStack.Rotate(0, 0, 0, 1);
	else if (side == 1)
		modelStack.Rotate(90, 0, 0, 1);
	else if (side == 2)
		modelStack.Rotate(180, 0, 0, 1);

	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_SEAT], false);
	modelStack.PopMatrix();
}

void SceneAssignment1::RenderRestaurant()
{
	//// left wall
	//modelStack.PushMatrix();
	//modelStack.Translate(leftWall->position.x, leftWall->position.y, 0);
	//modelStack.Scale(leftWall->scale.x, leftWall->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	//// right wall
	//modelStack.PushMatrix();
	//modelStack.Translate(rightWall->position.x, rightWall->position.y, 0);
	//modelStack.Scale(rightWall->scale.x, rightWall->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	//// front wall
	//modelStack.PushMatrix();
	//modelStack.Translate(frontWall->position.x, frontWall->position.y, 0);
	//modelStack.Scale(frontWall->scale.x, frontWall->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	//// back wall
	//modelStack.PushMatrix();
	//modelStack.Translate(backWall->position.x, backWall->position.y, 0);
	//modelStack.Scale(backWall->scale.x, backWall->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	//// kitchen
	//modelStack.PushMatrix();
	//modelStack.Translate(kitchen->position.x, kitchen->position.y, 0);
	//modelStack.Scale(kitchen->scale.x, kitchen->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	//// divider 1
	//modelStack.PushMatrix();
	//modelStack.Translate(divider1->position.x, divider1->position.y, 0);
	//modelStack.Scale(divider1->scale.x, divider1->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	//// divider 2
	//modelStack.PushMatrix();
	//modelStack.Translate(divider2->position.x, divider2->position.y, 0);
	//modelStack.Scale(divider2->scale.x, divider2->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	//// divider 3
	//modelStack.PushMatrix();
	//modelStack.Translate(divider3->position.x, divider3->position.y, 0);
	//modelStack.Scale(divider3->scale.x, divider3->scale.y, 0);
	//RenderMesh(meshList[GEO_WALL], false);
	//modelStack.PopMatrix();

	for (std::vector<Furniture*>::iterator it = entityMgr->GetFurnitureList()->begin(); it != entityMgr->GetFurnitureList()->end(); it++)
	{
		Furniture* furniture = (Furniture*)*it;
		modelStack.PushMatrix();
		modelStack.Translate(furniture->position.x, furniture->position.y, 0);
		modelStack.Scale(furniture->scale.x, furniture->scale.y, 0);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
	}

	for (std::vector<CTable*>::iterator it = entityMgr->GetTableList()->begin(); it != entityMgr->GetTableList()->end(); it++)
	{
		CTable* table = (CTable*)*it;

		if (table->GetActive())
		{
			modelStack.PushMatrix();
			modelStack.Translate(table->GetPos().x, table->GetPos().y, 0);
			modelStack.Scale(20, 20, 1);
			RenderMesh(meshList[GEO_TABLE], false);
			modelStack.PopMatrix();

			for (int i = 0; i < table->GetSeatList()->size(); i++)
			{
				Vector3 seatPos = table->GetSeatList()->at(i);

				modelStack.PushMatrix();
				modelStack.Translate(seatPos.x, seatPos.y, 0);
				modelStack.Scale(3, 3, 1);
				RenderMesh(meshList[GEO_WALL], false);
				modelStack.PopMatrix();
			}
		}
	}

	// Seats
	RenderSeat(SEAT_1, 0);
	RenderSeat(SEAT_2, 0);
	RenderSeat(SEAT_3, 0);
	RenderSeat(SEAT_4, 1);
	RenderSeat(SEAT_5, 1);
	RenderSeat(SEAT_6, 1);
	RenderSeat(SEAT_7, 2);
	RenderSeat(SEAT_8, 2);
	RenderSeat(SEAT_9, 2);
}

void SceneAssignment1::RenderEntities()
{
	for (int i = entityMgr->GetStillHereID(); i <= entityMgr->GetLatestID(); i++)
	{
		if (entityMgr->GetEntityFromID(i)->GetExitStatus() == false)
		{
			modelStack.PushMatrix();
			modelStack.Translate(entityMgr->GetEntityFromID(i)->GetPosition().x, entityMgr->GetEntityFromID(i)->GetPosition().y, entityMgr->GetEntityFromID(i)->GetPosition().z);
			modelStack.Scale(10, 10, 10);
			if (entityMgr->GetEntityFromID(i)->GetSpriteInvertStatus() == false)
				RenderMesh(meshList[GEO_HAPPY_CUSTOMER], false);
			else
				RenderMesh(meshList[GEO_HAPPY_CUSTOMER_INVERT], false);
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(waiter->GetPosition().x, waiter->GetPosition().y, waiter->GetPosition().z);
	modelStack.Scale(11, 11, 11);
	if (waiter->GetSpriteInvertStatus() == false)
		RenderMesh(meshList[GEO_WAITER], false);
	else
		RenderMesh(meshList[GEO_WAITER_INVERT], false);
	modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(usher->GetPosition().x, usher->GetPosition().y, usher->GetPosition().z);
    modelStack.Scale(11, 11, 11);
    if (usher->GetSpriteInvertStatus() == false)
        RenderMesh(meshList[GEO_WAITER], false);
    else
        RenderMesh(meshList[GEO_WAITER_INVERT], false);
    modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(chef->GetPosition().x, chef->GetPosition().y, chef->GetPosition().z);
	modelStack.Scale(10, 10, 10);
	if (chef->GetSpriteInvertStatus() == false)
		RenderMesh(meshList[GEO_CHEF], false);
	else
		RenderMesh(meshList[GEO_CHEF_INVERT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(cleaner->GetPosition().x, cleaner->GetPosition().y, cleaner->GetPosition().z);
	modelStack.Scale(7, 10, 10);
	if (cleaner->GetSpriteInvertStatus() == false)
		RenderMesh(meshList[GEO_CLEANER], false);
	else
		RenderMesh(meshList[GEO_CLEANER_INVERT], false);
	modelStack.PopMatrix();

	// For debugging
	modelStack.PushMatrix();
	modelStack.Translate(debugPos.x, debugPos.y, debugPos.z);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BALL3], false);
	modelStack.PopMatrix();
}

void SceneAssignment1::RenderEntities_States()
{
    float x1 = -4.f + usher->GetPosition().x * 0.6f;
    float y1 = 3.f + usher->GetPosition().y * 0.6f;
    RenderTextOnScreen(meshList[GEO_TEXT], usher->GetStateInText(), Color(1, 1, 1), 2.5, x1, y1);

	float x2 = -4.f + waiter->GetPosition().x * 0.6f;
	float y2 = 3.f + waiter->GetPosition().y * 0.6f;
	RenderTextOnScreen(meshList[GEO_TEXT], waiter->GetStateInText(), Color(1, 1, 1), 2.5, x2, y2);

	float x3 = -4.f + chef->GetPosition().x * 0.6f;
	float y3 = 3.f + chef->GetPosition().y * 0.6f;
	RenderTextOnScreen(meshList[GEO_TEXT], chef->GetStateInText(), Color(1, 1, 1), 2.5, x3, y3);

	float x4 = -4.f + cleaner->GetPosition().x * 0.6f;
	float y4 = 2.5f + cleaner->GetPosition().y * 0.6f;
	RenderTextOnScreen(meshList[GEO_TEXT], cleaner->GetStateInText(), Color(1, 1, 1), 2.5, x4, y4);

	for (int i = entityMgr->GetStillHereID(); i <= entityMgr->GetLatestID(); i++)
	{
		if (entityMgr->GetEntityFromID(i)->GetExitStatus() == false)
		{
			float x = -4.f + entityMgr->GetEntityFromID(i)->GetPosition().x * 0.6f;
			float y = -0.5f + entityMgr->GetEntityFromID(i)->GetPosition().y * 0.6f;
			RenderTextOnScreen(meshList[GEO_TEXT], entityMgr->GetEntityFromID(i)->GetStateInText(), Color(1, 1, 1), 2.5, x, y);
		}
	}
}

void SceneAssignment1::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	/*for (int i = 0; i < gridList.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(gridList[i]->GetPosition().x, gridList[i]->GetPosition().y, 2);
		modelStack.Scale(0.5, 0.5, 0.5);
		if (gridList[i]->bCollided == false)
			RenderMesh(meshList[GEO_BALL], false);
		else
			RenderMesh(meshList[GEO_BALL2], false);
		modelStack.PopMatrix();
	}*/

	for (int x = 0; x < pathfinder->GetGrid()->GetGridSizeX(); x++)
	{
		for (int y = 0; y < pathfinder->GetGrid()->GetGridSizeY(); y++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(pathfinder->GetGrid()->Get()[x][y].GetPosition().x, pathfinder->GetGrid()->Get()[x][y].GetPosition().y, 2);
			modelStack.Scale(0.5, 0.5, 0.5);
			if (pathfinder->GetGrid()->Get()[x][y].GetWalkable() == true)
				RenderMesh(meshList[GEO_BALL], false);
			else
				RenderMesh(meshList[GEO_BALL2], false);
			modelStack.PopMatrix();
		}
	}

	//On screen text
	std::ostringstream ss;

	RenderLocationNames();

	ss.str("");
	ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 55);

	//RenderTextOnScreen(meshList[GEO_TEXT], "Restaurant", Color(0, 1, 0), 3, 0, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(/*chef->GetOrderList()->size()*/entityMgr->GetLatestID()), Color(0, 1, 0), 3, 0, 0);

	RenderTextOnScreen(meshList[GEO_TEXT], "debugPos: (" + std::to_string(debugPos.x) + ", " + std::to_string(debugPos.y) + ")", Color(0, 1, 0), 3, 20, 3);

	RenderEntities();
	RenderRestaurant();
	RenderEntities_States();
}

void SceneAssignment1::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}

	// Delete customers // dis is wrong for now
	/*for (int i = 4; i < entityMgr->GetEntityMap().size(); i++)
	{
		entityMgr->GetEntityMap()[i] = NULL;
		delete entityMgr->GetEntityMap()[i];
	}*/

	entityMgr->Exit();

	if (waiter)
	{
		waiter = NULL;
		delete waiter;
	}

    if (usher)
    {
        usher = NULL;
        delete usher;
    }

	if (chef)
	{
		chef = NULL;
		delete chef;
	}

	if (cleaner)
	{
		cleaner = NULL;
		delete cleaner;
	}

	if (pathfinder)
	{
		pathfinder = NULL;
		delete pathfinder;
	}
}
