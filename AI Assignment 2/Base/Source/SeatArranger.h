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

	Vector3 GetTablePosition();
	bool ArrangeSeats(unsigned int _numSeats, const Vector3& _position, double _dt);
	bool AddSeat(unsigned int _tableID, unsigned int _numSeats);
	bool RemoveSeats(unsigned int _tableID);

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

	bool bTable1Taken;
	bool bTable2Taken;
	bool bTable3Taken;
};

#endif // SEAT_ARRANGER_H