#include "BaseGameEntity.h"
#include <iostream>

int CBaseGameEntity::m_iNextValidID = 0;

CBaseGameEntity::CBaseGameEntity(int ID) : position(0, 0, 0), seatPos(0, 0, 0), bSeated(false), bExited(false), bDone(false), bSpriteInvert(false), bCooking(false), bIdle(false)
{
	SetID(ID);
}

CBaseGameEntity::~CBaseGameEntity()
{
}

int CBaseGameEntity::GetID()
{
	return m_iID;
}

void CBaseGameEntity::SetID(int value)
{
	if (value < m_iNextValidID)
		std::cout << "An entity has an invalid ID" << std::endl;

	m_iID = value;
	m_iNextValidID = m_iID + 1;
}

void CBaseGameEntity::SetPosition(float x, float y, float z)
{
	this->position.Set(x, y, z);
}

Vector3 CBaseGameEntity::GetPosition()
{
	return position;
}

void CBaseGameEntity::SetSpriteInvertStatus(bool bSpriteInvert)
{
	this->bSpriteInvert = bSpriteInvert;
}

bool CBaseGameEntity::GetSpriteInvertStatus()
{
	return bSpriteInvert;
}

void CBaseGameEntity::SetIdleState(bool bIdle)
{
	this->bIdle = bIdle;
}

bool CBaseGameEntity::GetIdleState()
{
	return bIdle;
}

void CBaseGameEntity::SetCookingState(bool bCooking)
{
	this->bCooking = bCooking;
}

bool CBaseGameEntity::GetCookingState()
{
	return bCooking;
}

void CBaseGameEntity::SetSeatPosition(Vector3 seatPos)
{
	this->seatPos = seatPos;
}

Vector3 CBaseGameEntity::GetSeatPosition()
{
	return seatPos;
}

void CBaseGameEntity::SetSeatedStatus(bool bSeated)
{

	this->bSeated = bSeated;
}

bool CBaseGameEntity::GetSeatedStatus()
{
	return bSeated;
}

void CBaseGameEntity::SetExitStatus(const bool bExited)
{
	this->bExited = bExited;
}

bool CBaseGameEntity::GetExitStatus() const
{
	return bExited;
}

void CBaseGameEntity::SetDone(bool bDone)
{
	this->bDone = bDone;
}

bool CBaseGameEntity::GetDoneStatus() const
{
	return bDone;
}
