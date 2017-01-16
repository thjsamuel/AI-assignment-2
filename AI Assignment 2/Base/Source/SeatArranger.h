#ifndef SEAT_ARRANGER_H
#define SEAT_ARRANGER_H

#include "SingletonTemplate.h"
#include "Vector3.h"

struct Furniture;

class SeatArranger : public Singleton <SeatArranger>
{
	friend Singleton <SeatArranger>;
public:
	void ArrangeSeats(unsigned int _numSeats, const Vector3& _position, double _dt);

private:
	SeatArranger();
	~SeatArranger();

	unsigned int m_numSeats;
	double timer;
	double time;
	double pauseTime;
	Vector3 placementPos;
	bool bPosSet;
};

#endif // SEAT_ARRANGER_H