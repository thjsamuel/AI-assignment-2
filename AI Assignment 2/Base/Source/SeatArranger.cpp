#include "SeatArranger.h"
#include "EntityManager.h"

SeatArranger::SeatArranger()
	: m_numSeats(0)
	, timer(0.0)
	, time(0.0)
	, pauseTime(0.0)
	, pauseTimeSet(false)
	, value(0.0)
	, placementPos(0, 0, 0)
	, bPosSet(false)
{
}

SeatArranger::~SeatArranger()
{
}

bool SeatArranger::ArrangeSeats(unsigned int _numSeats, const Vector3& _position, double _dt)
{
	if (bPosSet == false)
	{
		placementPos = _position;
		table = new CTable(Vector3(placementPos.x, placementPos.y * 1.13, 0));
		bPosSet = true;
	}

	if (!pauseTimeSet)
	{
		switch (_numSeats)
		{
		case 2:
			pauseTime = 0.6; // 0.45
			break;

		case 3:
			pauseTime = 0.35;
			break;

		case 4:
			pauseTime = 0.3;
			break;

		case 5:
			pauseTime = 0.25;
			break;

		case 6:
			pauseTime = 0.2;
			break;
		}

		pauseTimeSet = true;
	}

	else
	{
		// Framerate is inconsistent = seat positions are inconsistent
		double tempDT = 0.01;
		value += 5 * tempDT;

		float distApart = 0.35f; // 0.5f;
		placementPos.x += cos(value) * distApart;
		placementPos.y += sin(value) * distApart;

		time += tempDT;

		if (time < 1.3)
		{
			if (time - timer > pauseTime)
			{
				timer = time;

				table->AddSeat(placementPos);
			}
		}
		else
		{
			std::cout << "table id: " << table->GetID() << std::endl;
			CEntityManager::GetInstance()->GetTableList()->push_back(table); // grouped customers will get their seat positions from their assigned table's seatlist

			bPosSet = false;
			value = 0.0;
			time = 0.0;
			pauseTimeSet = false;

			return true;
		}
	}

	return false;
}

bool SeatArranger::AddSeat(unsigned int _tableID, unsigned int _numSeats)
{
	/* How adding seats to already arranged table works:
	1. When 1st group comes in, waiter gets table from store room and sets up table and seats in the dining area
	2. When this group finishes eating and no more groups come within 10 secs, waiter will put the table back in the store room
	3. If another group comes within 10 secs, waiter will add more seats until everyone in that group can be seated
	*/

	std::cout << "running AddSeat()" << std::endl;

	return true; // false
}

bool SeatArranger::RemoveSeats(unsigned int _tableID)
{
	CTable* theTable;

	for (int i = 0; i < CEntityManager::GetInstance()->GetTableList()->size(); i++)
	{
		table = CEntityManager::GetInstance()->GetTableList()->at(i);

		if (table->GetID() == _tableID)
		{
			theTable = table;
			break;
		}
	}

	// Run timer etc...
	for (int i = 0; theTable->GetNumSeats(); i++)
	{
		theTable->GetSeatList()->pop_back();
	}

	theTable->SetActive(false);

	// Remove table from CEntityManager::GetInstance()->GetTableList()
	// ...

	return true; // false
}