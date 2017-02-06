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

	void Update();

	void AddSeat(const Vector3& pos);
	void FillUpSeats();
	bool CheckEmptySeats();

	void SetActive(bool _bActive);
	bool GetActive() const;

	void SetUsingState(bool _bUsing);
	bool GetUsingState();

	bool GetRemoveStatus();

	unsigned int GetNumSeats();

	unsigned int GetID();
	Vector3 GetPos();
	std::vector<Seat*>* GetSeatList();

private:
	unsigned int m_ID;
	bool bActive;
	bool bUsing;
	bool bRemove;

	Vector3 position;
	double timer;

	std::vector<Seat*> seatList;
};

#endif // TABLE_H