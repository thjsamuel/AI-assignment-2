#include "EntityManager.h"
#include "BaseGameEntity.h"

#include <cassert>

CEntityManager::CEntityManager()
{
}

CEntityManager* CEntityManager::GetInstance()
{
	static CEntityManager instance;

	return &instance;
}

void CEntityManager::RegisterEntity(CBaseGameEntity* newEntity)
{
	m_EntityMap.insert(std::make_pair(newEntity->GetID(), newEntity));
}

void CEntityManager::RemoveEntity(CBaseGameEntity* entity)
{
	m_EntityMap.erase(m_EntityMap.find(entity->GetID()));
}

CBaseGameEntity* CEntityManager::GetEntityFromID(int id) const
{
	EntityMap::const_iterator entity = m_EntityMap.find(id);

	// check whether the entity is part of the map
	assert((entity != m_EntityMap.end()) && "<CEntityManager::GetEntityFromID>: invalid ID");
	/*if (entity != m_EntityMap.end())
		return NULL;*/

	return entity->second;
}

std::map<int, CBaseGameEntity*> CEntityManager::GetEntityMap() const
{
	return m_EntityMap;
}