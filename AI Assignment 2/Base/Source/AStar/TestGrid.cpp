#include "TestGrid.h"

CTestGrid::CTestGrid() 
	: position(0, 0, 0)
	, minAABB(0, 0, 0)
	, maxAABB(0, 0, 0)
	, bCollided(false)
{
	SetAABB(Vector3(3, 3, 0), Vector3(-3, -3));
}

CTestGrid::~CTestGrid()
{
}

void CTestGrid::SetPosition(const Vector3& position)
{
	this->position = position;
}

Vector3 CTestGrid::GetPosition() const
{
	return position;
}

void CTestGrid::SetAABB(Vector3 minAABB, Vector3 maxAABB)
{
	this->minAABB = minAABB;
	this->maxAABB = maxAABB;
}

