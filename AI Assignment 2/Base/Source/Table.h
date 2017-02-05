#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include "Vector3.h"

class CTable
{
public:
	CTable(Vector3 pos);
	~CTable();

	void AddSeat(const Vector3& pos);

	void SetActive(bool _bActive);
	bool GetActive() const;

	void SetUsingState(bool _bUsing);
	bool GetUsingState();

	unsigned int GetNumSeats();

	int GetID();
	Vector3 GetPos();
	std::vector<Vector3>* GetSeatList();

private:
	static int m_ID;
	bool bActive;
	bool bUsing;

	Vector3 position;

	std::vector<Vector3> seatList;
};

#endif // TABLE_H