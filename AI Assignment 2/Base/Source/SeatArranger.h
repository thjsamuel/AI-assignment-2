#ifndef SEAT_ARRANGER_H
#define SEAT_ARRANGER_H

#include "Vector3.h"
#include <vector>

struct Furniture;
class CTable;

class SeatArranger
{
public:
	SeatArranger();
	~SeatArranger();

	bool ArrangeSeats(unsigned int _numSeats, const Vector3& _position, double _dt);

private:
	CTable* table;

	unsigned int m_numSeats;
	double timer;
	double time;
	double value;
	double pauseTime;
	bool pauseTimeSet;
	Vector3 placementPos;
	bool bPosSet;
};

#endif // SEAT_ARRANGER_H