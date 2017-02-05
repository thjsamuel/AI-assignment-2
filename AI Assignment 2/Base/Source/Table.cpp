#include "Table.h"

int CTable::m_ID = 0;

CTable::CTable()
{
	m_ID++;
}

CTable::~CTable()
{
}

void CTable::AddSeat(const Vector3& pos)
{
	seatList.push_back(pos);
}

int CTable::GetID()
{
	return m_ID;
}

std::vector<Vector3>* CTable::GetSeatList()
{
	return &seatList;
}