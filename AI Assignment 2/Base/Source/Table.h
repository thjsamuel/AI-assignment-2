#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include "Vector3.h"

struct Seat
{
	Seat() : position(0, 0, 0), bUsing(false) {};
	~Seat() {};

	Vector3 position;
	bool bUsing;
};

class CTable
{
public:
	CTable(Vector3 pos);
	~CTable();

	void AddSeat(const Vector3& pos);
	void FillUpSeats();
	bool CheckEmptySeats();

	void SetActive(bool _bActive);
	bool GetActive() const;

	void SetUsingState(bool _bUsing);
	bool GetUsingState();

	unsigned int GetNumSeats();

	int GetID();
	Vector3 GetPos();
	std::vector<Seat*>* GetSeatList();

private:
	int m_ID;
	bool bActive;
	bool bUsing;

	Vector3 position;

	std::vector<Seat*> seatList;
};

#endif // TABLE_H