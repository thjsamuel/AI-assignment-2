#ifndef SEAT_ARRANGER_H
#define SEAT_ARRANGER_H

#include "Vector3.h"

struct Furniture;

class SeatArranger
{
public:
	SeatArranger();
	~SeatArranger();

	bool ArrangeSeats(unsigned int _numSeats, const Vector3& _position, double _dt);

private:
	unsigned int m_numSeats;
	double timer;
	double time;
	double pauseTime;
	Vector3 placementPos;
	bool bPosSet;
};

#endif // SEAT_ARRANGER_H