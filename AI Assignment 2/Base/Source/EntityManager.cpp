#include "EntityManager.h"
#include "BaseGameEntity.h"

#include <cassert>

CEntityManager::CEntityManager() 
		: m_ID(0)
		, m_stillHereID(0)
		, m_startingSize(0)
{
}

CEntityManager* CEntityManager::GetInstance()
{
	static CEntityManager instance;

	return &instance;
}

void CEntityManager::Exit()
{
	// Causes crashing sometimes
	/*for (int i = 0; i < m_EntityMap.size(); i++)
	{
		m_EntityMap.erase(m_EntityMap.find(m_EntityMap[i]->GetID()));
	}*/

	for (int i = 0; i < furnitureList.size(); i++)
	{
		furnitureList[i] = NULL;
		delete furnitureList[i];
	}

	for (int i = 0; i < tableList.size(); i++)
	{
		tableList[i] = NULL;
		delete tableList[i];
	}
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

void CEntityManager::SetLatestID(unsigned int m_ID)
{
	this->m_ID = m_ID;
}

unsigned int CEntityManager::GetLatestID()
{
	return m_ID;
}

void CEntityManager::SetStillHereID(unsigned int stillHereID)
{
	m_stillHereID = stillHereID;
}

unsigned int CEntityManager::GetStillHereID()
{
	return m_stillHereID;
}

void CEntityManager::IncrementStillHereID()
{
	m_stillHereID++;
}

void CEntityManager::SetStartingSize(unsigned int _startingSize)
{
	this->m_startingSize = _startingSize;
}

unsigned int CEntityManager::GetStartingSize()
{
	return m_startingSize;
}

unsigned int CEntityManager::GetNextID()
{
	++m_ID;
	return m_ID;
}

std::vector<Furniture*>* CEntityManager::GetFurnitureList()
{
	return &furnitureList;
}

std::vector<CTable*>* CEntityManager::GetTableList()
{
	return &tableList;
}