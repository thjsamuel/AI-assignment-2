#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include <vector>

struct Rotation
{
	float degrees;
	float axisX;
	float axisY;
	float axisZ;

	void Set(float a = 0, float b = 0, float c = 0, float d = 1)
	{
		degrees = a;
		axisX = b;
		axisY = c;
		axisZ = d;
	}
};

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_ASTEROID,
		GO_ASTEROID2,
		GO_PLAYERSHIP,
		GO_BULLET,
		GO_MISSILE,
		GO_HEALTHPACK,
		GO_EARTH,
		GO_FRIENDLY1,
		GO_FRIENDLY2,
		GO_FRIENDLY3,
		GO_FRIENDLY4,
		GO_REDBOX,
		GO_DINO1,
		GO_WALL,
		GO_PILLAR,
		GO_HOLE,
		GO_TOTAL, //must be last
	};

	enum ASTEROID_TYPE
	{
		ATYPE_NONE = 0,
		ATYPE_SMALL,
		ATYPE_BIG,
		ATYPE_TOTAL,
	};

	enum SPAWN_LOCATION
	{
		SLOC_LEFT,
		SLOC_RIGHT,
		SLOC_TOP,
		SLOC_TOTAL,
	};

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();

	static void Init();

	static GameObject* FetchGO();
	static std::vector<GameObject *> m_goList;

	void FindTimeToCollide(GameObject *go, GameObject *go2, Vector3 N);
	void ApplyImpulse(GameObject *go, GameObject *go2, Vector3 N);

	void ApplyFriction(float scale, double dt);
	void ApplyInelasticCollision(float scale);

	Vector3 GetSpawnLoc();

//private:
	float m_worldWidth;
	float m_worldHeight;

	GAMEOBJECT_TYPE type;
	ASTEROID_TYPE asteroidType;
	SPAWN_LOCATION spawnLoc;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	Rotation rotation;
	bool active;
	float mass;

	Vector3 impulse;
	float th;
	float speed;
};

#endif