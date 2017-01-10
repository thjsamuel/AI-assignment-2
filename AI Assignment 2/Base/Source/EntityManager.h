#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <map>

class CBaseGameEntity;

class CEntityManager
{
	typedef std::map<int, CBaseGameEntity*> EntityMap;

public:
	static CEntityManager* GetInstance();

	void RegisterEntity(CBaseGameEntity* newEntity);
	void RemoveEntity(CBaseGameEntity* entity);

	CBaseGameEntity* GetEntityFromID(int ID) const;
	EntityMap GetEntityMap() const;

private:
	// pointers to entities are cross referenced by their ID
	EntityMap m_EntityMap;

	CEntityManager();
};

#endif // ENTITY_MANAGER_H