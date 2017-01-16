#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Furniture.h"

#include <map>
#include <vector>

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

	void SetLatestID(unsigned int m_ID);
	unsigned int GetLatestID();

	void SetStillHereID(unsigned int stillHereID);
	unsigned int GetStillHereID();
	void IncrementStillHereID();
	
	void SetStartingSize(unsigned int _startingSize);
	unsigned int GetStartingSize();

	unsigned int GetNextID();

	std::vector<Furniture*>* GetFurnitureList();

private:
	// pointers to entities are cross referenced by their ID
	EntityMap m_EntityMap;
	unsigned int m_ID;
	unsigned int m_stillHereID;
	unsigned int m_startingSize;

	// List of furnitures
	std::vector<Furniture*> furnitureList;

	CEntityManager();
};

#endif // ENTITY_MANAGER_H