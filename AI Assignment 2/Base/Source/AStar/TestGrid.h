#ifndef TEST_GRID_H
#define TEST_GRID_H

#include "Vector3.h"

class CTestGrid
{
public:
	CTestGrid();
	~CTestGrid();

	void SetAABB(Vector3 minAABB, Vector3 maxAABB);

private:
	Vector3 minAABB;
	Vector3 maxAABB;
};

#endif // TEST_GRID_H