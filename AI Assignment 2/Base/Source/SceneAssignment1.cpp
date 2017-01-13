#include "SceneAssignment1.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "MyTimer.h"
#include "EntityNames.h"
#include "Messaging\MessageDispatcher.h"
#include "Locations.h"
#include "AStar\TestGrid.h"

CMyTimer* clock_ = CMyTimer::GetInstance();

SceneAssignment1::SceneAssignment1()
{
}

SceneAssignment1::~SceneAssignment1()
{
}

void SceneAssignment1::Init()
{
	srand(time(NULL));

	SceneBase::Init();

	entityMgr = CEntityManager::GetInstance();

	waiter = new CWaiter(ENT_WAITER);
	entityMgr->RegisterEntity(waiter);

	chef = new CChef(ENT_CHEF);
	entityMgr->RegisterEntity(chef);

	cleaner = new CCleaner(ENT_CLEANER);
	entityMgr->RegisterEntity(cleaner);

	entityMgr->SetLatestID(entityMgr->GetEntityMap().size());

	customer = new CCustomer(entityMgr->GetNextID(), SEAT_1, true);
    customer->waypoints[0] = ENTRANCE;
	entityMgr->RegisterEntity(customer);
	entityMgr->SetStillHereID(customer->GetID());
    customer_list.push_back(customer);

	// For Debugging
	debugPos = Vector3(0, -10, 0);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	/*m_worldHeight = 190.f;
	m_worldWidth = m_worldHeight * (float) 133  / 190;*/
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

	/*seatNum = (rand() % 9 + 1);

	switch (seatNum)
	{
	case 1:
		theSeatPos = SEAT_1;
		break;
	case 2:
		theSeatPos = SEAT_2;
		break;
	case 3:
		theSeatPos = SEAT_3;
		break;
	case 4:
		theSeatPos = SEAT_4;
		break;
	case 5:
		theSeatPos = SEAT_5;
		break;
	case 6:
		theSeatPos = SEAT_6;
		break;
	case 7:
		theSeatPos = SEAT_7;
		break;
	case 8:
		theSeatPos = SEAT_8;
		break;
	case 9:
		theSeatPos = SEAT_9;
		break;
	}*/

	//std::cout << "theCustomer's ID: " << theCustomer->GetID() << std::endl;
	//std::cout << seatNum << std::endl;
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
	for (int i = entityMgr->GetStillHereID(); i <= entityMgr->GetLatestID(); i++)
	{
		// other way: when exited, remove from entity manager
		// bad way: make a virtual function to set and get exited status just for customer's use, since cannot use dynamic_cast
		// when exited == true, don't update and render
		if (entityMgr->GetEntityFromID(i)->GetExitStatus() == false)
			entityMgr->GetEntityFromID(i)->Update(dt);
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
	modelStack.Translate(25, 75, 0);
	modelStack.Scale(58, 3, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// divider 1
	modelStack.PushMatrix();
	modelStack.Translate(55, 87, 0);
	modelStack.Scale(3, 27, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// divider 2
	modelStack.PushMatrix();
	modelStack.Translate(80, 87, 0);
	modelStack.Scale(3, 27, 3);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	// divider 3
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

	for (float x = 3; x < (m_worldWidth - 3); x += 5.f)
	{
		for (float y = 3; y < (m_worldHeight - 3); y += 5.f)
		{
			//CTestGrid* grid = new CTestGrid();

			/*modelStack.PushMatrix();
			modelStack.Translate(x, y, 0);
			modelStack.Scale(0.5, 0.5, 0.5);
			RenderMesh(meshList[GEO_BALL], false);
			modelStack.PopMatrix();*/
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
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

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
	for (int i = 4; i < entityMgr->GetEntityMap().size(); i++)
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
