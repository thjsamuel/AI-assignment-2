#include "GameObject.h"
#include "Application.h"

std::vector<GameObject *> GameObject::m_goList;

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue)
	, scale(1, 1, 1)
	, active(false)
	, mass(1.f)
	, pos(0, 0, 0)
	, vel(0, 0, 0)
	, normal(1, 0, 0)
	, asteroidType(ATYPE_NONE)
	, spawnLoc(SLOC_LEFT)
	, impulse(0, 0, 0)
	, th(0)
	, speed(0)
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	// Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (unsigned i = 0; i < 100; i++)
	{
		GameObject *go = new GameObject(GO_ASTEROID);
		m_goList.push_back(go);
	}
}

Vector3 GameObject::GetSpawnLoc()
{
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	spawnLoc = static_cast<SPAWN_LOCATION>(rand() % SLOC_TOTAL); 

	if (spawnLoc == SLOC_LEFT)
		return Vector3(0, Math::RandFloatMinMax(40, m_worldHeight), 0); //return Vector3(5, m_worldHeight * 0.5f, 0);
	else if (spawnLoc == SLOC_RIGHT)
		return Vector3(m_worldWidth, Math::RandFloatMinMax(40, m_worldHeight), 0); //return Vector3(m_worldWidth - 5, m_worldHeight * 0.5f, 0);
	else if (spawnLoc == SLOC_TOP)
		return Vector3(Math::RandFloatMinMax(0, m_worldWidth), m_worldHeight, 0); //return Vector3(m_worldWidth * 0.5f, m_worldHeight - 5, 0);
}


void GameObject::FindTimeToCollide(GameObject *go, GameObject *go2, Vector3 N)
{
	Vector3 u1 = go->vel;
	Vector3 u1n = u1.Dot(N) * N;
	go->vel = u1 - 2 * u1n;
	Vector3 v1 = go->vel;

	// Find time to collide 
	Vector3 w0 = go2->pos;
	Vector3 b1 = go->pos;
	Vector3 dir = w0 - b1;
	if (dir.Dot(N) < 0)
		N = -N;
	Vector3 U = go->vel;

	if (U.Dot(N) > 0)
		return;

	float r = go->scale.x;
	float h = go2->scale.x;
	w0 -= (r + h * 0.5) * N;
	float dist = (w0 - b1).Dot(N);
	speed = U.Dot(N);
	th = dist / speed;

	impulse = go->mass * (v1 - u1);
}

void GameObject::ApplyImpulse(GameObject *go, GameObject *go2, Vector3 N)
{
	FindTimeToCollide(go, go2, N);
	Vector3 force = impulse * (1 / th);
	Vector3 accel = force * (1 / mass);
	Vector3 vel = (accel * th) + speed;
	this->vel = vel * 0.5; // this->vel += vel;
}

void GameObject::ApplyFriction(float scale, double dt)
{
	vel -= vel * scale * dt;
}

void GameObject::ApplyInelasticCollision(float scale)
{
	vel = scale * vel;
	//vel -= vel * scale;
}

GameObject* GameObject::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; i++) // keep replenishing GOs
	{
		GameObject *go2 = new GameObject(GameObject::GO_NONE);
		m_goList.push_back(go2);
	}

	GameObject *go = *(m_goList.end() - 1); 
	go->active = true;
	return go;
}