#ifndef SEAT_ARRANGER_H
#define SEAT_ARRANGER_H

#include "Vector3.h"
#include <vector>

struct Furniture;

class SeatArranger
{
public:
	SeatArranger();
	~SeatArranger();

	bool ArrangeSeats(unsigned int _numSeats, const Vector3& _position, double _dt);
	std::vector<Vector3>* GetSeatPositions();

private:
	unsigned int m_numSeats;
	double timer;
	double time;
	double value;
	double pauseTime;
	bool pauseTimeSet;
	Vector3 placementPos;
	bool bPosSet;

	std::vector<Vector3>* seatPositions;
};

#endif // SEAT_ARRANGER_H