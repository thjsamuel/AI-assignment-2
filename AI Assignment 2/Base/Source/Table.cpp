#include "Table.h"

int CTable::m_ID = 0;

CTable::CTable(Vector3 pos) : position(pos), bActive(false), bUsing(false)
{
	m_ID++;
	bActive = true;
}

CTable::~CTable()
{
}

void CTable::AddSeat(const Vector3& pos)
{
	seatList.push_back(pos);
}

void CTable::SetActive(bool _bActive)
{
	this->bActive = _bActive;
}

void CTable::SetUsingState(bool _bUsing)
{
	this->bUsing = _bUsing;
}

bool CTable::GetUsingState()
{
	return bUsing;
}

bool CTable::GetActive() const
{
	return bActive;
}

unsigned int CTable::GetNumSeats()
{
	return seatList.size();
}

int CTable::GetID()
{
	return m_ID;
}

Vector3 CTable::GetPos()
{
	return position;
}

std::vector<Vector3>* CTable::GetSeatList()
{
	return &seatList;
}