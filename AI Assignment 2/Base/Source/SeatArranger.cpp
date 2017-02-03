#include "SeatArranger.h"
#include "Furniture.h"
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
	seatPositions = new std::vector<Vector3>();
}

SeatArranger::~SeatArranger()
{
}

bool SeatArranger::ArrangeSeats(unsigned int _numSeats, const Vector3& _position, double _dt)
{
	if (bPosSet == false)
	{
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
		static double tempDT = 0.01;
		
		value += 5 * tempDT;

		placementPos.x += cos(value) * 0.5;
		placementPos.y += sin(value) * 0.5;
		time += tempDT;

		if (time < 1.3)
		{
			if (time - timer > pauseTime)
			{
				timer = time;

				Furniture* furniture = new Furniture(Vector3(placementPos.x, placementPos.y, 0), Vector3(3, 3, 0));
				CEntityManager::GetInstance()->GetFurnitureList()->push_back(furniture);
				seatPositions->push_back(furniture->position); // need to pop later
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

std::vector<Vector3>* SeatArranger::GetSeatPositions()
{
	return seatPositions;
}