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
		table = new CTable();

		placementPos = _position;
		bPosSet = true;
	}

	if (!pauseTimeSet)
	{
		switch (_numSeats)
		{
		case 2:
			pauseTime = 0.45;
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

		placementPos.x += cos(value) * 0.5;
		placementPos.y += sin(value) * 0.5;
		time += tempDT;

		if (time < 1.3)
		{
			if (time - timer > pauseTime)
			{
				timer = time;

				table->AddSeat(placementPos);
				
				std::cout << "table id: " << table->GetID() << std::endl;
				CEntityManager::GetInstance()->GetTableList()->push_back(table); // grouped customers will get their seat positions from their assigned table's seatlist
			}
		}
		else
		{
			bPosSet = false;
			value = 0.0;
			time = 0.0;
			pauseTimeSet = false;

			return true;
		}
	}

	return false;
}