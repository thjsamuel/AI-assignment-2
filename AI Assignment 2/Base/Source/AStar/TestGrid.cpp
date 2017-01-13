#include "TestGrid.h"

CTestGrid::CTestGrid() 
	: minAABB(0, 0, 0), maxAABB(0, 0, 0)
{
	SetAABB(Vector3(3, 3, 0), Vector3(-3, -3));
}

CTestGrid::~CTestGrid()
{
}

void CTestGrid::SetAABB(Vector3 minAABB, Vector3 maxAABB)
{
	this->minAABB = minAABB;
	this->maxAABB = maxAABB;
}

