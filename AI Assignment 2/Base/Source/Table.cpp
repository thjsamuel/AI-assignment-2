#include "Table.h"

int index = 0;
static int nextID = 0;

CTable::CTable(Vector3 pos) : position(pos), bActive(true), bUsing(false), m_ID(0), timer(0.0), bRemove(false)
{
	m_ID = nextID + 1;
	nextID++;
}

CTable::~CTable()
{
}

void CTable::Update()
{
	if (!bUsing)
	{
		timer += 0.01;

		if (timer > 5.0)
		{
			bRemove = true;
		}
	}
	else
	{
		timer = 0.0;
	}
}

void CTable::AddSeat(const Vector3& pos)
{
	Seat* seat = new Seat();
	seat->bUsing = false;
	seat->position = pos;

	seatList.push_back(seat);
}

void CTable::FillUpSeats()
{
	for (int i = 0; i < seatList.size(); i++)
	{
		seatList[i]->bUsing = true;
	}
}

bool CTable::CheckEmptySeats()
{
	for (index = 0; index < seatList.size(); index++)
	{
		if (seatList[index]->bUsing == true)
			break;
	}

	if (index == (seatList.size()))
		return true;
	else
		return false;
	
	//return (index == (seatList.size()));
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

bool CTable::GetRemoveStatus()
{
	return bRemove;
}

bool CTable::GetActive() const
{
	return bActive;
}

unsigned int CTable::GetNumSeats()
{
	return seatList.size();
}

unsigned int CTable::GetID()
{
	return m_ID;
}

Vector3 CTable::GetPos()
{
	return position;
}

std::vector<Seat*>* CTable::GetSeatList()
{
	return &seatList;
}