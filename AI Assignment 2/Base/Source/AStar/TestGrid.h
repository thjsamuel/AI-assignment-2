#ifndef TEST_GRID_H
#define TEST_GRID_H

#include "Vector3.h"

class CTestGrid
{
public:
	CTestGrid();
	~CTestGrid();

	void SetPosition(const Vector3& position);
	Vector3 GetPosition() const;

	void SetAABB(Vector3 minAABB, Vector3 maxAABB);

	bool bCollided;

private:
	Vector3 position;

	Vector3 minAABB;
	Vector3 maxAABB;
};

#endif // TEST_GRID_H