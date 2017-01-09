#include "SceneAssignment1.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "../Source/Rayner/MyTimer.h"
#include "../Source/Rayner/EntityNames.h"
#include "../Source/Rayner/Messaging/MessageDispatcher.h"
#include "../Source/Rayner/Locations.h"

CMyTimer* clock_ = CMyTimer::GetInstance();

SceneAssignment1::SceneAssignment1()
{
}

SceneAssignment1::~SceneAssignment1()
{
}

/*SAMUEL AI*/
static void ErrorCallBack(int error, const char*description)
{
    fputs(description, stderr);
    _fgetchar();
}

bool IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

void RenderFillCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b)
{
    int n = 360;
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= n; i++)
    {
        float angle = (float)(i * (2.0 * 3.14159 / n));
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

void RenderLine(GLfloat xStart, GLfloat yStart, GLfloat xEnd, GLfloat yEnd, GLfloat r, GLfloat g, GLfloat b)
{
    //Vertex v;
    //std::vector<Vertex> vertex_buffer_data;
    //v.pos.Set(-1000, 0, 0);
    //v.color.Set(1, 0, 0);
    //vertex_buffer_data.push_back(v);
    //v.pos.Set(1000, 0, 0);
    //v.color.Set(1, 0, 0);
    //vertex_buffer_data.push_back(v);
    //v.pos.Set(0, -1000, 0);
    //v.color.Set(0, 1, 0);
    //vertex_buffer_data.push_back(v);
    //v.pos.Set(0, 1000, 0);
    //v.color.Set(0, 1, 0);
    //vertex_buffer_data.push_back(v);
    //v.pos.Set(0, 0, -1000);
    //v.color.Set(0, 0, 1);
    //vertex_buffer_data.push_back(v);
    //v.pos.Set(0, 0, 1000);
    //v.color.Set(0, 0, 1);
    //vertex_buffer_data.push_back(v);

    //std::vector<GLuint> index_buffer_data;
    //index_buffer_data.push_back(0);
    //index_buffer_data.push_back(1);
    //index_buffer_data.push_back(2);
    //index_buffer_data.push_back(3);
    //index_buffer_data.push_back(4);
    //index_buffer_data.push_back(5);

    //Mesh *mesh = new Mesh(meshName);

    //glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    //glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    //mesh->indexSize = index_buffer_data.size();
    //mesh->mode = Mesh::DRAW_LINES;

    //return mesh;
    glLineWidth(10.5);
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex3f(xStart, yStart, yStart);
    glVertex3f(xEnd, yEnd, yEnd);
    glEnd();
}

int RandomInteger(int lowerLimit, int upperLimit)
{
    return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

void fillWaypoint(const MyVector min, const MyVector max, map<const int, MyVector>& container, const int containerSize) // takes two diagonal vertice of an imaginary rectangle and fill the area with waypoints
{
    float xmin = min.x; int xmax = max.x; int ymin = min.y; int ymax = max.y;
    for (int i = 0; i < containerSize; ++i)
    {
        container[i].SetPosition(RandomInteger(xmin, xmax), RandomInteger(ymin, ymax));
        for (int j = 0; j < i; ++j)
            while (container[i] != container[j] == false)
            {
                container[i].SetPosition(RandomInteger(xmin, xmax), RandomInteger(ymin, ymax));
            } // change my vector to float or else numerous combinations of table coordinates is impossible, alternatively, increase min and max for more space
        //if (i == containerSize - 1)
        //    for (int j = 0; j < i; ++j)
        //        while (container[i] != container[j] == false)
        //        {
        //            container[i].SetPosition(RandomInteger(xmin, xmax), RandomInteger(ymin, ymax));
        //        }
        //if (container[containerSize - 1] != container[j + 1] == false)
        // container[containerSize - 1].SetPosition(RandomInteger(xmin, xmax), RandomInteger(ymin, ymax));
    }
}

void SceneAssignment1::createCustomer(float x, float y, float patienceMeter)
{
    S_Customer* fatJudge = new S_Customer();
    fatJudge->setPos(MyVector(x, y));
    fatJudge->setPatience(patienceMeter);
    customerList.push_back(fatJudge);
}

void SceneAssignment1::initFSM()
{
    createObject(0, MyVector(100, 50), -1, Obj::TABLE); // for chef
    createObject(10, MyVector(70, m_worldHeight * 0.75)); // for door
    ramsay->init(); // allow Ramsay to walk upon spawning
    s_waiter->init();
    s_waiter->collectArea = MyVector(100, 50);
    fillWaypoint(s_waiter->min, s_waiter->max, s_waiter->waypoints, s_waiter->getTables());
    map<const int, vector<GameObject>>::iterator it;
    int count = 0;
    vector<Obj> chefWaypoints = GOMap[0];
    vector<Obj> exits = GOMap.at(10);
    createCustomer(-3, 2, 75);
    createCustomer(-3, 2);
    //createCustomer(-3, 2);
    //createCustomer(-3, 2);
    //createCustomer(-3, 2);
    //createCustomer(-3, 2);
    vector<MyVector> customerStack;
    customerStack.push_back(exits[0].getPos());

    vector<Obj>::iterator it2;
    for (it2 = chefWaypoints.begin(); it2 != chefWaypoints.end(); it2++)
    {
        if (ramsay->currState == ramsay->IDLE)
        {
            ramsay->waypoints[count].SetPosition((*it2).getPos().x, (*it2).getPos().y);
        }
        ++count;
    }

    for (int i = 0; i < customerList.size(); i++)
    {
        customerList[i]->init();
        if (customerList[i]->state == S_Customer::LINEUP)
        {
            customerList[i]->waypoints[0].SetPosition(customerStack[customerStack.size() - 1].GetX(), customerStack[customerStack.size() - 1].GetY() + 20); // the latest one pushed to the stack is the one that customer follows
            customerStack.push_back(MyVector(customerStack[customerStack.size() - 1].GetX(), customerStack[customerStack.size() - 1].GetY() + 20));
            customerList[i]->hungryChance = RandomInteger(1, 100);
            if (customerList[i]->hungryChance > 25)
                customerList[i]->status = S_Customer::YES;
            else
                customerList[i]->status = S_Customer::NO;
        }
        ++count;
    }
    //GOMap.insert(std::pair<const int, GameObject>(0, new GameObject()));
}

int SceneAssignment1::createObject(const unsigned int ID, MyVector pos, int existingID, Obj::OBJTYPE type)
{
    vector<Obj> insertion;
    Obj obj;
    obj.setPos(pos);
    obj.objType = type;
    insertion.push_back(obj);
    if (existingID < 0)
    {
        GOMap.insert(std::pair<const int, vector<Obj>>(ID, insertion));
        return existingID;
    }
    GOMap.insert(std::pair<const int, vector<Obj>>(ID, insertion));
    if (ID == -1) // if default parameter value is returned, help user find a key to use
    {
        int count = 0;
        while (GOMap.count(count) != 0)
            ++count;
        //GOMap.insert(std::pair<const int, GameObject>(count, obj));
        GOMap.insert(std::pair<const int, vector<Obj>>(ID, insertion));
    }
    if (GOMap.count(ID) != 0) // each object has its own unique key
        return 0;
    GOMap.insert(std::pair<const int, vector<Obj>>(ID, insertion));
    return ID;
}

void SceneAssignment1::renderObjects()
{
    map<const int, vector<Obj>>::iterator it;
    for (it = GOMap.begin(); it != GOMap.end(); it++)
    {
        vector<Obj>::iterator it2;
        for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            if (it2->objType == Obj::TABLE)
            {
                modelStack.PushMatrix();
                modelStack.Translate(it2->getPos().x, it2->getPos().y, -5);
                /*RenderFillCircle(0.0f, 0.0f, 0.2, 1.0f, 0.0f, 1.0f);*/
                modelStack.Scale(7.5f, 15, 1);
                RenderMesh(meshList[GEO_TABLE], false);
                modelStack.PopMatrix();
            }
            else if (it2->objType == Obj::DOOR)
            {
                modelStack.PushMatrix();
                modelStack.Translate(it2->getPos().x, it2->getPos().y, -4);
                /*RenderFillCircle(0.0f, 0.0f, 0.2, 1.0f, 0.0f, 1.0f);*/
                modelStack.Scale(3, 22, 3);
                RenderMesh(meshList[GEO_DOOR], false);
                modelStack.PopMatrix();
            }
        }
    }
}

void SceneAssignment1::CollaborateWaiterCustomer() // collaboration between two AI FSM in a container
{
    for (unsigned int i = 0; i < customerList.size(); ++i)
    {
        if (customerList[i]->hungryChance >= 25 && customerList[i]->state == customerList[i]->IDLE && (s_waiter->state == s_waiter->IDLE || s_waiter->state == s_waiter->TAKE_ORDER))
        {
            s_waiter->servingOrder[s_waiter->num_serve] = i;

            ++s_waiter->num_serve;
            /* Another way to make customer move but he jitters because float values
            //MyVector direction = (customerList[i]->getPos() - s_waiter->waypoints.at(i)).Normalize();
            //MyVector go = customerList[i]->getPos() + direction * (double)0.013f;
            //customerList[i]->setPos(go);*/
            customerList[i]->waypoints.at(1) = s_waiter->waypoints.at(i);
            customerList[i]->setTableNum(i);
        }
        else if (customerList[i]->hungryChance >= 25 && customerList[i]->state == customerList[i]->LEAVE && customerList[i]->status == customerList[i]->ANGRY)
        {
            s_waiter->servingOrder[i] = -1;
            s_waiter->num_serve--;
            customerList[i]->status = customerList[i]->MAX_STATUS;
        }
        if (customerList[i]->getPatience() <= 50)
            s_waiter->goFast = true;
    }
    //if (s_waiter->num_serve > 0 && s_waiter->state == s_waiter->IDLE)
    //{
    //    for (int i = 0; i < s_waiter->num_serve; ++i)
    //    {
    //        s_waiter->waypoints.at(i) = 
    //    }
    //}
}

void SceneAssignment1::CollaborateWaiterCook() // collaboration between two AI FSM in a container
{
    //if ((s_waiter->state == s_waiter->IDLE || s_waiter->state == s_waiter->TAKE_ORDER) && s_waiter->getTables() == 0)
    //{

    //}
}

void SceneAssignment1::CollaborateWaiterCustomerCook() // collaboration between two AI FSM in a container
{
    if (s_waiter->getTables() == 0)
    {
        for (int i = 0; i < s_waiter->num_serve; ++i)
        {
            if (customerList[i]->hungryChance > 25 && customerList[i]->isReady == false)
            {
                s_waiter->isCustomerReady = false;
                return;
            }
        }
        if ((s_waiter->getPos() != s_waiter->waypoints[s_waiter->servingOrder[s_waiter->serving_num]]) == false)
        {
            ramsay->setHasOrder(true);
            if (ramsay->cookTime <= 0)
                s_waiter->collect = true;
        }
        else
            s_waiter->isCustomerReady = true;
    }
}
/**/

void SceneAssignment1::Init()
{
    srand((unsigned)time(NULL));

	SceneBase::Init();

	entityMgr = CEntityManager::GetInstance();

	waiter = new CWaiter(ENT_WAITER);
	entityMgr->RegisterEntity(waiter);

	chef = new CChef(ENT_CHEF);
	entityMgr->RegisterEntity(chef);

	cleaner = new CCleaner(ENT_CLEANER);
	entityMgr->RegisterEntity(cleaner);

	customer = new CCustomer(entityMgr->GetEntityMap().size(), SEAT_1, true);
	entityMgr->RegisterEntity(customer);

	// For Debugging
	debugPos = Vector3(0, -10, 0);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;

	//Calculating aspect ratio
	/*m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();*/

	m_worldHeight = 190.f;
	m_worldWidth = m_worldHeight * (float) 133  / 190;

    /*SAMUELAI*/
    ramsay = new Chef();
    s_waiter = new Waiter(MyVector(5, 5.f), MyVector(m_worldWidth - 5, m_worldHeight * 0.7f));
    initFSM();
    /**/
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

	for (int i = 3; i < entityMgr->GetEntityMap().size(); i++)
	{
		if (entityMgr->GetEntityMap()[i]->GetDoneStatus() == false)
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

	if (bSeat1Taken == false)
		theSeatPos = SEAT_1;
	else if (bSeat2Taken == false)
		theSeatPos = SEAT_2;
	else if (bSeat3Taken == false)
		theSeatPos = SEAT_3;
	else if (bSeat4Taken == false)
		theSeatPos = SEAT_4;
	else if (bSeat5Taken == false)
		theSeatPos = SEAT_5;
	else if (bSeat6Taken == false)
		theSeatPos = SEAT_6;
	else if (bSeat7Taken == false)
		theSeatPos = SEAT_7;
	else if (bSeat8Taken == false)
		theSeatPos = SEAT_8;
	else if (bSeat9Taken == false)
		theSeatPos = SEAT_9;

	
		if (!bSeat1Taken || !bSeat2Taken || !bSeat3Taken || !bSeat4Taken || !bSeat5Taken || !bSeat6Taken || !bSeat7Taken || !bSeat8Taken || !bSeat9Taken)
		{
			if ((rand() % 500 + 1) == 1)
			{
				CCustomer* theCustomer = new CCustomer(entityMgr->GetEntityMap().size(), theSeatPos, true);
				entityMgr->RegisterEntity(theCustomer);
			}
		}
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

	// Update customers
	for (int i = 3; i < entityMgr->GetEntityMap().size(); i++)
	{
		// other way: when exited, remove from entity manager
		// bad way: make a virtual function to set and get exited status just for customer's use, since cannot use dynamic_cast
		// when exited == true, don't update and render
		if (entityMgr->GetEntityMap()[i]->GetExitStatus() == false)
			entityMgr->GetEntityMap()[i]->Update(dt);
	}

	//customer->Update(dt);
	waiter->Update(dt);
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

    /*SAMUELAI*/
    ramsay->runFSM();
    ramsay->update(dt);

    for (int i = 0; i < customerList.size(); ++i)
    {
        //customerList[i]->setTableNum(s_waiter->getTables());
        if (customerList[i]->hungryChance > 25)
            customerList[i]->runFSM();
        if (customerList[i]->hungryChance <= 25)
            customerList[i]->runNotHungryFSM();
        customerList[i]->update(dt);
        if (customerList[i]->state == customerList[i]->LEAVE && customerList[i]->getPos() != customerList[i]->waypoints[0] == false)
        {
            //S_Customer* temp = customerList[i];
            customerList.pop_back();
            //delete temp;
        }
    }
    if (customerList.size() == 0)
    {
        vector<Obj> exits = GOMap.at(10);
        vector<MyVector> customerStack;
        customerStack.push_back(exits[0].getPos());
        createCustomer(-3, 2, 60);
        createCustomer(-3, 2);
        createCustomer(-3, 2, 65);
        for (int i = 0; i < customerList.size(); i++)
        {
            customerList[i]->init();
            if (customerList[i]->state == S_Customer::LINEUP)
            {
                customerList[i]->waypoints[0].SetPosition(customerStack[customerStack.size() - 1].GetX(), customerStack[customerStack.size() - 1].GetY() + 20); // the latest one pushed to the stack is the one that customer follows
                customerStack.push_back(MyVector(customerStack[customerStack.size() - 1].GetX(), customerStack[customerStack.size() - 1].GetY() + 20));
                customerList[i]->hungryChance = RandomInteger(1, 100);
                if (customerList[i]->hungryChance > 25)
                    customerList[i]->status = S_Customer::YES;
                else
                    customerList[i]->status = S_Customer::NO;
            }
        }
        s_waiter->isCustomerReady = false;
        s_waiter->num_serve = 0;
        ramsay->cookTime = 5.f;
        ramsay->currState = ramsay->IDLE;
        ramsay->setHasOrder(false);
    }
    s_waiter->runFSM();
    s_waiter->update(dt);
    CollaborateWaiterCustomer();
    //CollaborateWaiterCook();
    CollaborateWaiterCustomerCook();
    /**/
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
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 20, 25.5);

	ss.str("");
	ss << "Trash bin";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 50, 25.5);

	ss.str("");
	ss << "Toilet";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 68, 25.5);

	ss.str("");
	ss << "Dining Area";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 35, 14);
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
	// left wall
	modelStack.PushMatrix();
	modelStack.Translate(1.5, 60, 0);
	modelStack.Scale(3, 85, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// right wall
	modelStack.PushMatrix();
	modelStack.Translate(132, 60, 0);
	modelStack.Scale(3, 85, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// front wall
	modelStack.PushMatrix();
	modelStack.Translate(66, 99, 0);
	modelStack.Scale(130, 3, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// back
	modelStack.PushMatrix();
	modelStack.Translate(81, 19, 0);
	modelStack.Scale(100, 3, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// kitchen
	modelStack.PushMatrix();
	modelStack.Translate(40, 75, 0);
	modelStack.Scale(80, 3, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// divider 1
	modelStack.PushMatrix();
	modelStack.Translate(80, 87, 0);
	modelStack.Scale(3, 27, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// divider 2
	modelStack.PushMatrix();
	modelStack.Translate(105, 87, 0);
	modelStack.Scale(3, 27, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

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
	for (int i = 3; i < entityMgr->GetEntityMap().size(); i++)
	{
		if (entityMgr->GetEntityMap()[i]->GetExitStatus() == false)
		{
			modelStack.PushMatrix();
			modelStack.Translate(entityMgr->GetEntityMap()[i]->GetPosition().x, entityMgr->GetEntityMap()[i]->GetPosition().y, entityMgr->GetEntityMap()[i]->GetPosition().z);
			modelStack.Scale(10, 10, 10);
			if (entityMgr->GetEntityMap()[i]->GetSpriteInvertStatus() == false)
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

	// For Debugging
	modelStack.PushMatrix();
	modelStack.Translate(debugPos.x, debugPos.y, debugPos.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_BALL], false);
	modelStack.PopMatrix();
}

void SceneAssignment1::RenderEntities_States()
{
	float x2 = -4.f + waiter->GetPosition().x * 0.6f;
	float y2 = 1.7f + waiter->GetPosition().y * 0.3f;
	RenderTextOnScreen(meshList[GEO_TEXT], waiter->GetStateInText(), Color(1, 1, 1), 2.5, x2, y2);

	float x3 = -4.f + chef->GetPosition().x * 0.6f;
	float y3 = -23.3f + chef->GetPosition().y * 0.6f;
	RenderTextOnScreen(meshList[GEO_TEXT], chef->GetStateInText(), Color(1, 1, 1), 2.5, x3, y3);

	float x4 = -4.f + cleaner->GetPosition().x * 0.6f;
	float y4 = 1.8f + cleaner->GetPosition().y * 0.3f;
	RenderTextOnScreen(meshList[GEO_TEXT], cleaner->GetStateInText(), Color(1, 1, 1), 2.5, x4, y4);

	for (int i = 3; i < entityMgr->GetEntityMap().size(); i++)
	{
		if (entityMgr->GetEntityMap()[i]->GetExitStatus() == false)
		{
			float x = -4.f + entityMgr->GetEntityMap()[i]->GetPosition().x * 0.6f;
			float y = 1.5f + entityMgr->GetEntityMap()[i]->GetPosition().y * 0.3f;
			RenderTextOnScreen(meshList[GEO_TEXT], entityMgr->GetEntityMap()[i]->GetStateInText(), Color(1, 1, 1), 2.5, x, y);
		}
	}
}

void SceneAssignment1::RenderRayner()
{
    RenderLocationNames();
    //On screen text
    std::ostringstream ss;
    ss.str("");
    ss.precision(3);
    ss << "Speed: " << m_speed;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

    ss.str("");
    ss.precision(5);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

    //RenderTextOnScreen(meshList[GEO_TEXT], "Restaurant", Color(0, 1, 0), 3, 0, 0);
    RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(chef->GetOrderList()->size()), Color(0, 1, 0), 3, 0, 0);

    RenderTextOnScreen(meshList[GEO_TEXT], "debugPos: (" + std::to_string(debugPos.x) + ", " + std::to_string(debugPos.y) + ")", Color(0, 1, 0), 3, 20, 3);

    RenderEntities();
    RenderRestaurant();
    RenderEntities_States();
}

void SceneAssignment1::RenderSamuel()
{
    //On screen text
    std::ostringstream ss;
    /*SAMUELAI*/
    modelStack.PushMatrix();
    modelStack.Translate(0, m_worldHeight * 0.75f, -5);
    modelStack.Scale(145, 4, 1);
    RenderMesh(meshList[GEO_CUBE], false);
    modelStack.PopMatrix();

    renderObjects();

    modelStack.PushMatrix();
    modelStack.Translate(ramsay->getPos().x, ramsay->getPos().y, -7.7);
    modelStack.Scale(15, 20, 15);
    RenderMesh(meshList[GEO_CHEF], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(s_waiter->getPos().x, s_waiter->getPos().y, -7.6);
    modelStack.Scale(15, 20, 15);
    RenderMesh(meshList[GEO_WAITER], false);
    modelStack.PopMatrix();

    for (int i = 0; i < s_waiter->waypoints.size(); ++i)
    {
        if (s_waiter->reachDes[i] == true)
        {
            map<const int, MyVector> tablepos = s_waiter->waypoints;
            modelStack.PushMatrix();
            modelStack.Translate(tablepos.at(i).GetX(), tablepos.at(i).GetY(), 0);
            modelStack.Scale(7.5f, 15, 15);
            RenderMesh(meshList[GEO_TABLE], false);
            modelStack.PopMatrix();
        }
    }

    for (int i = 0; i < customerList.size(); ++i)
    {
        modelStack.PushMatrix();
        modelStack.Translate(customerList[i]->getPos().x, customerList[i]->getPos().y, 0);
        modelStack.Scale(4, 8, 4);
        if (customerList[i]->getPatience() > 0)
        {
            if (customerList[i]->hungryChance > 25)
                RenderMesh(meshList[GEO_CUSTOMER], false);
            else if (customerList[i]->hungryChance <= 25)
                RenderMesh(meshList[GEO_CUSTOMERHUNGRY], false);
        }
        else
            RenderMesh(meshList[GEO_CUSTOMERANGRY], false);
        //else
          //  RenderMesh(meshList[GEO_CUSTOMER], false);
        modelStack.PopMatrix();
    }
    /**/

    /*SAMUELAI*/
    //float _x = 80 / 133;
    //float _y = 60 / 190;
    //for (int i = 0; i < customerList.size(); ++i)
    //{
    //    ss.str(std::string());
    //    ss.precision(5);
    //    ss << customerList[i]->stateToString(customerList[i]->state);
    //    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, customerList[i]->getPos().x * _x, customerList[i]->getPos().y * _y);
    //}
    //ss.str(std::string());
    //ss << ramsay->stateToString(ramsay->currState);
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, ramsay->getPos().x * _x, ramsay->getPos().y * _y);
    //ss.str(std::string());
    //ss << s_waiter->stateToString(s_waiter->state);
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, s_waiter->getPos().x * _x, s_waiter->getPos().y * _y);

    /*float _x = 80 / m_worldWidth;
    float _y = 60 / m_worldHeight;*/
   
    for (int i = 0; i < customerList.size(); ++i)
    {
        float _x = 8.5f + customerList[i]->getPos().x * 0.3;
        float _y = 25.7f + customerList[i]->getPos().y * 0.2;

        ss.str(std::string());
        ss.precision(5);
        ss << customerList[i]->stateToString(customerList[i]->state);
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, _x, _y);
    }

    ss.str(std::string());
    ss << ramsay->stateToString(ramsay->currState);
    float _x = -4.f + ramsay->getPos().x * 0.6f;
    float _y = 2.f + ramsay->getPos().y * 0.6f;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, _x, _y);

    ss.str(std::string());
    ss << s_waiter->stateToString(s_waiter->state);
    float _x2 = -4.f + s_waiter->getPos().x* 0.6f;
    float _y2 = 30.f + s_waiter->getPos().y * 0.1f;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, _x2, _y2);
    //for (int i = 0; i < customerList.size(); ++i)
    //{
    //    ss.str(std::string());
    //    ss.precision(5);
    //    ss << customerList[i]->stateToString(customerList[i]->state);
    //    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, 0, 0);
    //}
    //ss.str(std::string());
    //ss << ramsay->stateToString(ramsay->currState);
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, 30, 0);
    //ss.str(std::string());
    //ss << s_waiter->stateToString(s_waiter->state);
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.5, 60, 0);
    /**/
}

void SceneAssignment1::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, 133, 0, 190, -10, 10);
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

    modelStack.PushMatrix();
    modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, -8.1);
    modelStack.Scale(m_worldWidth, m_worldHeight, 1);
    RenderMesh(meshList[GEO_FLOOR], false);
    modelStack.PopMatrix();

    RenderRayner();

    projection.SetToOrtho(0, 133, -200, 190, -10, 10);
    projectionStack.LoadMatrix(projection);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

    RenderSamuel();
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

	// Delete customers
	for (int i = 3; i < entityMgr->GetEntityMap().size(); i++)
	{
		entityMgr->GetEntityMap()[i] = NULL;
		delete entityMgr->GetEntityMap()[i];
	}

	if (waiter)
	{
		waiter = NULL;
		delete waiter;
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
}
