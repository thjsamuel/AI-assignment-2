#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include "Vector3.h"

class CTable
{
public:
	CTable();
	~CTable();

	void AddSeat(const Vector3& pos);

	int GetID();
	std::vector<Vector3>* GetSeatList();

private:
	static int m_ID;
	std::vector<Vector3> seatList;
};

#endif // TABLE_H